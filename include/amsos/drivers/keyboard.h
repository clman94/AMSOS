#ifndef DRIVER_KEYBOARD_H
#define DRIVER_KEYBOARD_H

#include <stdint.h>

enum scancodes{
	SCANCODE_ESCAPE = 0x01,
	SCANCODE_ENTER  = 0x1C,
	SCANCODE_LCTRL  = 0x1D,
	SCANCODE_LSHIFT = 0x2A,
	SCANCODE_RSHIFT = 0x36,
	SCANCODE_LALT   = 0x39,
	SCANCODE_CAPS   = 0x3E,
	SCANCODE_F1     = 0x3B,
	SCANCODE_F2     = 0x3C,
	SCANCODE_F3     = 0x3D,
	SCANCODE_F4     = 0x3E,
	SCANCODE_F5     = 0x3F,
	SCANCODE_F6     = 0x40,
	SCANCODE_F7     = 0x41,
	SCANCODE_F8     = 0x42,
	SCANCODE_F9     = 0x43,
	SCANCODE_F10    = 0x44,
};

uint8_t keyboard_get_ascii(bool wait);

void keyboard_clear_buffer();
void keyboard_enable_buffer();
void keyboard_disable_buffer();

void keyboard_enable_direct();
void keyboard_disable_direct();
bool keyboard_key_status(uint8_t scan);

void IRQH_keyboard_controller();

#endif