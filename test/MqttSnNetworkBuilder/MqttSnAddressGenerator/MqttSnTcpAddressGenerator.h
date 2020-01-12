//
// Created by SomeDude on 17.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNTCPADDRESSGENERATOR_H
#define CMQTTSNFORWARDER_MQTTSNTCPADDRESSGENERATOR_H

#include <stdint.h>
#include <vector>
#include "MqttSnNetworkBuilderMqttSnAddressGeneratorInterface.h"
class MqttSnTcpAddressGenerator : public MqttSnNetworkBuilderMqttSnAddressGeneratorInterface {
 private:
  const std::vector<uint8_t> baseInterfaceAddress;
  const int32_t baseInterfacePort;
  int32_t GetMqttSnInterfaceAddressCallCount = 0;

  const std::vector<uint8_t> baseBroadcastAddress;
  const int32_t baseBroadcastPort;
  int32_t GetMqttSnBroadcastAddressCallCount = 0;

 public:
  MqttSnTcpAddressGenerator();
  const std::vector<uint8_t> GetMqttSnInterfaceAddress() override;
  const std::vector<uint8_t> GetMqttSnBroadcastAddress() override;
};

#endif  // CMQTTSNFORWARDER_MQTTSNTCPADDRESSGENERATOR_H
