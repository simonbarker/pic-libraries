/*

Software SPI library functions

*/
#ifndef softwareSPI_h
#define softwareSPI_h

#include <stdio.h>
#include <stdlib.h>

#define _XTAL_FREQ 8000000

/*#define SCK   RC3
#define MOSI  RC5
#define MISO  RC4*/

#define SCK   RA0
#define MOSI  RA1
#define MISO  RA2

unsigned char writeSPIByte(unsigned char transmit);
int writeSPIWord(unsigned short int setting);
int writeSPI24bits(unsigned short long setting);
int readSPIWord();
unsigned char readSPIByte();

#endif