#include <EEPROM.h>
#include <EEPROMList.h>
#define ELEMENT_SIZE 6

// WARNING: This test will overwrite de eeprom from address 1000
EEPROMList list(ELEMENT_SIZE, 1000);

void setup(){
  Serial.begin(9600);
  delay(1000);

  uint8_t it[6] = {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xab          };
  if(!list.add(it)){
    Serial.println("Item was not added");
  }
  uint8_t it2[6] = {
    0x67, 0x89, 0xab,0x01, 0x23, 0x45           };
  if(!list.add(it2)){
    Serial.println("Item was not added");
  }

  Serial.print("checkIntegrity: ");
  Serial.println(list.checkIntegrity());


  printHexList();
}

void loop(){

}

void printHexList(){
  Serial.println("Printing list contents");
  Serial.print("List size: ");
  Serial.println(list.size());

  uint8_t* current;
  for(uint16_t i=0; i<list.size(); i++){
    Serial.print("Element ");
    Serial.print(i);
    Serial.print(": ");
    current = list.get(i);
    for(uint8_t i=0; i<ELEMENT_SIZE; i++){
      Serial.print(current[i], HEX);
      Serial.print(" ");
    }
    Serial.println(); 
  }

}
