/*-------------------------------------------------------------------------------------------
 * 	 Transmit Applicaiton: 	Broadcasts packet of information to other radios with temperature
 * 							data encoded in 3 digit format
 *   					
 *   Toggles red LED every time a packet is sent.
 *-----------------------------------------------------------------------------------------*/

#include "bsp.h"
#include "mrfi.h"
#include "radios/family1/mrfi_spi.h"
#include <string.h>
#include "delay.h"
#include <stdio.h>
#include "int_to_str.h"
#include "encrypt.h"

/* Useful #defines */
#define RED_SEND_LED 		0x01

/* Function prototypes */
int read_temp();
void transmit(char msg[]);
void format_msg(int temp, char msg[], char msg2[]);


/* Main function for transmit application */
void main(void) {
	int temp;
	char msg[5]; 	//mrfiPacket_t 	packet;
	char msg2[3];
	
	int e = 1;
	int n = 23;
	
	//Disable Watchdog Timer
	WDTCTL = WDTPW + WDTHOLD;
	
	//Red LED P1.1
	P1DIR |= 0x01;
	P1OUT |= 0x00;

	P1DIR &= ~0x02;	//P1.1 is Input
	P2DIR &= ~0x0C; //P2.2 & P2.3 are Inputs
	
	/* Perform board-specific initialization */
	BSP_Init();
	
	/* Initialize minimal RF interface, wake up radio */
	MRFI_Init();
	MRFI_WakeUp();
	
	/* Set red LED to output */
	P1DIR = RED_SEND_LED;
	P1OUT = RED_SEND_LED;
		
	//Global Interrupt Enable
	__bis_SR_register(GIE);
	
	/* Main (infinite) transmit loop */
	while(1){
		temp = read_temp(); 			//Read ADCs for temperature
		itoa(temp, msg2);				//Convert into to array of characters
		format_msg(temp, msg, msg2);	//Format message into 3 digits XXX
		transmit(encrypt_complex(msg,e,n));					//Create and send packet with header
	}
}

/* Construct a packet and sends over the radio.
 *  Packet frame structure:
 *  ---------------------------------------------------
 *  | Length (1B) | Dest (4B) | Source (4B) | Payload |
 *  ---------------------------------------------------
 */
void transmit(char msg[]){
	mrfiPacket_t 	packet; 
	/* First byte of packet frame holds message length in bytes */
		packet.frame[0] = strlen(msg) + 8;	/* Includes 8-byte address header */
		
		/* Next 8 bytes are addresses, 4 each for source and dest. */
		packet.frame[1] = 0x12;		/* Destination */
		packet.frame[2] = 0x34;
		packet.frame[3] = 0xab;
		packet.frame[4] = 0xcd;
		
		packet.frame[5] = 0x02;		/* Source */
		packet.frame[6] = 0x00;
		packet.frame[7] = 0x01;
		packet.frame[8] = 0x02;
		
		/* Remaining bytes are the message/data payload */
		strcpy( (char *) &packet.frame[9] , msg );
		
		/* Transmit the packet over the radio */
		MRFI_Transmit(&packet , MRFI_TX_TYPE_FORCED);
		
		/* Toggle red LED after transmitting, then wait a while */
		P1OUT ^= RED_SEND_LED;
		sleep(60000);
}

/* Sets the ADCs to approriate values to measure temperature,
 * converts it to a value in degrees celcius.
 * Returns: interger that is current temperature in C */ 
int read_temp()
{
	int t;
	ADC10CTL0=SREF_1 + REFON + ADC10ON + ADC10SHT_3 ; 	//1.5V ref,Ref on,64 clocks for sample
    ADC10CTL1=INCH_10+ ADC10DIV_3; 						//temp sensor is at 10 and clock/4
    t=0;
    __delay_cycles(1000);              					//wait 4 ref to settle
    ADC10CTL0 |= ENC + ADC10SC;      					//enable conversion and start conversion
    while(ADC10CTL1 & BUSY);         					//wait..i am converting..pum..pum..
    t=ADC10MEM;                       					//store val in t
    ADC10CTL0&=~ENC;                     				//disable adc conv
    return(int) ((t * 27069L - 18169625L) >> 16); 		//convert and pass
}

/* Formats final message before transmission since converting int
 * puts a number without any zero paddings
 * 
 * Input: current temp, final msg, msg2 from into_to_str conversion */			
void format_msg(int temp, char msg[], char msg2[]){
	//If temp in range [100, 999]
	if(temp >= 100){
		msg[0] = msg2[0];
		msg[1] = msg2[1];
		msg[2] = msg2[2];	
	}	
	//If temp in range [10,99]
	else if(temp >= 10){
		msg[0] = '0';
		msg[1] = msg2[0];
		msg[2] = msg2[1];
	}
	//If temp in range [0,9]
	else{
		msg[0] = '0';
		msg[1] = '0';
		msg[2] = msg2[0];
	}						
	msg[3] = '\r'-2;
	msg[4] = '\n'-2;	
}

/* Function to execute upon receiving a packet
 *   Called by the driver when new packet arrives */
void MRFI_RxCompleteISR(void) {
/*   Since this is a transmit-only application we do nothing.
 *   This function still needs to be defined for the project
 *   to compile properly. */
}
