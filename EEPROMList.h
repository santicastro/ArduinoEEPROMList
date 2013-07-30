#ifndef EEPROMList_h
#define EEPROMList_h

#include <EEPROM.h>
#include <inttypes.h>
typedef	struct {
  uint8_t elementSize;
  uint16_t size;
  uint16_t hashCode;		
} 
listInfoType;

class EEPROMList {
public:
  // constructors:
  EEPROMList(uint8_t elementSize, uint16_t startAddress, uint16_t maxMemorySize);
  EEPROMList(uint8_t elementSize, uint16_t startAddress);
  EEPROMList(uint8_t elementSize);

  //our interface methods
  void clear();
  int size();
  uint8_t* get(uint16_t position);
  bool add(uint8_t* element);
  bool exists(uint8_t* element);
  uint16_t getPosition(uint8_t* element);

  listInfoType getInfo();

  bool checkIntegrity();	
  uint16_t calculateHashCode();

private:
  void init(uint8_t elementSize, uint16_t startAddress, uint16_t maxMemorySize);
  int8_t compare(uint8_t* element1, uint8_t* element2);
  void saveListInfo();
  void loadListInfo();
  uint16_t infoAddress;
  uint16_t dataBaseAddress;
  uint16_t maxMemoryAddress;
  uint8_t* buffer;
  listInfoType listInfo;
};

#endif

