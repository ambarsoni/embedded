/*-------------------------------------------------------------------------------------------
 * 	 Receiver Applicaiton: 	which receives data from another radio and outputs that data out
 * 						   	through the UART so it can be analyzed
 *   					
 *   Always blinks green LED.
 *   Toggles red LED every time a packet is received.
 *-----------------------------------------------------------------------------------------*/

#include "bsp.h"
#include "mrfi.h"
#include "radios/family1/mrfi_spi.h"
#include "uart.h"
#include "delay.h"
#include "encrypt.h"
#include <stdio.h>

#define RED_RECEIVE_LED 0x01
#define GREEN_LED   0x02

void main(void) {
	int e = 1;
	int n = 23;
	int phi_num = phi(n);
	int d = findD(e,n);
	
	/* Set a filter address for packets received by the radio
	 *   This should match the "destination" address of
	 *   the packets sent by the transmitter. */
	uint8_t address[] = {0x12,0x34,0xab,0xcd};
	
	/* Filter setup return value: success or failure */
	unsigned char status;
	
	/*uart set up */
	init_uart();	/* Setup UART (Note: Sets clock to 1 MHz) */
 	uart_clear_screen();
	
	/* Perform board-specific initialization */
	BSP_Init();
	
	/* Initialize minimal RF interface, wake up radio */
	MRFI_Init();
	MRFI_WakeUp();
			
	/* Attempt to turn on address filtering
	 *   If unsuccessful, turn on both LEDs and wait forever */
	status = MRFI_SetRxAddrFilter(address);	
	MRFI_EnableRxAddrFilter();
	if( status != 0) {
		P1OUT = RED_RECEIVE_LED | GREEN_LED;
		while(1);
	}
		
	/* Red and green LEDs are output, green starts on */
	P1DIR = RED_RECEIVE_LED | GREEN_LED;
	P1OUT = GREEN_LED;
	
	/* Turn on the radio receiver */
	MRFI_RxOn();
	
	/* Main loop just toggles the green LED with some delay */
	__bis_SR_register(GIE);
	while(1){
		sleep(60000);
		//uart_putc(i);
		P1OUT ^= GREEN_LED;
	}
}


/* Function to execute upon receiving a packet
 *   Called by the driver when new packet arrives */
void MRFI_RxCompleteISR(void) {
	/* Read the received data packet */
	char msg[4];
	char *msg2;

	//Set up decrypting
	int e = 1;
	int n = 23;
	int phi_num = phi(n);
	int d = findD(e,n);
	
	mrfiPacket_t	packet;
	MRFI_Receive(&packet);
	
	/* Send 3 digits as a character array through UART */
	msg2[0] = packet.frame[9];
	msg2[1] = packet.frame[10];
	msg2[2] = packet.frame[11];
	
	//Formal final message that has been decrypted 
	msg2 = decrypt_complex(msg2,d,n);
	msg[0] = msg2[0];
	msg[1] = msg2[1];
	msg[2] = msg2[2];
	msg[3] = '\0';
	uart_puts(msg);
	uart_puts("\n");

	/* Toggle the red LED to signal that data has arrived */
	P1OUT ^= RED_RECEIVE_LED;
}
