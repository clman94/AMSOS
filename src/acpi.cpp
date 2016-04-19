
#include <amsos/library/string.h>
#include <amsos/library/checksum.h>
#include <amsos/library/io.h>

#include <amsos/acpi/rsdp.h>
#include <amsos/acpi/sdt.h>
#include <amsos/acpi/fadt.h>

#include <amsos/ports.h>
#include <amsos/ramfs.h>

#include <stdint.h>


RSDP_descriptor   *rsdp = nullptr;
RSDP20_descriptor *rsdp2 = nullptr;

RSDT_descriptor *rsdt = nullptr;
XSDT_descriptor *xsdt = nullptr;

FADT_descriptor *fadt = nullptr;

ram_dir* ACPI_dir;

uint8_t SLP_TYPa, SLP_TYPb;

RSDP_descriptor* ACPI_find_RSDP_descriptor()
{
	RSDP_descriptor* r = nullptr;
	
	for (size_t i = 0xE0000; i < 0x100000; i += 0x10/sizeof(void*))
	{
		r = (RSDP_descriptor*)i;
		if (!strncmp(r->signature, RSDP_SIGNATURE, 8) &&
		    checksum<RSDP_descriptor>(r))
			return r;
	}
	
	uint32_t EBDA_address = *((uint16_t*)0x040E) >> 4;
	for (size_t i = EBDA_address; i < EBDA_address + 0x400; i += 0x10/sizeof(void*))
	{
		r = (RSDP_descriptor*)i;
		if (!strncmp(r->signature, RSDP_SIGNATURE, 8) &&
		    checksum<RSDP_descriptor>(r))
			return r;
	}
	return nullptr;
}

ACPI_SDT_header* acpi_find_SDT(RSDT_descriptor *r, const char* name)
{
	int e = (r->h.length - sizeof(ACPI_SDT_header)) / 4;
	
	for (int i = 0; i < e; i++)
	{
		ACPI_SDT_header *h = (ACPI_SDT_header*)(r->sdt_ptr[i]);
		if (!strncmp(h->signature, name, 4))
			return h;
	}
	return nullptr;
}

ACPI_SDT_header* acpi_find_SDT_2(XSDT_descriptor *r, const char* name)
{
	int e = (r->h.length - sizeof(r->h)) / 8;
	
	for (int i = 0; i < e; i++)
	{
		ACPI_SDT_header *h = (ACPI_SDT_header*)(r->sdt_ptr[i]);
		if (!strncmp(h->signature, name, 4))
			return h;
	}
	return nullptr;
}

int ACPI_enable()
{
	// Check if enabled
	if((port_in_w(fadt->PM1_ctrl_block[0]) & 1) == 1)
		return 1;
	
	// Check ability to be enabled
	if (!fadt->ACPI_enable)
		return 2;
	
	if (!fadt->SMI_command_port)
		return 3;
	
	// Send enable command
	port_out_b(fadt->SMI_command_port, fadt->ACPI_enable);
	
	int timeout = 0;
	while ((port_in_w(fadt->PM1_ctrl_block[0]) & 1) == 0 && timeout < 1000000) { ++timeout; } // wait for a
	
	if (fadt->PM1_ctrl_block[1] != 0)
		while ((port_in_w(fadt->PM1_ctrl_block[1]) & 1) == 0 && timeout < 1000000) { ++timeout; } // wait for b

	return 0;
}

#define RESERVE_AS_FILE(A, B, C) make_file(ACPI_dir, A, (mem_addr)B, (mem_addr)B + (mem_addr)C, RAM_FILE_TYPE_FILE, true)

void ACPI_power_off()
{
	printf("[ACPI] Shutting down...\n");
	
	printf("[ACPI] EC : %c\n", ACPI_enable() + '0');
	
	port_out_w(fadt->PM1_ctrl_block[0], SLP_TYPa | (1<<13));
	if (fadt->PM1_ctrl_block[1] != 0)
		port_out_w(fadt->PM1_ctrl_block[1], SLP_TYPb | (1<<13));
	
	printf("[ACPI] Shutdown Failed\n");
}

int ACPI_init_rev1()
{
	// Get RSDT
	rsdt = (RSDT_descriptor*)(rsdp->rsdt_address);
	
	// Find FADT
	fadt = (FADT_descriptor*)acpi_find_SDT(rsdt, "FACP");
	if (!fadt)
		return 1;
	printf("FADT Found\n");
	
	RESERVE_AS_FILE("RSDP", rsdp, sizeof(rsdp));
	RESERVE_AS_FILE("RSDT", rsdt, rsdt->h.length);
	
	return 0;
}

int ACPI_init_rev2()
{
	rsdp2 = (RSDP20_descriptor*)rsdp;
	
	if(!checksum(rsdp2))
		return 1;
	
	// Get XSDT
	xsdt = (XSDT_descriptor*)((uint32_t)rsdp2->xsdt_address);
	
	// Find FADT
	fadt = (FADT_descriptor*)acpi_find_SDT_2(xsdt, "FACP");
	if (!fadt)
		return 2;
	printf("FADT Found\n");
	
	RESERVE_AS_FILE("RSDP", rsdp2, sizeof(rsdp2));
	RESERVE_AS_FILE("XSDT", xsdt, xsdt->h.length);
	
	return 0;
}

int ACPI_init(ram_dir* dir)
{
	printf("[ACPI] Loading...\n");
	
	ACPI_dir = create_dir(dir, "ACPI");
	
	// Find RSDP
	rsdp = ACPI_find_RSDP_descriptor();
	if(!rsdp)
		return 1;
	printf("RSDP Found\nACPI Revision : %c\n", rsdp->revision + '0');
	if(rsdp->revision < 2)
		printf("[ACPI] init EC : %c\n", ACPI_init_rev1() + '0');
	else
		printf("[ACPI] init EC : %c\n", ACPI_init_rev2() + '0');
	
	RESERVE_AS_FILE("FADT", fadt, fadt->h.length);
	
	// Find shut down AML
	char* S5;
	size_t DSDT_length;
	
	if (rsdp->revision < 2){
		S5 = (char*)fadt->DSDT + sizeof(ACPI_SDT_header); // Skip header
		DSDT_length = ((ACPI_SDT_header*)fadt->DSDT)->length - sizeof(ACPI_SDT_header);
	}else{
		S5 = (char*)fadt->X_DSDT + sizeof(ACPI_SDT_header); // Skip header
		DSDT_length = ((ACPI_SDT_header*)fadt->X_DSDT)->length - sizeof(ACPI_SDT_header);
	}
	
	for (size_t i = 0; i < DSDT_length; i++)
	{
		if(!strncmp(S5, "_S5_", 4))
			break;
		++S5;
	}
	
	// Validate AML
	if ((*(S5-1) == 0x08 || (*(S5-2) == 0x08 && *(S5-1) == '\\')) && *(S5+4) == 0x12){}
	else
		return 3;
	
	S5 += 5;
	S5 += ((*S5 & 0xC0) >> 6) + 2;
	
	if (*S5 == 0x0A)
		S5++;   // skip byte prefix
	SLP_TYPa = *S5 << 10;
	S5++;
	
	if (*S5 == 0x0A)
		S5++;   // skip byte prefix
	SLP_TYPb = *S5 << 10;
	
	return 0;
}

