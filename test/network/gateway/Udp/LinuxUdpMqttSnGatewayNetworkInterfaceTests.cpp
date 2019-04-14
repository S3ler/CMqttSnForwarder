//
// Created by SomeDude on 01.04.2019.
//

#ifndef CMQTTSNFORWARDER_LINUXUDPMQTTSNGATEWAYNETWORKINTERFACETESTS_CPP
#define CMQTTSNFORWARDER_LINUXUDPMQTTSNGATEWAYNETWORKINTERFACETESTS_CPP

#include <gtest/gtest.h>
#include <global_defines.h>
#include <network/gateway/udp/MqttSnGatewayUdpNetwork.h>
#include <vector>
#include <memory>
#include <TestConfigurations/MqttSnGatewayNetworkValueParameter.h>
#include <MqttSnFixedSizeRingBufferMock.h>
#include <TestConfigurations/GetParameterMqttSnGatewayNetworkTestTypeParameter.h>
#include "MockGatewayLinuxUdpNetworkImplementation.h"
#include "../Tests/MqttSnGatewayNetworkInterfaceSendReceiveTests.h"

MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;
std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap;

device_address generateMockGatewayUdpNetworkAddress(uint16_t mockClientIdentifier) {
  uint16_t p = 20000 + mockClientIdentifier;
  device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
  return mockGatewayDeviceAddress;
}

device_address forwarderGatewayNetworkAddress({127, 0, 0, 1, (uint8_t) (10000 >> 8), (uint8_t) (10000 >> 0)});
MqttSnGatewayUdpNetwork udpGatewayNetworkContext = {0};
std::vector<std::shared_ptr<MockGatewayLinuxUdpNetworkImplementation>> mockGatewayUdpNetworkInterfaces;

device_address getDeviceAddressFromMqttSnGatewayUdpNetworkContext(uint16_t identifier, void *context) {
  // TODO should never be called - check
  throw std::exception();
  device_address peer_address = {0};
  return peer_address;
}

MqttSnForwarderGatewayNetworkTestConfiguration mqttSnForwarderGatewayNetworkTestConfiguration(
    forwarderGatewayNetworkAddress,
    &udpGatewayNetworkContext,
    GatewayLinuxUdpInit,
    getDeviceAddressFromMqttSnGatewayUdpNetworkContext,
    false);

std::vector<MqttSnGatewayNetworkValueParameter> udpGatewayNetworkTestParameter =
    getParameterMqttSnGatewayNetworkTestTypeParameter<generateMockGatewayUdpNetworkAddress,
                                                      MockGatewayLinuxUdpNetworkImplementation>(
        mqttSnForwarderGatewayNetworkTestConfiguration,
        mockGatewayUdpNetworkInterfaces
    );

struct PrintToStringMqttSnGatewayUdpNetworkTestValueParameterParamName {
  template<class ParamType>
  std::string operator()(const ::testing::TestParamInfo<ParamType> &info) const {
    std::string result = "Udp" + std::to_string(info.index);
    std::stringstream buffer;
    buffer << info.param;
    result.append(buffer.str());
    return result;
  }
};

INSTANTIATE_TEST_SUITE_P(UdpGatewaySendReceiveTests,
                         MqttSnGatewayNetworkInterfaceSendReceiveTests,
                         ::testing::ValuesIn(udpGatewayNetworkTestParameter.begin(),
                                             udpGatewayNetworkTestParameter.end()),
                         PrintToStringMqttSnGatewayUdpNetworkTestValueParameterParamName());

#endif //CMQTTSNFORWARDER_LINUXUDPMQTTSNGATEWAYNETWORKINTERFACETESTS_CPP
