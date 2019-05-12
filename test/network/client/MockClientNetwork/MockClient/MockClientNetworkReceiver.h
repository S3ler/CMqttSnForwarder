//
// Created by bele on 06.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKCLIENTNETWORKRECEIVER_H
#define CMQTTSNFORWARDER_MOCKCLIENTNETWORKRECEIVER_H

#include "MockClientNetworkReceiverInterface.h"
#include "gmock/gmock-function-mocker.h"

class MockClientNetworkReceiver : public MockClientNetworkReceiverInterface {
 public:
  MOCK_METHOD3(receive_any_message,
  void(device_address
  *address,
  uint8_t *data, uint16_t
  length));
};

#endif //CMQTTSNFORWARDER_MOCKCLIENTNETWORKRECEIVER_H
