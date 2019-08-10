//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTOBJECTBUILDERCONFIGURATION_H_
#define CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTOBJECTBUILDERCONFIGURATION_H_

#include <stdint.h>
#include <string>
#include "../MqttBroker/MqttBrokerTestContainerConfiguration.h"
#include "MqttNetworkBuilderMqttClientBuilderConfiguration.h"
#include "MqttNetworkBuilderMqttBrokerBuilderConfiguration.h"

class MqttNetworkBuilderConfiguration {
 public:
  const MqttNetworkBuilderMqttBrokerBuilderConfiguration mqtt_broker_config;
  const MqttNetworkBuilderMqttClientBuilderConfiguration mqtt_client_config;
  // MQTT-Broker
  /*
  MqttBrokerTestType mqttBrokerTestType;
  uint16_t mqttBrokerTestMinPort;
  uint16_t mqttBrokerTestMaxPort;
  std::string mqttBrokerTestNamePrefix;
   */
  // Mqtt Broker implementations
  // naming schema is fix mqttBrokerTestNamePrefix+mqttBrokerPort;
  // Mqtt Broker Port Pool (Min, Max)
  // Broker naming/identifier does the builder
  // throw exception when no more ports available

  // Mqtt Client types
  // Mqtt Client implementation
  // Mqtt Client's Broker Address+Port by Mqtt-Broker distribution type (single, equal)
  // Client naming/identifier does the builder

  // Mqtt Test Client Types
  // Mqtt Test Client implementation
  // Mqtt Test Client's Broker Address+Port by Mqtt-Broker distribution type (single, equal)
  // Client naming/identifier does the builder

  // Mqtt-Sn Gateway
  // Mqtt-SN Gateway implementation
  // Mqtt-Sn Gateway's Broker Address+Port with Distribution (single, equal)
  // Mqtt-Sn Gateway's Network Address Pool - depend on implementation (Min, Max, List)
  // Mqtt-sn Gateway's Multicast Network Address

  // Mqtt-Sn Client
  // Mqtt-Sn Client implementation
  // Mqtt-SN Client's find gateway strategy (Default=Direct, SearchGw, Advertisement)
  // On Direct: Mqtt-SN Client's Gateway Address + Mqtt-SN Client's Multicast Network Address (same as Gateway's)
  // On Other: Mqtt-SN Client's Multicast Network Address (same as Gateway)
  // Mqtt-Sn Client Network Address Pool - depend on implementation (Min, Max, List)

  // Mqtt-Sn Forwarder
  // Mqtt-Sn Forwarder implementation
  // Mqtt-Sn Forwarder's find gateway strategy (Default=Direct, SearchGw, Advertisement)
  // On Direct: Mqtt-Sn Forwarder's Gateway Address + Mqtt-Sn Forwarder's Multicast Network Address (same as Gateway's)
  // On Other: Mqtt-Sn Forwarder's Multicast Network Address (same as Gateway's)
 public:
  MqttNetworkBuilderConfiguration(const MqttNetworkBuilderMqttBrokerBuilderConfiguration &mqtt_broker_config,
                                  const MqttNetworkBuilderMqttClientBuilderConfiguration &mqtt_client_config);
  static const MqttNetworkBuilderConfiguration DefaultConfiguration();
};

#endif //CMQTTSNFORWARDER_TEST_MQTTOBJECTBUILDER_MQTTOBJECTBUILDERCONFIGURATION_H_
