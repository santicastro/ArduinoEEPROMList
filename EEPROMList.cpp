#include "EEPROMList.h"
#include <stdlib.h>

#define DEFAULT_MAX_ADDRESS 1024 //this should depend on wich microcontroller is been used
#define DEFAULT_START_ADDRESS 0
#define getAddress(position) dataBaseAddress+position*listInfo.elementSize

EEPROMList::EEPROMList(uint8_t elementSize, uint16_t startAddress, uint16_t maxMemorySize)
{
  init(elementSize, startAddress, maxMemorySize);
}

EEPROMList::EEPROMList(uint8_t elementSize, uint16_t startAddress)
{
  init(elementSize, startAddress, DEFAULT_MAX_ADDRESS);
}

EEPROMList::EEPROMList(uint8_t elementSize)
{
  init(elementSize, DEFAULT_START_ADDRESS, DEFAULT_MAX_ADDRESS);
}

void EEPROMList::init(uint8_t elementSize, uint16_t startAddress, uint16_t maxMemorySize){
  infoAddress = startAddress;
  maxMemoryAddress = infoAddress + maxMemorySize; //TODO: check if it's a valid value
  loadListInfo();
  dataBaseAddress = infoAddress + sizeof(listInfo);
  buffer = (uint8_t*) malloc(sizeof(uint8_t*) * listInfo.elementSize);

  //TODO: this code removes the list if the hashcode is invalid, it should try to restore.
  if((listInfo.elementSize!=elementSize) || !checkIntegrity()){
    listInfo.elementSize = elementSize;
    listInfo.size = 0;
    listInfo.hashCode = 0;
    saveListInfo();
  }
}

void EEPROMList::saveListInfo(){
  uint8_t* structAddress = (uint8_t* )&listInfo;
  for(int i=0; i<sizeof(listInfoType); i++){
    EEPROM.write(infoAddress + i, structAddress[i]);
  }
}

void EEPROMList::loadListInfo(){
  uint8_t* structAddress = (uint8_t* )&listInfo;
  for(int i=0; i<sizeof(listInfoType); i++){
    structAddress[i] = EEPROM.read(infoAddress + i);
  }	
}

void EEPROMList::clear(){
  listInfo.size = 0;
  listInfo.hashCode = 0;
  saveListInfo();
}

int EEPROMList::size()
{
  return listInfo.size;
}

uint8_t* EEPROMList::get(uint16_t position){
  if(position >= listInfo.size){
    return 0;
  }
  else{
    int startPosition = getAddress(position);
    for(uint8_t i=0; i<listInfo.elementSize; i++){
      buffer[i] = EEPROM.read(startPosition + i);
    }
  }
  return buffer;
}

bool EEPROMList::add(uint8_t* element){
  int startPosition = getAddress(size());
  if((startPosition + listInfo.elementSize) < maxMemoryAddress){
    for(uint8_t i=0; i<listInfo.elementSize; i++){
      EEPROM.write(startPosition + i, element[i]);
    }
    listInfo.size++;
    listInfo.hashCode = calculateHashCode();
    saveListInfo();
    return true;
  }
  else{
    return false;
  }
}

bool EEPROMList::exists(uint8_t* element){
  uint8_t* current;
  for(uint16_t i=0; i<size(); i++){
    current = get(i);
    return compare(current, element)==0;
  }
}

int8_t EEPROMList::compare(uint8_t* element1, uint8_t* element2){
  for(uint8_t i=0; i<listInfo.elementSize; i++){
    if(element1[i]<element2[i]){
      return -1;
    }
    else if(element1[i]>element2[i]){
      return 1;
    }
  }
  return 0;
}

listInfoType EEPROMList::getInfo(){
  return listInfo;	
}

bool EEPROMList::checkIntegrity(){
  uint16_t calculatedHashCode = calculateHashCode();
  return calculatedHashCode == listInfo.hashCode;
}

// Calculates a hashcode of the elements array
uint16_t EEPROMList::calculateHashCode(){
  uint16_t hash=0, count;
  count = size();
  for(uint16_t i=getAddress(0); i < getAddress(count); i++){
    hash  = hash * 31 + EEPROM.read(i);
  }
  return hash;
}


