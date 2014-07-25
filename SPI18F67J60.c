#include <xc.h>
#include "SPI18F67J60.h"

void SPIOn(){
  SSP1CON1bits.SSPEN = 1;
  SSPIE = 1;
}

int initSPI(int mode, int clockDivide, char masterSlave){
  //set up SPI master or slave
  if(masterSlave == 'm'){
    if(clockDivide == 4)
      SSP1CON1 = SSP1CON1 & 0b11110000; //master, clock/4
    else if(clockDivide == 16){
      SSP1CON1 = SSP1CON1 & 0b11110000; //master clock/16
      SSP1CON1 = SSP1CON1 | 0b00000001;
    }
    else if(clockDivide == 64){
      SSP1CON1 = SSP1CON1 & 0b11110000; //master clock/64
      SSP1CON1 = SSP1CON1 | 0b00000010;
    }
    else{
      return 1; //error code 1
    }

  }
  if(masterSlave == 's'){
    SSP1CON1 = SSP1CON1 & 0b11110000; //clear the lsb
    SSP1CON1 = SSP1CON1 | 0b00000100; //nSS enabled in slave mode
    SMP = 0;
  }
  //set up SPI mode
  if(mode == 1){
    SSP1STATbits.CKE = 1;
    SSP1CON1bits.CKP = 0;
  }
  else if(mode == 2){
    SSP1STATbits.CKE = 0;
    SSP1CON1bits.CKP = 0;
  }
  else if(mode == 3){
    SSP1STATbits.CKE = 1;
    SSP1CON1bits.CKP = 1;
  }
  else if(mode == 4){
    SSP1STATbits.CKE = 0;
    SSP1CON1bits.CKP = 1;
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
  SSP1STATbits.BF = 0;
  unsigned char i;
  SSPBUF = data;
  while(SSP1STATbits.BF == 0){}
  return SSPBUF;
}
unsigned char readSPIByte(void){
  writeSPIByte(0x00);
  return SSPBUF;
}

void writeSPIWord(unsigned short int setting)
{
    writeSPIByte(setting >> 8);
    writeSPIByte(setting);
}
void readSPIWord(){
  readSPIByte();
  readSPIByte();
}
void SPIOff(){
  SSP1CON1bits.SSPEN = 0;
}