//
// Created by SomeDude on 31.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNADDRESSGENERATOR_MQTTSNUDPADDRESSGENERATOR_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNADDRESSGENERATOR_MQTTSNUDPADDRESSGENERATOR_H_

#include <stdint.h>
#include <vector>
#include "MqttSnNetworkBuilderMqttSnAddressGeneratorInterface.h"
class MqttSnUdpAddressGenerator : public MqttSnNetworkBuilderMqttSnAddressGeneratorInterface{
 private:
  const std::vector<uint8_t> baseInterfaceAddress;
  const int32_t baseInterfacePort;
  int32_t GetMqttSnInterfaceAddressCallCount = 0;

  const std::vector<uint8_t> baseBroadcastAddress;
  const int32_t baseBroadcastPort;
  int32_t GetMqttSnBroadcastAddressCallCount = 0;

 public:
  MqttSnUdpAddressGenerator();
  const std::vector<uint8_t> GetMqttSnInterfaceAddress() override;
  const std::vector<uint8_t> GetMqttSnBroadcastAddress() override;
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNNETWORKBUILDER_MQTTSNADDRESSGENERATOR_MQTTSNUDPADDRESSGENERATOR_H_
