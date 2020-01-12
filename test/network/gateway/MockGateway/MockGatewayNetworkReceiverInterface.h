//
// Created by bele on 03.02.19.
//

#ifndef CMQTTSNFORWARDER_MOCKGATEWAYNETWORKRECEIVERINTERFACE_H
#define CMQTTSNFORWARDER_MOCKGATEWAYNETWORKRECEIVERINTERFACE_H

#include <cstdint>

class MockGatewayNetworkReceiverInterface {
 public:
  virtual void receive_any_message(device_address *address, uint8_t *data, uint16_t length) = 0;

  virtual ~MockGatewayNetworkReceiverInterface() {};
};
#endif //CMQTTSNFORWARDER_MOCKGATEWAYNETWORKRECEIVERINTERFACE_H
