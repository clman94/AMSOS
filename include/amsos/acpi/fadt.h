#ifndef ACPI_FADT_H
#define ACPI_FADT_H

#include <amsos/acpi/sdt.h>
#include <stdint.h>

struct FADT_descriptor
{
	ACPI_SDT_header h;
	uint32_t firmware_ctrl;
	uint32_t DSDT;
	uint8_t  _reverved;
	uint8_t  preferred_profile;
	uint16_t SCI_interrupt;
	uint32_t SMI_command_port;
	uint8_t  ACPI_enable;
	uint8_t  ACPI_disable;
	uint8_t  S4BIOS_REQ;
    uint8_t  PSTATE_ctrl;
    uint32_t PM1_event_block[2];
    uint32_t PM1_ctrl_block[2];
    uint32_t PM2_ctrl_block;
	uint32_t PM_timer_block;
    uint32_t GPE0_block;
    uint32_t GPE1_block;
    uint8_t  PM1_event_Length;
    uint8_t  PM1_ctrl_Length;
    uint8_t  PM2_ctrl_Length;
	char _unused[49];
	uint64_t X_DSDT;
	// More will be added when needed.
} __attribute__ ((packed));

#endif