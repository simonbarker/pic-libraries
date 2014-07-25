/*
  i2c Library
*/

#include <xc.h>
#include "I2C16F886.h"

void i2cInit(){
  SSPCONbits.SSPM = 0b1000; //set to harware master mode
  SSPCONbits.SSPEN = 1;
  SSPSTATbits.CKE = 0;
  SSPSTATbits.SMP = 1;

  SSPADD = 19;  //baud = 100Khz when Fosc = 8MHz

}

void i2cPollSSPIF(){
  while(!PIR1bits.SSPIF);
  PIR1bits.SSPIF = 0;
}

void i2cWait(){
  while ((SSPCON2 & 0x1F ) || ( SSPSTAT & 0x04 ) );
}

void i2cStart(){
  SSPCON2bits.SEN = 1;
  while(SSPCON2bits.SEN);
}

void i2cStop(){
  SSPCON2bits.PEN = 1;
  while(PEN);
}

void i2cRestart(){
  SSPCON2bits.RSEN = 1;
  while(RSEN);
}

void i2cAck(){
  ACKDT = 0; /* Acknowledge data bit, 0 = ACK */
  ACKEN = 1; /* Ack data enabled */
  while(ACKEN); /* wait for ack data to send on bus */
}


void i2cNak(){
  ACKDT = 1; /* Acknowledge data bit, 1 = NAK */
  ACKEN = 1; /* Ack data enabled */
  while(ACKEN); /* wait for ack data to send on bus */
}

void i2cSend(unsigned char data){
  SSPBUF = data;
  while(BF);
  i2cWait();
}

unsigned char i2cReceive(){

  unsigned char temp;
  /* Reception works if transfer is initiated in read mode */
  RCEN = 1; /* Enable data reception */
  while(!BF); /* wait for buffer full */
  temp = SSPBUF; /* Read serial buffer and store in temp register */
  i2cWait(); /* wait to check any pending transfer */
  return temp; /* Return the read data from bus */
}

void i2cReceiveMode(unsigned char state){ //0 disables receive mode, 1 enables it
  SSPCON2bits.RCEN = state;
}

unsigned char i2cBF(){
  return SSPSTATbits.BF;
}

void i2cAckDT(unsigned char state){ //0 send an ACK, 1 send a NACK
  SSPCON2bits.ACKSTAT = state;
}

unsigned char i2cAckStat(){
  return SSPCON2bits.ACKSTAT;
}

unsigned char i2cSSPOV(){
  return SSPCONbits.SSPOV;
}

void i2cSetSlewControl(unsigned char state){ //set to 0 if in 400Kbps mode, 1 otherwise
  SSPSTATbits.SMP = state;
}