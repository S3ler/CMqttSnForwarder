//
// Created by bele on 27.02.19.
//

#include "MqttSnGatewayNetworkInterfaceInitializationTests.h"
/*
 * int GatewayNetworkInit(MqttSnGatewayNetworkInterface *n,
                       device_address *gateway_network_address,
                       device_address *mqtt_sn_gateway_network_address,
                       void *context,
                       int (*gateway_network_init)(struct MqttSnGatewayNetworkInterface *, void *context));
 */
TEST_P(MqttSnGatewayNetworkInterfaceInitializationTests, GatewayNetworkInitSuccessReturnsZero){
  MqttSnGatewayNetworkValueParameter p = GetParam();
  MqttSnGatewayNetworkInterface mqttSnGatewayNetworkInterface;
  EXPECT_EQ(GatewayNetworkInit(&mqttSnGatewayNetworkInterface,
      &p.mqttSnForwarderGatewayNetworkTestConfiguration.forwarderGatewayNetworkAddress,
      &p.mqttSnGatewayNetworkAddress,
      p.mqttSnForwarderGatewayNetworkTestConfiguration.gatewayNetworkContext,
      p.mqttSnForwarderGatewayNetworkTestConfiguration.gateway_network_init), 0);
  ASSERT_TRUE(false) << "not implemented yet";
}

TEST_P(MqttSnGatewayNetworkInterfaceInitializationTests, ClientNetworkInitFailsReturnsMinusOne){
  ASSERT_TRUE(false) << "not implemented yet";
}