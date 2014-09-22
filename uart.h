/*------------------------------------------------------------------------
 * UART Communication: Defines functions for initializng the serial
 * 						port to send strings through UART
 *----------------------------------------------------------------------*/

#ifndef UART_H_
#define UART_H_

/* Initialize the UART for TX (9600, 8N1) */
/* Settings taken from TI UART demo */ 
void init_uart(void);

/* Transmit a single character over UART interface */
void uart_putc(char c);

/* Transmit a nul-terminated string over UART interface */
void uart_puts(char *str);

/* Clear terminal screen using VT100 commands */
/* http://braun-home.net/michael/info/misc/VT100_commands.htm */
void uart_clear_screen(void);


#endif /*UART_H_*/
