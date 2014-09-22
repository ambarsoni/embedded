/*------------------------------------------------------------------------
 * Delay: 	procedure for making the system sleep for a certain number
 * 			of cycles
 *----------------------------------------------------------------------*/

/* Parameterized "sleep" helper function */
void sleep(unsigned int count) {
	int i;
	for (i = 0; i < 10; i++) {
		while(count > 0) {
			count--;
			__no_operation();
		}
	}
}
