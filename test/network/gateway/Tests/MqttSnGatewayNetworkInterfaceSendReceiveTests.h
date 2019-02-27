//
// Created by bele on 26.02.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACETESTS_H
#define CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACETESTS_H

#include <gtest/gtest.h>
#include "../TestConfigurations/MqttSnGatewayNetworkValueParameter.h"
#include "../MockGatewayNetwork/MockGateway.h"
#include "../MockGatewayNetwork/MockGatewayNetworkReceiver.h"
#include "../../../../forwarder/MqttSnFixedSizeRingBuffer.h"
#include "../../../../forwarder/MqttSnGatewayNetworkInterface.h"
#include "../TestConfigurations/MqttSnForwarderGatewayNetworkTestConfiguration.h"
#include "../MockForwarderGatewayNetworkLooper/MockForwarderGatewayNetworkLooper.h"
#include "../Tcp/MockGatewayLinuxTcpNetworkImplementation.h"
#include "../TestConfigurations/GetParameterMqttSnGatewayNetworkTestTypeParameter.h"
#include "../TestConfigurations/GetParameterMqttSnGatewayNetworkTestTypeParameter.cpp"
#include "../../../../forwarder/network/gateway/tcp/MqttSnGatewayTcpNetwork.h"

class MqttSnGatewayNetworkInterfaceSendReceiveTests : public ::testing::TestWithParam<MqttSnGatewayNetworkValueParameter> {
 public:
  MqttSnGatewayNetworkInterface mqttSnGatewayNetworkInterface = {0};
  MockForwarderGatewayNetworkLooper gatewayNetworkForwarderLooper;
  MqttSnFixedSizeRingBuffer receiveBuffer = {0};
  MqttSnFixedSizeRingBuffer sendBuffer = {0};
  device_address mqttSnGatewayDeviceAddress = {0};
 public:
  std::vector<std::shared_ptr<MockGateway>> mockGateways;
  std::vector<std::shared_ptr<MockGatewayNetworkReceiver>> mockGatewayNetworkReceivers;
  uint8_t *(*generateMessageData)(uint8_t *arr, uint16_t len) = nullptr;

  virtual void SetUp() {
    MqttSnFixedSizeRingBufferInit(&receiveBuffer);
    MqttSnFixedSizeRingBufferInit(&sendBuffer);

    MqttSnGatewayNetworkValueParameter const &a = GetParam();

    for (auto &mockGatewayConfiguration : a.mockGatewayConfigurations) {
      std::shared_ptr<MockGatewayNetworkReceiver> receiver(new MockGatewayNetworkReceiver);
      std::shared_ptr<MockGateway> mockGateway(new MockGateway(mockGatewayConfiguration.gatewayIdentifier,
                                                               &mockGatewayConfiguration.address,
                                                               mockGatewayConfiguration.mockGatewayNetworkInterface,
                                                               receiver.get()));
      ASSERT_TRUE(mockGateway->start_loop());
      ASSERT_TRUE(mockGateway->isNetworkConnected());

      mockGateways.push_back(mockGateway);
      mockGatewayNetworkReceivers.push_back(receiver);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    MqttSnForwarderGatewayNetworkTestConfiguration p = a.mqttSnForwarderGatewayNetworkTestConfiguration;

    if (a.searchGateway) {
      ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                                   &p.forwarderGatewayNetworkAddress,
                                   NULL,
                                   p.gatewayNetworkContext,
                                   p.gateway_network_init), 0);
    } else {
      mqttSnGatewayDeviceAddress = a.mqttSnGatewayNetworkAddress;
      ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                                   &p.forwarderGatewayNetworkAddress,
                                   &mqttSnGatewayDeviceAddress,
                                   p.gatewayNetworkContext,
                                   p.gateway_network_init), 0);
    }

    ASSERT_EQ(GatewayNetworkConnect(&mqttSnGatewayNetworkInterface, p.gatewayNetworkContext), 0);
    ASSERT_TRUE(gatewayNetworkForwarderLooper.startNetworkLoop(mqttSnGatewayNetworkInterface.gateway_receive,
                                                               &mqttSnGatewayNetworkInterface,
                                                               &receiveBuffer,
                                                               1000,
                                                               p.gatewayNetworkContext));
    this->generateMessageData = a.messageDataGenerator;

  }

  virtual void TearDown() {
    gatewayNetworkForwarderLooper.stopNetworkLoop();
    while (!gatewayNetworkForwarderLooper.isStopped) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    GatewayNetworkDisconnect(&mqttSnGatewayNetworkInterface,
                             GetParam().mqttSnForwarderGatewayNetworkTestConfiguration.gatewayNetworkContext);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    for (auto &mockGateway : mockGateways) {
      mockGateway->stop_loop();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  MqttSnGatewayNetworkInterfaceSendReceiveTests() {

  }

  virtual ~MqttSnGatewayNetworkInterfaceSendReceiveTests() {

  }
};


#endif //CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKINTERFACETESTS_H
