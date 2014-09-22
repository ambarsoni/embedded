/*------------------------------------------------------------------------
 * Integer to String Conversion: provides the common C utility for 
 * converting an int to an array of characters because printf
 * is not supported on msp430
 *----------------------------------------------------------------------*/

#ifndef INT_TO_STR_H_
#define INT_TO_STR_H_

/* itoa:  convert n to characters in s */
void itoa(int n, char s[]);

/* reverse:  reverse string s in place */
void reverse(char s[]);

#endif /*INT_TO_STR_H_*/
