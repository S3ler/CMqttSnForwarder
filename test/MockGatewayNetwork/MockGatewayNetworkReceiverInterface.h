//
// Created by bele on 03.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKGATEWAYNETWORKRECEIVERINTERFACE_H
#define CMQTTSNFORWARDER_MOCKGATEWAYNETWORKRECEIVERINTERFACE_H

#include <cstdint>

class MockGatewayNetworkReceiverInterface {
 public:

  virtual ~MockGatewayNetworkReceiverInterface() {};

  virtual void receive_any_message(uint8_t *data, uint16_t length) = 0;

};
#endif //CMQTTSNFORWARDER_MOCKGATEWAYNETWORKRECEIVERINTERFACE_H
