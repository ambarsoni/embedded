/* Encryption: 	Set up functions to encrypt and decrypt using both a caesar cipher
 * 				as well as an RSA encryption */
 
#include "msp430x22x4.h"
#include "encrypt.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

//Simple Caesar Cipher of shifting messages down by 2
char* encrypt_simple(char msg[]){
	int i;
	int len = strlen(msg);
	char* to_return;
	for(i = 0; i < strlen(msg); i++){
		to_return[i] = msg[i] + 2;
	}	
	return to_return;
}


//Simple Caesar Cipher of shifting messages down by 2
char* decrypt_simple(char *msg){
	int i;
	int len = strlen(msg);
	char* to_return;
	for(i = 0; i < strlen(msg); i++){
		to_return[i] = msg[i] - 2;
	}	
	return to_return;
}


//Full RSA Encryption, when knowing the public keys (N,e)
//Input: char c - the single character (in ASCII form, so subtract 48) to encrypt
// 		 int e - a random number 1<e<n such that gcd(n,e) =1. meaing it is relatively prime to n
//		 int n - a number that is p*q, where (p,q) are the private keys of RSA
char encrypt_RSA(char c, int e, int n) {
	
	//Knowing message space, move char to int
	int m;
	int ret;
	int x;
	m= c -48;
	
	//Formula: Enc(c) = m^e mod N
	//This system uses the public key to encrypt, which cannot be broken easily without the private keys
	x = pow(m,e);
	ret = (x%n);
	return (ret+48);
}

//Full RSA Decryption, when knowing the secret keys
//Input: char input - the single character (in ASCII form, so subtract 48) to decrypt
//		 int d - the multiplicative inverse of the (e) value in the public key - described more in detail in the funtion findD()
//		 int n - the second part of the public key, knowing that n = p*q.
char decrypt_RSA(char input, int d, int n){
	
	int c;
	int inter;
	
	//Formula: Dec(c) = c^d mod N
	//This system is difficult to crack mathematically because of the integer factorization is not polynomial time
	c= input-48;
	inter = pow(c,d);
	inter = inter%n;
	return (inter+48);
	
	
}

//in RSA, d = e^(-1) mod N, where e^(-1) is not 1/e, but the multplicative inverse with respect to N
//This is also calculated in a similar fashion as the Euler Totient Calculator below
//The function looks between 1 <k<n, and see if k*e mod N =1, if such, then we return k as the smallest
//multiplicative inverse of e with respect to N
int findD(int e,int n){
	
	int i=1; //Start at 1 because 0 is everyone's multiplicative inverse
	for(;i<n;i++){
		if((i*e)%n == 1){
			return i;
		}
	}
	
	return 0;
}

//Euler Totient Calculator
//the function phi(n) gives the number of integers, less than n, that is relatively prime to n
//e.g. phi(13) is 12, because 13 is prime and thus everything is relatively prime to it
int phi(int a) {
	
	int i=0;
	int k=0;
	int temp;
	
	//rotates through every number less than the input a, to see if the number is
	//relatively prime to a
	//note: for numbers within a certain range (2^16), this method is not too slow
	//for expandable purposes, we would have to use euler's algorithm
	for(;i<a;i++){
		temp = gcd(i,a);
		if(temp ==1){
			k++;
		}	
	}
	
	return k;
}

//GCD Calculator
//Calculates the greatest common denominator of two numbers a and b
int gcd ( int a, int b )
{
  int c;
  while ( a != 0 ) {
     c = a; a = b%a;  b = c;
  }
  return b;
}


//An expansion on top of encrypt_RSA to be useful for string messages instead of characters
//Calls encrypt_RSA repeatedly 
char* encrypt_complex(char msg[],int e, int n){
	
	int i;
	int len = strlen(msg);
	char* to_return;
	for(i = 0; i < strlen(msg); i++){
		to_return[i] = encrypt_RSA(msg[i],e,n);
	}	
	return to_return;
}

//An expansion on top of decrypt_RSA to be useful for string messages instead of characters
//Calls decrypt_RSA repeatedly 
char* decrypt_complex(char msg[],int d, int n){
	int i;
	int len = strlen(msg);
	char* to_return;
	for(i = 0; i < strlen(msg); i++){
		to_return[i] = decrypt_RSA(msg[i],d,n);
	}	
	return to_return;
}