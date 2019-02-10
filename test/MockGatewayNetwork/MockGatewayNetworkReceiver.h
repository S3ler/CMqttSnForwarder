//
// Created by bele on 03.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKGATEWAYNETWORKRECEIVER_H
#define CMQTTSNFORWARDER_MOCKGATEWAYNETWORKRECEIVER_H

#include "MockGatewayNetworkReceiverInterface.h"
#include "../../cmake-build-debug/googletest-src/googlemock/include/gmock/gmock-function-mocker.h"

class MockGatewayNetworkReceiver : public MockGatewayNetworkReceiverInterface {
 public:
  MOCK_METHOD2(receive_any_message, void(uint8_t *data, uint16_t length));
};

#endif //CMQTTSNFORWARDER_MOCKGATEWAYNETWORKRECEIVER_H
