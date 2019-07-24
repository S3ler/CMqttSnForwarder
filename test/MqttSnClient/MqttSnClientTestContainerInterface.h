//
// Created by SomeDude on 24.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINERINTERFACE_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINERINTERFACE_H_

#include <stdint.h>

class MqttSnClientTestContainerInterface {
 protected:
  uint64_t action_number = 0;

  //MqttSnClientTestContainerReceiverInterface *receiver_interface = nullptr;
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTTESTCONTAINERINTERFACE_H_
