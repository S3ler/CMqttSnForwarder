//
// Created by SomeDude on 04.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACETESTS_H
#define CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACETESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <GatewayNetworkMock.h>
#include "../../forwarder/MqttSnFixedSizeRingBuffer.h"
#include "../../forwarder/MqttSnGatewayNetworkInterface.h"
#include "../shared/PlaceholderNetworkContext/PlaceholderNetworkContext.h"

#ifndef GLOBAL_TEST_EXEC
GatewayNetworkMock *globalGatewayNetworkMockObj = nullptr;
#else
extern GatewayNetworkMock *globalGatewayNetworkMockObj;
#endif

using ::testing::Return;
using ::testing::_;
using ::testing::Invoke;

class MqttSnGatewayNetworkInterfaceTests : public ::testing::Test {
 public:

  GatewayNetworkMock gatewayNetworkMock;

  PlaceholderNetworkContext placeholderContext;
  void *gatewayNetworkContext = &placeholderContext;

  device_address forwarder_gateway_network_address;
  device_address mqtt_sn_gateway_network_address;
  MqttSnGatewayNetworkInterface mqttSnGatewayNetworkInterface;

  virtual void SetUp() {
    globalGatewayNetworkMockObj = &gatewayNetworkMock;
    memset(&mqttSnGatewayNetworkInterface, 0, sizeof(MqttSnGatewayNetworkInterface));
    memset(&forwarder_gateway_network_address, 1, sizeof(device_address));
    memset(&mqtt_sn_gateway_network_address, 2, sizeof(device_address));
  }

  virtual void TearDown() {
    memset(&mqttSnGatewayNetworkInterface, 0, sizeof(MqttSnGatewayNetworkInterface));
    memset(&forwarder_gateway_network_address, 0, sizeof(device_address));
    memset(&mqtt_sn_gateway_network_address, 0, sizeof(device_address));
    globalGatewayNetworkMockObj = nullptr;
  }

  static int fakeGatewayNetworkInitSuccess(struct MqttSnGatewayNetworkInterface *n, void *context) {
    n->gateway_network_receive = mock_gateway_network_receive;
    n->gateway_network_send = mock_gateway_network_send;
    n->gateway_network_init = mock_gateway_network_init;
    n->gateway_network_connect = mock_gateway_network_connect;
    n->gateway_network_disconnect = mock_gateway_network_disconnect;
    return 0;
  };


  static int fakeGatewayNetworkInitFail(struct MqttSnGatewayNetworkInterface *n, void *context) {
    n->gateway_network_receive = mock_gateway_network_receive;
    n->gateway_network_send = mock_gateway_network_send;
    n->gateway_network_init = mock_gateway_network_init;
    n->gateway_network_connect = mock_gateway_network_connect;
    n->gateway_network_disconnect = mock_gateway_network_disconnect;
    return -1;
  };

  MqttSnGatewayNetworkInterfaceTests() {}
  virtual ~MqttSnGatewayNetworkInterfaceTests() {}
};

#endif //CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACETESTS_H
