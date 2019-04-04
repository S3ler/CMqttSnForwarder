//
// Created by bele on 27.02.19.
//

#ifndef CMQTTSNFORWARDER_LINUXTCPMQTTSNGATEWAYNETWORKINTERFACETESTS_H
#define CMQTTSNFORWARDER_LINUXTCPMQTTSNGATEWAYNETWORKINTERFACETESTS_H

#include <vector>
#include <gtest/gtest.h>
#include <network/tcphelper/MqttSnTcpNetworkMessageParser.h>
#include "../../../../forwarder/global_defines.h"
#include "../../../../forwarder/network/gateway/tcp/MqttSnGatewayTcpNetwork.h"
#include "MockGatewayLinuxTcpNetworkImplementation.h"
#include "../TestConfigurations/MqttSnGatewayNetworkValueParameter.h"
#include "TestConfigurations/GetParameterMqttSnGatewayNetworkTestTypeParameter.h"
#include "../Tests/MqttSnGatewayNetworkInterfaceSendReceiveTests.h"
#include "MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter.h"
#include "MqttSnGatewayNetworkInterfaceMessageDefragmentationTests.h"
#include "GetMqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter.h"
#include <netinet/in.h>

int getDeviceAddressFromFileDescriptor1(int peer_fd, device_address *peer_address) {
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);
  getpeername(peer_fd, (struct sockaddr *) &address, &addrlen);

  unsigned char *ip = (unsigned char *) &address.sin_addr.s_addr;
  (*peer_address).bytes[0] = ip[0];
  (*peer_address).bytes[1] = ip[1];
  (*peer_address).bytes[2] = ip[2];
  (*peer_address).bytes[3] = ip[3];

  uint16_t port_as_number = (uint16_t) htons(address.sin_port);
  (*peer_address).bytes[4] = (uint8_t) (port_as_number >> 8);
  (*peer_address).bytes[5] = (uint8_t) (port_as_number >> 0);
  return 0;
}

MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;
std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap;

device_address generateMockGatewayTcpNetworkAddress(uint16_t mockClientIdentifier) {
  // a zeroed device forwarderAddress tells the test set to use the given device forwarderAddress
  uint16_t p = 8888 + mockClientIdentifier;
  device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});

  return mockGatewayDeviceAddress;
}

device_address forwarderGatewayNetworkAddress({0, 0, 0, 0, 0, 0});
MqttSnGatewayTcpNetwork tcpGatewayNetworkContext = {0};
std::vector<std::shared_ptr<MockGatewayLinuxTcpNetworkImplementation>> mockGatewayTcpNetworkInterfaces;

device_address getDeviceAddressFromMqttSnGatewayTcpNetworkContext(uint16_t identifier, void *context) {
  MqttSnGatewayTcpNetwork *clientTcpNetwork = (MqttSnGatewayTcpNetwork *) context;
  device_address peer_address = {0};
  if (clientTcpNetwork->mqtt_sg_gateway_fd <= 0) {
    throw std::bad_exception();
  }
  getDeviceAddressFromFileDescriptor1(clientTcpNetwork->mqtt_sg_gateway_fd, &peer_address);
  return peer_address;
}

MqttSnForwarderGatewayNetworkTestConfiguration mqttSnForwarderGatewayNetworkTestConfiguration(
    forwarderGatewayNetworkAddress,
    &tcpGatewayNetworkContext,
    GatewayLinuxTcpInit,
    getDeviceAddressFromMqttSnGatewayTcpNetworkContext,
    true);

std::vector<MqttSnGatewayNetworkValueParameter> gatewayNetworkTestParameter =
    getParameterMqttSnGatewayNetworkTestTypeParameter<generateMockGatewayTcpNetworkAddress,
                                                      MockGatewayLinuxTcpNetworkImplementation>(
        mqttSnForwarderGatewayNetworkTestConfiguration,
        mockGatewayTcpNetworkInterfaces
    );

std::vector<MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter> gatewayNetworkDefragmentationTestParameter =
    getMqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter<generateMockGatewayTcpNetworkAddress,
                                                                  MockGatewayLinuxTcpNetworkImplementation>(
        mqttSnForwarderGatewayNetworkTestConfiguration,
        mockGatewayTcpNetworkInterfaces
    );

struct PrintToStringMqttSnClientGatewayTcpNetworkTestValueParameterParamName {
  template<class ParamType>
  std::string operator()(const ::testing::TestParamInfo<ParamType> &info) const {
    std::string result = "Tcp" + std::to_string(info.index);
    std::stringstream buffer;
    buffer << info.param;
    result.append(buffer.str());
    return result;
  }
};

/*
INSTANTIATE_TEST_SUITE_P(GatewayInitializationTests,
                         MqttSnGatewayNetworkInterfaceInitializationTests,
                         ::testing::ValuesIn(gatewayNetworkTestParameter.begin(), gatewayNetworkTestParameter.end()),
                         PrintToStringMqttSnClientGatewayNetworkTestValueParameterParamName());

INSTANTIATE_TEST_SUITE_P(GatewayConnectTests,
                         MqttSnGatewayNetworkInterfaceConnectTests,
                         ::testing::ValuesIn(gatewayNetworkTestParameter.begin(), gatewayNetworkTestParameter.end()),
                         PrintToStringMqttSnClientGatewayNetworkTestValueParameterParamName());
                         */

INSTANTIATE_TEST_SUITE_P(GatewaySendReceiveTests,
                         MqttSnGatewayNetworkInterfaceSendReceiveTests,
                         ::testing::ValuesIn(gatewayNetworkTestParameter.begin(), gatewayNetworkTestParameter.end()),
                         PrintToStringMqttSnClientGatewayTcpNetworkTestValueParameterParamName());

INSTANTIATE_TEST_SUITE_P(GatewayDefragmentationTests,
                         MqttSnGatewayNetworkInterfaceMessageDefragmentationTests,
                         ::testing::ValuesIn(gatewayNetworkDefragmentationTestParameter.begin(),
                                             gatewayNetworkDefragmentationTestParameter.end()),
                         PrintToStringMqttSnClientGatewayTcpNetworkTestValueParameterParamName());

#endif //CMQTTSNFORWARDER_LINUXTCPMQTTSNGATEWAYNETWORKINTERFACETESTS_H
