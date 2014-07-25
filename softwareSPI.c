/*

Software SPI library functions - currently only works in mode 1 used for 25AA** EEPROM

*/
#include <xc.h>
#include "softwareSPI.h"

unsigned char writeSPIByte(unsigned char transmit){
  unsigned char received = 0;
  unsigned char bits = 8;
  unsigned char tmpbit = 0;

  SCK = 0;

  //make the transmission
  unsigned char mask = 0x80;                //Initialize to write and read bit 7
  unsigned char ret = 0;                    //Initialize read byte with 0
  
  do{
    //Clock out current bit onto SPI Out line
    if (transmit & mask)
      MOSI = 1;
    else
      MOSI = 0;
    SCK = 1;            //Set SPI Clock line
    if (MISO)
      ret |= mask;      //Read current bit fromSPI In line
    __delay_us(1);      //Ensure minimum delay of 500nS between SPI Clock high and SPI Clock Low
    SCK = 0;            //Set SPI Clock line
    mask = mask >> 1;   //Shift mask so that next bit is written and read from SPI lines
    __delay_us(1);      //Ensure minimum delay of 1000ns between bits
  }while (mask != 0);
  
  return ret;
}

int writeSPIWord(unsigned short int setting)
{
  int data;
  unsigned char b1, b2;
  b1 = writeSPIByte(setting >> 8);
  b2 = writeSPIByte(setting);
  data = b1 << 8 | b2;
  __delay_us(50);
  return data;
}

int writeSPI24bits(unsigned short long setting)
{
  unsigned short long data;
  unsigned char b1, b2, b3;
  b1 = writeSPIByte(setting >> (unsigned short long)16);
  b2 = writeSPIByte(setting >> 8);
  b3 = writeSPIByte(setting);
  data = (unsigned short long)b1 << (unsigned short long)16 | b2 << 8 | b3;
  __delay_us(50);
  return data;
}

int readSPIWord(){
  int data;
  unsigned char b1, b2;
  b1 = writeSPIByte(0x00);
  b2 = writeSPIByte(0x00);
  data = b1 << 8 | b2;
  return data;  
}
unsigned char readSPIByte(){
  unsigned char data;
  data = writeSPIByte(0x00);
  return data;
} 