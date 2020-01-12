//
// Created by SomeDude on 17.09.2019.
//

#include "MqttSnTcpAddressGenerator.h"
#include <config/network/gateway/gateway_network_config.h>
#include <test/shared/MqttSnURLParser/MqttSnURLParser.h>

MqttSnTcpAddressGenerator::MqttSnTcpAddressGenerator()
    : baseInterfaceAddress({0, 0, 0, 0}), baseInterfacePort(10000), baseBroadcastAddress({225, 1, 1, 1}), baseBroadcastPort(DEFAULT_MQTT_SN_GATEWAY_BROADCAST_BIND_PORT) {}

const std::vector<uint8_t> MqttSnTcpAddressGenerator::GetMqttSnInterfaceAddress() {
  auto result = std::vector<uint8_t>(baseInterfaceAddress);
  auto portVector = MqttSnURLParser::GetPortVectorFromInt32(baseInterfacePort + GetMqttSnInterfaceAddressCallCount);
  GetMqttSnInterfaceAddressCallCount += 1;
  result.insert(result.end(), portVector.begin(), portVector.end());
  return result;
}
const std::vector<uint8_t> MqttSnTcpAddressGenerator::GetMqttSnBroadcastAddress() {
  auto result = std::vector<uint8_t>(baseBroadcastAddress);
  auto portVector = MqttSnURLParser::GetPortVectorFromInt32(baseBroadcastPort + GetMqttSnBroadcastAddressCallCount);
  GetMqttSnBroadcastAddressCallCount += 1;
  result.insert(result.end(), portVector.begin(), portVector.end());
  return result;
}
