#include <xc.h>
#include "onewire.h"

void onewireWriteBit(int b) {
  b = b & 0x01;
  if (b) {
    // Write '1' bit
    onewirePinDirection = 0;
    onewirePin = 0;
    __delay_us(5);
    onewirePinDirection = 1;
    __delay_us(60);
  } else {
    // Write '0' bit
    onewirePinDirection = 0;
    onewirePin = 0;
    __delay_us(70);
    onewirePinDirection = 1;
    __delay_us(2);
  }
}

unsigned char onewireReadBit() {
  unsigned char result;

  onewirePinDirection = 0;
  onewirePin = 0;
  __delay_us(1);
  onewirePinDirection = 1;
  __delay_us(5);
  result = onewirePin;
  __delay_us(55);
  return result;

}

unsigned char onewireInit() {
  onewirePinDirection = 0;
  onewirePin = 0;
  __delay_us(480);
  onewirePinDirection = 1;
  __delay_us(60);
  if (onewirePin == 0) {
      __delay_us(100);
      return 1;
  }
  return 0;
}

unsigned char onewireReadByte() {
  unsigned char result = 0;

  for (unsigned char loop = 0; loop < 8; loop++) {
    // shift the result to get it ready for the next bit
    result >>= 1;

    // if result is one, then set MS bit
    if (onewireReadBit())
      result |= 0x80;
  }
  return result;
}

void onewireWriteByte(char data) {
  // Loop to write each bit in the byte, LS-bit first
  for (unsigned char loop = 0; loop < 8; loop++) {
    onewireWriteBit(data & 0x01);

    // shift the data byte for the next bit
    data >>= 1;
  }
}

unsigned char onewireCRC(unsigned char* addr, unsigned char len) {
  unsigned char i, j;
  unsigned char crc = 0;

  for (i = 0; i < len; i++) {
    unsigned char inbyte = addr[i];
    for (j = 0; j < 8; j++) {
      unsigned char mix = (crc ^ inbyte) & 0x01;
      crc >>= 1;
      if (mix) crc ^= 0x8C;
      inbyte >>= 1;
    }
  }

  return crc;
}