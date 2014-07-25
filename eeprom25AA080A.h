/* EEPROM library for 16F886*/
#ifndef eeprom25AA080A_h
#define eeprom25AA080A_h

#include <stdio.h>
#include <stdlib.h>
#include "SPILibrary16F886.h"

//EEPROM command defines
#define AA_READ           0x03
#define AA_WRITE          0x02
#define AA_WRDI           0x04
#define AA_WREN           0x06
#define AA_READ_STATUS    0x05
#define AA_WRITE_STATUS   0x01

//SPI Pin
#define  nHOLD   RC1 //hold pin for EEPROM
#define  nCS     RC2 //cs pin for EEPROM

unsigned char read_aa_status();
void write_aa_status(unsigned char setting);
void enable_aa_write();
void initiate_aa_write(int address);
void write_aa(unsigned int* data, unsigned char length, int address);
void read_aa(unsigned int* data, unsigned char length, int address);

#endif