/*
  i2c Library for PIC16F886
*/

/* I2C library for 16F886*/
#ifndef I2C16F886_h
#define I2C16F886_h

#include <stdio.h>
#include <stdlib.h>
#include "I2C16F886.h"

void i2cInit();
void i2cPollSSPIF();
void i2cWait();
void i2cStart();
void i2cStop();
void i2cRestart();
void i2cAck();
void i2cNak();
void i2cSend(unsigned char data);
unsigned char i2cReceive();
void i2cReceiveMode(unsigned char state);
unsigned char i2cBF();
void i2cAckDT(unsigned char state);
unsigned char i2cAckStat();
unsigned char i2cSSPOV();
void i2cSetSlewControl(unsigned char state);

#endif