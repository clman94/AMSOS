#ifndef DRIVER_SERIAL_H
#define DRIVER_SERIAL_H

#include <stdint.h>

#define SERIAL_COM1 0
#define SERIAL_COM2 1
#define SERIAL_COM3 2
#define SERIAL_COM4 3

#define SERIAL_BITS_5 0
#define SERIAL_BITS_6 1
#define SERIAL_BITS_7 2
#define SERIAL_BITS_8 3

void serial_COM_init(int _com, uint16_t baud_div, char bits);
void serial_COM_send(int _com, char data);
char serial_COM_recieve(int _com, bool wait);

void IRQH_serial_COM_1_3();
void IRQH_serial_COM_2_4();

#endif