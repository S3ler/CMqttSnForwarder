//
// Created by SomeDude on 30.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTNETWORKBUILDER_MQTTSNGATEWAYTESTCONTAINERCONFIGURATION_H_
#define CMQTTSNFORWARDER_TEST_MQTTNETWORKBUILDER_MQTTSNGATEWAYTESTCONTAINERCONFIGURATION_H_

#include <string>
#include <test/MqttClient/MqttClientTestContainerConfiguration.h>
#include <test/MqttSnGateway/MqttSnGatewayProtocolTestType.h>

class MqttSnGatewayTestContainerConfiguration {
 public:
  // MQTT-Broker
  const MqttClientTestContainerConfiguration mqtt_client_configuration;

  // MQTT-SN Network
  const MqttSnGatewayProtocolTestType protocol;  // example UDP/BLE/B5.0/ZigBee
  const std::vector<uint8_t> interfaceAddress;
  const std::vector<uint8_t> interfaceBroadcastAddress;
  const uint8_t gateway_id;
  const uint64_t advertise_duration;
  const std::vector<std::tuple<uint16_t, std::string>> predefined_topics;

  MqttSnGatewayTestContainerConfiguration(const MqttClientTestContainerConfiguration& mqtt_client_configuration, const MqttSnGatewayProtocolTestType a_protocol,
                                          const std::vector<uint8_t>& interface_address, const std::vector<uint8_t>& interface_broadcast_address, const uint8_t gateway_id,
                                          const uint16_t advertise_duration, const std::vector<std::tuple<uint16_t, std::string>>& predefined_topics);
  MqttSnGatewayTestContainerConfiguration(const MqttClientTestContainerConfiguration& mqtt_client_configuration, const MqttSnGatewayProtocolTestType a_protocol, const std::string& interfaceAddressURL,
                                          const int32_t interfacePort, const std::string& interfaceBroadcastAddressURL, const int32_t interfaceBroadcastPort, const uint8_t gateway_id,
                                          const uint16_t advertise_duration, const std::vector<std::tuple<uint16_t, std::string>>& predefined_topics);
  
  static const MqttSnGatewayTestContainerConfiguration GetDefaultTestContainerConfiguration();
};

#endif  // CMQTTSNFORWARDER_TEST_MQTTNETWORKBUILDER_MQTTSNGATEWAYTESTCONTAINERCONFIGURATION_H_
