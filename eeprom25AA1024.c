#include <xc.h>
#include "eeprom25AA1024.h"

//read status register
unsigned char read_aa_status(){

  nCS = 0;

  unsigned char status;

  writeSPIByte(AA_READ_STATUS);
  status = readSPIByte();

  nCS = 1;

  return status;
}

//direct write to SR
void write_aa_status(unsigned char setting){

  nCS = 0;

  unsigned char status;

  writeSPIByte(AA_WRITE_STATUS);
  writeSPIByte(setting);

  nCS = 1;
  
}

//set the WEL in SR
void enable_aa_write(){
  nCS = 0;

  writeSPIByte(AA_WREN);

  nCS = 1;
}

//clear the WEL is SR
void disable_aa_write(){
  nCS = 0;

  writeSPIByte(AA_WRDI);

  nCS = 1;
}

//initiate write sequence starting at 24 bit address location
void initiate_aa_write(unsigned short long address){

  //need to set WEL is status
  enable_aa_write();

  nCS = 0;

  writeSPIByte(AA_WRITE);
  
  writeSPI24bits(address);
}

//set nCS to start write sequence and poll WEP in SR
void finalise_aa_write(){
  nCS = 1;
  while(read_aa_status() == 0x03){}   //wait for SR WIP to clear
}

bool checkPageEdge(unsigned short long currentAddress){
  if((currentAddress) % 0xFF == 0){ //page edge detected
    finalise_aa_write();
    return true;
  }
  else{
    return false;
  }
}

//write data to write buffer, initiate write sequence at page edges, repeat untill all data written
//this is a 16 bit write
void write_aa(unsigned int* data, unsigned char length, unsigned short long address){

  initiate_aa_write(address);

  //keep track of page edges and memory location as 16 bit writes are two memory locations on EEPROM but only one index change in data array
  unsigned short long page_address, memory_location = 0;

  for(int data_location = 0; data_location < length; data_location++){

    int dataToWrite = data[data_location];

    unsigned char b1, b2;
    //write first half of data
    b1 = writeSPIByte(dataToWrite >> 8);
    //check page edge
    if(checkPageEdge(address+memory_location)){
      initiate_aa_write(address+memory_location+1);    //restart write
    }
    //increase location
    memory_location++;

    //write 2nd half
    b2 = writeSPIByte(dataToWrite);
    //check page edge
    if(checkPageEdge(address+memory_location)){
      initiate_aa_write(address+memory_location+1);    //restart write
    }
    //increase location
    memory_location++;

    
  }

  finalise_aa_write();

}
//read from memory in 16 bit reads
void read_aa(unsigned int* data, unsigned char length, unsigned short long address){

  nCS = 0;

  writeSPIByte(AA_READ);
  writeSPI24bits(address);

  //count in data for length of array - as reads are 16 bit don't need to worry about double incrementing in the memory
  for(int i = 0; i < length; i++){
    data[i] = readSPIWord();
  }

  nCS = 1;

}