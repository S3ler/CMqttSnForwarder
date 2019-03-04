//
// Created by SomeDude on 04.03.2019.
//

#include "MqttSnGatewayNetworkInterfaceTests.h"
#include "../../forwarder/MqttSnGatewayNetworkInterface.h"

#include "MockMqttSnGatewayNetwork/GatewayNetworkMock.h"

using ::testing::Return;
using ::testing::_;
using ::testing::Invoke;

TEST_F(MqttSnGatewayNetworkInterfaceTests, MqttSnGatewayNetworkInterfaceTests_InitSucess_ReturnsZero) {
  device_address own_address({0, 0, 0, 0, 0, 0});
  device_address gateway_address({0, 0, 0, 0, 0, 0});

  void *context = nullptr;
  MqttSnGatewayNetworkInterface mqttSnGatewayNetworkInterface = {0};

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface, context))
      .Times(1)
      .WillOnce(Return(0));

  EXPECT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &own_address,
                               &gateway_address,
                               context,
                               mock_gateway_network_init), 0);

}

TEST_F(MqttSnGatewayNetworkInterfaceTests, MqttSnGatewayNetworkInterfaceTests_AfterInitSucess_StatusIsZero) {
  device_address own_address({0, 0, 0, 0, 0, 0});
  device_address gateway_address({0, 0, 0, 0, 0, 0});

  void *context = nullptr;
  MqttSnGatewayNetworkInterface mqttSnGatewayNetworkInterface = {0};

  EXPECT_CALL(gatewayNetworkMock, gateway_network_init(&mqttSnGatewayNetworkInterface, context))
      .Times(1)
      .WillOnce(Return(0));

  ASSERT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
                               &own_address,
                               &gateway_address,
                               context,
                               mock_gateway_network_init), 0);

  EXPECT_EQ(mqttSnGatewayNetworkInterface.status, 0);
}

// TODO implement more Tests