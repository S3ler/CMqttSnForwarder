//
// Created by SomeDude on 12.05.2019.
//

#ifndef CMQTTSNFORWARDER_UTILITY_LINUX_EEPROM_FAKE_EEPROM_H_
#define CMQTTSNFORWARDER_UTILITY_LINUX_EEPROM_FAKE_EEPROM_H_

//#include <vector>
#include <stdint.h>
#include <stdio.h>

class EEPROMClass {
 private:
  //std::vector<uint8_t> _data;
  uint8_t _data;
 public:
  EEPROMClass(void);

  void begin(size_t size);
  bool commit();
  void end();

  uint8_t *getDataPtr();
  uint8_t const *getConstDataPtr() const;
};

extern EEPROMClass EEPROM;

#endif //CMQTTSNFORWARDER_UTILITY_LINUX_EEPROM_FAKE_EEPROM_H_
