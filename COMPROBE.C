/*
 * COMPROBE.C - probe for a com port/UART on an address
 *
 * (c) 2016 Nils Stec <stecdose@gmail.com>
 * compiler: Turbo C 2.01 for DOS
 *
 * usage: ./COMPROBE <port>
 * if <port> is omitted, default port 0x3f8
 * will be used for probing.
 *
 * got that probe-routine from:
 * http://www.sci.muni.cz/docs/pc/serport.txt
 * http://www.webcitation.org/5ugQv5JOw
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dos.h>


/* CONSTANTS */
#define DEFAULT_PORT	0x3f8

/* FUNCTION PROTOTYPES */
void show_help(char *own_name);
int probe_uart(unsigned short port);

int main(int argc, char **argv) {
	unsigned short addr;
	char temp[9];
	int type;

	if(argc != 2) {
		show_help(argv[0]);
		addr = DEFAULT_PORT;
	} else {
		addr = (unsigned short)strtoul(argv[1], NULL, 16);
	}

	printf("using port 0x%03x for probing...\n", addr);
	type = probe_uart(addr);

	switch(type) {
		case 0:
			printf("no UART on 0x%03x (0)\n", addr);
			break;
		case 1:
			printf("UART is a 8250\n");
			break;
		case 2:
			printf("UART is a 16450 or 8250 with SR\n");
			break;
		case 3:
			printf("UART is a 16550\n");
			break;
		case 4:
			printf("UART is a 16550A\n");
			break;
		default:
			printf("no UART on 0x%03x (type=0x%04x)\n", addr, type);
			break;
	}

	printf("\n");
}

int probe_uart(unsigned short addr) {
	int x, old;

	/* check if there is a UART at all */
	old = inport(addr+4);
	outport(addr+4, 0x10);
	if((inport(addr+6)&0xf0)) return 0;		/* no uart */

	outport(addr+4, 0x1f);
	if((inport(addr+6)&0xf0) != 0xf0) return 0;	/* no uart */
	outport(addr+4, old);

	/* look for scratch register */
	old = inport(addr+7);
	outport(addr+7, 0x55);			/* write magic */
	if(inport(addr+7) != 0x55) return 1;	/* if it is not the same, we have 8250 */
	outport(addr+7, 0xAA);  		/* write magic */
	if(inport(addr+7) != 0xAA) return 1;
	outport(addr+7, old);

	/* check if there's a fifo */
	outport(addr+2, 1);
	x = inport(addr+2);
	outport(addr+2, 0x00);
	if((x&0x80) == 0) return 2;
	if((x&0x40) == 0) return 3;
	return 4;
}

void show_help(char *own_name) {
	printf("usage: %s <port>\n", own_name);
	printf("if <port> is ommited, 0x%03x will be used.\n\n", DEFAULT_PORT);
}
