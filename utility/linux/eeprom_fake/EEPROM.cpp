//
// Created by SomeDude on 12.05.2019.
//

#include "EEPROM.h"
#include <string.h>

EEPROMClass::EEPROMClass(void) {
  // std::fill(_data.begin(), _data.end(), 0);
  memset(&_data, 0, sizeof(_data));
}
void EEPROMClass::begin(size_t size) {
  //_data.resize(size);
  //std::fill(_data.begin(), _data.end(), 0);
}
bool EEPROMClass::commit() {
  return true;
}
void EEPROMClass::end() {

}
uint8_t *EEPROMClass::getDataPtr() {
  return &_data;
  //return _data.data();
}
uint8_t const *EEPROMClass::getConstDataPtr() const {
  return &_data;
  //return _data.data();
}

EEPROMClass EEPROM;

