//
// Created by bele on 06.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKCLIENTNETWORKRECEIVER_H
#define CMQTTSNFORWARDER_MOCKCLIENTNETWORKRECEIVER_H

#include "MockClientNetworkReceiverInterface.h"
#include "../../cmake-build-debug/googletest-src/googlemock/include/gmock/gmock-function-mocker.h"

class MockClientNetworkReceiver : public MockClientNetworkReceiverInterface {
 public:
  MOCK_METHOD2(receive_any_message, void(uint8_t *data, uint16_t length));
};

#endif //CMQTTSNFORWARDER_MOCKCLIENTNETWORKRECEIVER_H
