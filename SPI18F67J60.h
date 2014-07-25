/* SPI library for 16F886*/
#ifndef SPI18F67J60_h
#define SPI18F67J60_h

#include <stdio.h>
#include <stdlib.h>
#include "SPI18F67J60.h"

//turn on SPI module (really the MSSP)
void SPIOn();
//init SPI with the mode choice (1 -4), clock division (4,16,64 - not used for slave), master mode or slave mode ('m'.'s')
int initSPI(int mode, int clockDivide, char masterSlave);
//write an SPI unsigned char - include GenericTypeDeffs.h
unsigned char writeSPIByte (unsigned char data);
//Read an SPI unsigned char - include GenericTypeDeffs.h
unsigned char readSPIByte (void);
//write an SPI WORD - include GenericTypeDeffs.h
void writeSPIWord(unsigned short int setting);
//write an SPI WORD - include GenericTypeDeffs.h
void readSPIWord();
//turn off SPI module
void SPIOff();

#endif