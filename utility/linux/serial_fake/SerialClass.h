//
// Created by SomeDude on 12.05.2019.
//

#ifndef CMQTTSNFORWARDER_UTILITY_LINUX_SERIAL_FAKE_SERIALCLASS_H_
#define CMQTTSNFORWARDER_UTILITY_LINUX_SERIAL_FAKE_SERIALCLASS_H_

class SerialClass {
 public:
  int available();
  char read();
};
extern SerialClass Serial;
#endif //CMQTTSNFORWARDER_UTILITY_LINUX_SERIAL_FAKE_SERIALCLASS_H_
