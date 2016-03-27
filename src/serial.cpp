#include <stdint.h>
#include <include/ports.h>

#define PORT_COM1         0x3F8
#define PORT_COM2         0x2F8
#define PORT_COM3         0x3E8
#define PORT_COM4         0x2E8

#define REG_DATA          0x00
#define REG_INT_ENABLE    0x01
#define REG_L_RATE        0x00
#define REG_H_RATE        0x01
#define REG_INTID_FIFO    0x02
#define REG_LINE_CTRL     0x03
#define REG_MODEM_CTRL    0x04
#define REG_LINE_STAT     0x05
#define REG_MODEM_STAT    0x06
#define REG_SCRATCH       0x07

#define INT_DISABLE       0x00
#define INT_DR            1
#define INT_THRE         (1<<2)
#define INT_STATUS       (1<<3)
#define INT_CHANGE       (1<<4)

#define DLAB_ENABLE      (1<<8)

#define FIFO_ENABLE       1
#define FIFO_CL_RECEIVER (1<<1)
#define FIFO_CL_TRANS    (1<<2)
#define FIFO_DMA_MODE    (1<<3)
#define FIFO_TRIG_LVL_1   0
#define FIFO_TRIG_LVL_4  (1<<6)
#define FIFO_TRIG_LVL_8  (2<<6)
#define FIFO_TRIG_LVL_14 (3<<6)

#define LINE_CTRL_LEN_5      0
#define LINE_CTRL_LEN_6      1
#define LINE_CTRL_LEN_7      2
#define LINE_CTRL_LEN_8      3
#define LINE_CTRL_STOP      (1<<2)
#define LINE_CTRL_PARITY(A) (A<<3)

#define MODEM_CTRL_DTR   1
#define MODEM_CTRL_RTS  (1<<2)
#define MODEM_CTRL_OUT1 (1<<2)
#define MODEM_CTRL_OUT2 (1<<3)
#define MODEM_CTRL_LOOP (1<<4)

// Used as a selecter for _com parameters so the ports arn't exposed
static const uint16_t comsel[4] = { PORT_COM1, PORT_COM2, PORT_COM3, PORT_COM4 };

int is_transmit_empty(int _com) {
   return port_in_b(comsel[_com] + REG_LINE_STAT) & 0x20;
}
 
void serial_COM_send(int _com, char data)
{
   while (is_transmit_empty(_com) == 0);
 
   port_out_b(comsel[_com], data);
}

void SERIAL_init(int _com, uint16_t baud_div, char bits)
{
	// Select port to use
	
	uint16_t com = comsel[_com];
	
	// Disable interrupts
	port_out_b(com + REG_INT_ENABLE, INT_DISABLE);
	
	// Enable DLAB (in send divisor)
	port_out_b(com + REG_LINE_CTRL, DLAB_ENABLE);
	
	// Set divisor
	port_out_b(com + REG_L_RATE, (uint8_t)(baud_div&0x00FF));
	port_out_b(com + REG_H_RATE, (uint8_t)(baud_div>>8));
	
	// Set transfer info
	port_out_b(com + REG_LINE_CTRL, 
		LINE_CTRL_PARITY(0) | 
		bits); // Expects the use of SERIAL_BITS_? in seral.h
	
	// Enable FIFO, cleared, and 14b thrreshold
	port_out_b(com + REG_INTID_FIFO, 
		FIFO_ENABLE       | 
		FIFO_CL_RECEIVER  | 
		FIFO_CL_TRANS     | 
		FIFO_TRIG_LVL_14);
	
	// Setup modem control
	port_out_b(com + REG_MODEM_CTRL, 
		MODEM_CTRL_DTR  |
		MODEM_CTRL_RTS  |
		MODEM_CTRL_OUT2);
	
}