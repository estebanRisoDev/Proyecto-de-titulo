#ifndef DEFINES_HPP
#define DEFINES_HPP
#endif

#include <iostream>

using namespace std;

#define WORD_SIZE 64

#define mask31 0x0000001F

/*#define max(x,y) ((x)>(y)?(x):(y))
#define min(x,y) ((x)<(y)?(x):(y))
*/
/*numero de bits del entero de la maquina*/
#define W 32
/* W-1 */
#define Wminusone 31
/*numero de bits del entero de la maquina*/
#define WW 64
/*bits para hacer la mascara para contar mas rapido*/
#define bitsM 8
/*bytes que hacen una palabra */
#define BW 4
#ifndef uchar
#define uchar unsigned char
#endif
#ifndef uint
#define uint unsigned int
#endif
#ifndef ulong
#define ulong unsigned long
#endif
#define size_uchar 256

/* reads bit p from e */
#define bitget(e,p) ((((e)[(p)/W] >> ((p)%W))) & 1)
/* sets bit p in e */
#define bitset(e,p) ((e)[(p)/W] |= (1<<((p)%W)))
/* cleans bit p in e */
#define bitclean(e,p) ((e)[(p)/W] &= ~(1<<((p)%W)))

/* permite conocer el valor 0 o 1 de la i-ésima posición*/
#define bitaccess(e,p) ((e)[(p)/W] & (1<<((p)%W)))

/* numero de enteros necesarios para representar e elementos de largo n */
#define enteros(e,n) ((e)*(n))/W+(((e)*(n))%W > 0)
/* bits needed to represent a number between 0 and n */

