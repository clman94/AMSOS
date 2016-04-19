#ifndef ACPI_RSDT_H
#define ACPI_RSDT_H

struct ACPI_SDT_header
{
	char     signature[4];
	uint32_t length;
	uint8_t  revision;
	uint8_t  checksum;
	char     OEM_ID[6];
	char     OEM_table_ID[8];
	uint32_t OEM_revision;
	uint32_t creator_ID;
	uint32_t creator_revision;
} __attribute__ ((packed));

struct RSDT_descriptor
{
	ACPI_SDT_header h;
	uint32_t sdt_ptr[];
} __attribute__ ((packed));

struct XSDT_descriptor
{
  ACPI_SDT_header h;
  uint64_t sdt_ptr[];
} __attribute__ ((packed));

#endif