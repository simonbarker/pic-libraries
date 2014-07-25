#include <xc.h>
#include "SPILibrary16F886.h"
void SPIOn(){
  SSPEN = 1;
  SSPIE = 1;
}

int initSPI(int mode, int clockDivide, char masterSlave){
  //set up SPI master or slave
  if(masterSlave == 'm'){
    if(clockDivide == 4)
      SSPCON = SSPCON & 0b11110000; //master, clock/4
    else if(clockDivide == 16){
      SSPCON = SSPCON & 0b11110000; //master clock/16
      SSPCON = SSPCON | 0b00000001;
    }
    else if(clockDivide == 64){
      SSPCON = SSPCON & 0b11110000; //master clock/64
      SSPCON = SSPCON | 0b00000010;
    }
    else{
      return 1; //error code 1
    } 
    
  }
  if(masterSlave == 's'){
    SSPCON = SSPCON & 0b11110000; //clear the lsb
    SSPCON = SSPCON | 0b00000100; //nSS enabled in slave mode
    SMP = 0;
  }
  //set up SPI mode
  if(mode == 1){
    CKE = 1;
    CKP = 0;  
  }
  else if(mode == 2){
    CKE = 0;
    CKP = 0;
  }
  else if(mode == 3){
    CKE = 1;
    CKP = 1;
  }
  else if(mode == 4){
    CKE = 0;
    CKP = 1;
  }
  else
    return 2;   //error code 2
  
  //set up interrupts
  SSPIE = 1;
  TRISC3 = 0; // 0 = output for sck
  return 0;
} 

unsigned char writeSPIByte(unsigned char data)
{
  SSPSTATbits.BF = 0;
  unsigned char i;
  SSPBUF = data;
  while(SSPSTATbits.BF == 0){}
  return SSPBUF;
} 

unsigned char readSPIByte(){
  unsigned char data;
  data = writeSPIByte(0x00);
  return data;
} 

int writeSPIWord(unsigned short int setting)
{
  int data;
  unsigned char b1, b2;
  b1 = writeSPIByte(setting >> 8);
  b2 = writeSPIByte(setting);
  data = b1 << 8 | b2;
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



void SPIOff(){
  SSPEN = 0;
}