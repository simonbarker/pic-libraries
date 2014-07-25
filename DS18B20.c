#include "DS18B20.h"
#include "onewire.h"
/*
  send message to every sensor on the bus to take a reading
*/
void broadcastConvert() {
  //broadcast that temp conversions should begin, all at once so saves time
  onewireInit();
  onewireWriteByte(0xCC);
  onewireWriteByte(0x44);

  while (1) {
    if (onewireReadBit())
      break;
  }
}

/*
  retrieve temperatures from sensors
*/
float getTemperature(unsigned char* address) {
  //get temperature from the device with address address
  float temperature;
  unsigned char scratchPad[9] = {0,0,0,0,0,0,0,0,0};

  onewireInit();
  onewireWriteByte(0x55);
  unsigned char i;
  for (i = 0; i < 8; i++)
    onewireWriteByte(address[i]);
  onewireWriteByte(0xBE);

  for (i = 0; i < 2; i++) {
    scratchPad[i] = onewireReadByte();
  }
  onewireInit();
  temperature = ((scratchPad[1] * 256) + scratchPad[0])*0.0625;

  return temperature;
}

int getTemperatureInt(unsigned char* address) {
  //get temperature from the device with address address
  int temperature;
  unsigned char scratchPad[9] = {0,0,0,0,0,0,0,0,0};

  onewireInit();
  onewireWriteByte(0x55);
  unsigned char i;
  for (i = 0; i < 8; i++)
    onewireWriteByte(address[i]);
  onewireWriteByte(0xBE);

  for (i = 0; i < 2; i++) {
    scratchPad[i] = onewireReadByte();
  }
  onewireInit();
  temperature = ((scratchPad[1] * 256) + scratchPad[0]);

  return temperature;
}

/*
  retrieve address of sensor and print to terminal
*/
void printSingleAddress() {
  onewireInit();
  //attach one sensor to port 25 and this will print out it's address
  unsigned char address[8]= {0,0,0,0,0,0,0,0};
  onewireWriteByte(0x33);
  unsigned char i;
  for (i = 0; i<8; i++)
    address[i] = onewireReadByte();
  for (i = 0; i<8; i++)
    printf("0x%x,",address[i]);
  
  //check crc
  unsigned char crc = onewireCRC(address, 7);
  printf("crc = %x \r\n",crc);
}