#include <xc.h>
#include "eeprom25AA080A.h"

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

//initiate write sequence starting at 16 bit address location
void initiate_aa_write(int address){

  //need to set WEL is status
  enable_aa_write();

  nCS = 0;

  writeSPIByte(AA_WRITE);
  
  writeSPIWord(address);
}

//set nCS to start write sequence and poll WEP in SR
void finalise_aa_write(){
  nCS = 1;
  while(read_aa_status() == 0x03){}   //wait for SR WIP to clear
}

//write data to write buffer, initiate write sequence at page edges, repeat untill all data written
//this is a 16 bit write
void write_aa(unsigned int* data, unsigned char length, int address){

  initiate_aa_write(address);

  //keep track of page edges and memory location as 16 bit writes are two memory locations on EEPROM but only one index change in data array
  int page_address, memory_location = 0;

  for(int data_location = 0; data_location < length; data_location++){
    writeSPIWord(data[data_location]);
    memory_location = memory_location+2;  //increment by two as it's two locations in the EEPROM
    //check for page end
    if((address+memory_location) % 8 == 0){ //pages are on multiples of 16 bytes - as writing 2 bytes at a time need to mod on 8 not 16
      finalise_aa_write();
      page_address = address+memory_location; //work out where we are in memory to initialise the new write
      initiate_aa_write(page_address);    //restart write
    }
  }

  finalise_aa_write();

}
//read from memory in 16 bit reads
void read_aa(unsigned int* data, unsigned char length, int address){

  nCS = 0;

  writeSPIByte(AA_READ);
  writeSPIWord(address);

  //count in data for length of array - as reads are 16 bit don't need to worry about double incrementing in the memory
  for(int i = 0; i < length; i++){
    data[i] = readSPIWord();
  }

  nCS = 1;

}