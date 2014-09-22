/* Encryption: 	Set up functions to encrypt and decrypt using both a caesar cipher
 * 				as well as an RSA encryption */

#ifndef ENCRYPT_H_
#define ENCRYPT_H_

//Simple Caesar Cipher of shifting messages down by 2
char* encrypt_simple(char msg[]);

//Simple Caesar Cipher of shifting messages down by 2
char* decrypt_simple(char *msg);

//GCD Calculator
//Calculates the greatest common denominator of two numbers a and b
int gcd(int a,int b);

//Euler Totient Calculator
//the function phi(n) gives the number of integers, less than n, that is relatively prime to n
//e.g. phi(13) is 12, because 13 is prime and thus everything is relatively prime to it
int phi(int a); 

//Full RSA Encryption, when knowing the public keys (N,e)
//Input: char c - the single character (in ASCII form, so subtract 48) to encrypt
// 		 int e - a random number 1<e<n such that gcd(n,e) =1. meaing it is relatively prime to n
//		 int n - a number that is p*q, where (p,q) are the private keys of RSA
char encrypt_RSA(char c, int e, int n);

//Full RSA Decryption, when knowing the secret keys
//Input: char input - the single character (in ASCII form, so subtract 48) to decrypt
//		 int d - the multiplicative inverse of the (e) value in the public key - described more in detail in the funtion findD()
//		 int n - the second part of the public key, knowing that n = p*q.
char decrypt_RSA(char input, int d, int n);

//An expansion on top of encrypt_RSA to be useful for string messages instead of characters
//Calls encrypt_RSA repeatedly 
char* encrypt_complex(char msg[],int e, int n);

//An expansion on top of decrypt_RSA to be useful for string messages instead of characters
//Calls decrypt_RSA repeatedly 
char* decrypt_complex(char msg[],int d, int n);

//in RSA, d = e^(-1) mod N, where e^(-1) is not 1/e, but the multplicative inverse with respect to N
//This is also calculated in a similar fashion as the Euler Totient Calculator below
//The function looks between 1 <k<n, and see if k*e mod N =1, if such, then we return k as the smallest
//multiplicative inverse of e with respect to N
int findD(int e,int n);


#endif /*ENCRYPT_H_*/
