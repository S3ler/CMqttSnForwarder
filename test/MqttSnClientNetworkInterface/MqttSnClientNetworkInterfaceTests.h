//
// Created by SomeDude on 03.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACETESTS_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACETESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <global_defines.h>
#include <MqttSnClientNetworkInterface.h>
#include <shared/PlaceholderNetworkContext/PlaceholderNetworkContext.h>
#include <shared/MockMqttSnClientNetwork/ClientNetworkMockInterface.h>
#include <shared/MockMqttSnClientNetwork/ClientNetworkMock.h>

ClientNetworkMock *globalClientNetworkMockObj = nullptr;

class MqttSnClientNetworkInterfaceTests : public ::testing::Test {
 public:
  ClientNetworkMock clientNetworkMock;

  PlaceholderNetworkContext placeholderContext;
  void *clientNetworkContext = &placeholderContext;

  device_address forwarder_client_network_address;
  device_address mqtt_sn_gateway_network_address;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface;

  virtual void SetUp() {
    globalClientNetworkMockObj = &clientNetworkMock;
    memset(&mqttSnClientNetworkInterface, 0, sizeof(MqttSnClientNetworkInterface));
    memset(&forwarder_client_network_address, 1, sizeof(device_address));
    memset(&mqtt_sn_gateway_network_address, 2, sizeof(device_address));
  }
  virtual void TearDown() {
    memset(&mqttSnClientNetworkInterface, 0, sizeof(MqttSnClientNetworkInterface));
    memset(&forwarder_client_network_address, 0, sizeof(device_address));
    memset(&mqtt_sn_gateway_network_address, 0, sizeof(device_address));
    globalClientNetworkMockObj = nullptr;
  }

  static int fakeClientNetworkInitSuccess(MqttSnClientNetworkInterface *n, void *context) {
    n->client_network_receive = mock_client_network_receive;
    n->client_network_send = mock_client_network_send;
    n->client_network_init = mock_client_network_init;
    n->client_network_connect = mock_client_network_connect;
    n->client_network_disconnect = mock_client_network_disconnect;
    return 0;
  };

  static int fakeClientNetworkInitFail(MqttSnClientNetworkInterface *n, void *context) {
    n->client_network_receive = mock_client_network_receive;
    n->client_network_send = mock_client_network_send;
    n->client_network_init = mock_client_network_init;
    n->client_network_connect = mock_client_network_connect;
    n->client_network_disconnect = mock_client_network_disconnect;
    return -1;
  };

  virtual ~MqttSnClientNetworkInterfaceTests() {}

  MqttSnClientNetworkInterfaceTests() {}
};

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACETESTS_H
