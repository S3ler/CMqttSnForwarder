//
// Created by bele on 05.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKCLIENTNETWORKRECEIVERINTERFACE_H
#define CMQTTSNFORWARDER_MOCKCLIENTNETWORKRECEIVERINTERFACE_H

#include <cstdint>

class MockClientNetworkReceiverInterface {
 public:

  virtual ~MockClientNetworkReceiverInterface() {};

  virtual void receive_any_message(uint8_t *data, uint16_t length) = 0;

};
#endif //CMQTTSNFORWARDER_MOCKCLIENTNETWORKRECEIVERINTERFACE_H
