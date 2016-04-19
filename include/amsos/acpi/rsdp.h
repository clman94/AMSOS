#ifndef ACPI_RSDP_H
#define ACPI_RSDP_H

#include <stdint.h>
#include <amsos/library/string.h>
#include <amsos/library/io.h>

#define RSDP_SIGNATURE "RSD PTR "

struct RSDP_descriptor
{
	char signature[8];
	uint8_t chechsum;
	char OEMID[6];
	uint8_t revision;
	uint32_t rsdt_address;
} __attribute__ ((packed));

struct RSDP20_descriptor
{
	RSDP_descriptor rsdp1;
	uint32_t length;
	uint64_t xsdt_address;
	uint8_t ex_checksum;
	uint8_t __reserved[3];
} __attribute__ ((packed));

#endif