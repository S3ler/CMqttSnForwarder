//
// Created by SomeDude on 17.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERTESTCONTAINERCONFIGURATION_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDERTESTCONTAINERCONFIGURATION_H

#include "../MqttSnClient/MqttSnClientTestContainerConfiguration.h"

#ifndef DEFAULT_MQTT_SN_FORWADER_BROADCAST_ADDRESS
#define DEFAULT_MQTT_SN_FORWADER_BROADCAST_ADDRESS "255.1.1.2"
#endif

#ifndef DEFAULT_MQTT_SN_FORWADER_BROADCAST_PORT
#define DEFAULT_MQTT_SN_FORWADER_BROADCAST_PORT 1884
#endif

class MqttSnForwarderTestContainerConfiguration {
 public:
  // MQTT-SN Gateway Network
  const MqttSnClientTestContainerConfiguration mqttSnClientConfiguration;

  // MQTT-SN Client Network
  const MqttSnGatewayProtocolTestType protocol;
  const std::vector<uint8_t> interfaceAddress;
  const std::vector<uint8_t> interfaceBroadcastAddress;

  MqttSnForwarderTestContainerConfiguration(const MqttSnClientTestContainerConfiguration& mqttSnClientConfiguration, const MqttSnGatewayProtocolTestType protocol, const std::string interfaceAddress,
                                            const uint16_t interfacePort, const std::string interfaceBroadcastAddress, const uint16_t interfaceBroadcastPort);
  MqttSnForwarderTestContainerConfiguration(const MqttSnClientTestContainerConfiguration& mqttSnClientConfiguration, const MqttSnGatewayProtocolTestType protocol,
                                            const std::vector<uint8_t>& interfaceAddress, const std::vector<uint8_t>& interfaceBroadcastAddress);

  static MqttSnForwarderTestContainerConfiguration GetDefaultTestContainerConfiguration();
};

#endif  // CMQTTSNFORWARDER_MQTTSNFORWARDERTESTCONTAINERCONFIGURATION_H
