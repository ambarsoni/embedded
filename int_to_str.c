/*------------------------------------------------------------------------
 * Integer to String Conversion: provides the common C utility for 
 * converting an int to an array of characters (itoa) because printf
 * is not supported on msp430
 *----------------------------------------------------------------------*/
 
#include "msp430x22x4.h"
#include <stdio.h>
#include <string.h>
#include "int_to_str.h"
 
/* itoa:  convert n to characters in s */
void itoa(int n, char s[]){
	int i;
	int sign;
	
    if((sign = n) < 0){  		/* record sign */
        n = -n;					/* make n positive */
    }         			 		
    i = 0;
    
    /* generate digits in reverse order */
    do{       
        s[i++] = n % 10 + '0';  /* get next digit */
    }while((n /= 10) > 0);     /* delete it */
    
    if (sign < 0){
        s[i++] = '-';
    }
    s[i] = '\0';
    reverse(s);
}
 
 
/* reverse:  reverse string s in place */
void reverse(char s[]){
    int i;
    int j;
    char c;
 
    for(i = 0, j = strlen(s)-1; i<j; i++, j--){
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
