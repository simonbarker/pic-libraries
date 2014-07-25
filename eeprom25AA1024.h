/* EEPROM library for 16F886*/
#ifndef eeprom25AA1024_h
#define eeprom25AA1024_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "softwareSPI.h"


//EEPROM command defines
#define AA_READ							0b00000011		//Read data from memory array beginning at selected address
#define AA_WRITE						0b00000010		//Write data to memory array beginning at selected address
#define AA_WREN							0b00000110		//Set the write enable latch (enable write operations)
#define AA_WRDI							0b00000100		//Reset the write enable latch (disable write operations)
#define AA_READ_STATUS			0b00000101		//Read STATUS register
#define AA_WRITE_STATUS			0b00000001		//Write STATUS register 
#define AA_PE								0b01000010		//Page Erase – erase one page in memory array
#define AA_SE								0b11011000		//Sector Erase – erase one sector in memory array
#define AA_CE								0b11000111		//Chip Erase – erase all sectors in memory array
#define AA_RDID							0b10101011		//Release from Deep power-down and read electronic signature
#define AA_DPD							0b10111001		//
//SPI Pin
#define  nHOLD   RC1 //hold pin for EEPROM
#define  nCS     RA3 //RC2 //cs pin for EEPROM

unsigned char read_aa_status();
void write_aa_status(unsigned char setting);
void enable_aa_write();
bool checkPageEdge(unsigned short long currentAddress);
void initiate_aa_write(unsigned short long address);
void write_aa(unsigned int* data, unsigned char length, unsigned short long address);
void read_aa(unsigned int* data, unsigned char length, unsigned short long address);

#endif