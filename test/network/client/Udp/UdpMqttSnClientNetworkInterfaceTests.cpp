//
// Created by SomeDude on 03.04.2019.
//

#ifndef CMQTTSNFORWARDER_UDPMQTTSNCLIENTNETWORKINTERFACETESTS_H
#define CMQTTSNFORWARDER_UDPMQTTSNCLIENTNETWORKINTERFACETESTS_H

#include <gtest/gtest.h>
#include <vector>
#include "../../../../forwarder/network/client/udp/MqttSnClientUdpNetwork.h"
#include <MqttSnFixedSizeRingBufferMock.h>
#include <TestConfigurations/MqttSnGatewayClientNetworkTestConfiguration.h>
#include <CartesianProductTestCaseGenerator.h>
#include "MockClientLinuxUdpNetworkImplementation.h"
#include <MqttSnClientNetworkInterfaceTests.h>

MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock = nullptr;
std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap = nullptr;

device_address getDeviceAddressFromMqttSnClientUdpNetworkContext(uint16_t identifier, void *context) {
  // TODO should never be called - check
  throw std::exception();
}

device_address generateMockClientUdpNetworkAddress(uint16_t mockClientIdentifier) {
  uint16_t p = 20000 + mockClientIdentifier;
  device_address mockClientDeviceAddress({127, 0, 0, 1, (uint8_t)(p >> 8), (uint8_t)(p >> 0)});
  return mockClientDeviceAddress;
}

MqttSnClientUdpNetwork clientNetworkContext = {0};
device_address forwarderAddress
    ({127, 0, 0, 1, (uint8_t)((uint16_t) 10000 >> 8), (uint8_t)((uint16_t) 10000 >> 0)});
bool useIdentifier = false;
std::vector<std::shared_ptr<MockClientLinuxUdpNetworkImplementation>> mockClientUdpNetworkInterfaces;
MqttSnGatewayClientNetworkTestConfiguration mqttSnGatewayClientNetworkTestConfiguration(forwarderAddress,
                                                                                        &clientNetworkContext,
                                                                                        ClientLinuxUdpInit,
                                                                                        getDeviceAddressFromMqttSnClientUdpNetworkContext,
                                                                                        useIdentifier);

std::vector<MqttSnClientNetworkTestValueParameter>
    clientNetworkTestParameter = getParameterMqttSnClientNetworkTestTypeParameter<generateMockClientUdpNetworkAddress,
                                                                                  MockClientLinuxUdpNetworkImplementation>(
    mqttSnGatewayClientNetworkTestConfiguration,
    mockClientUdpNetworkInterfaces
);

struct PrintToStringMqttSnClientUdpNetworkTestValueParameterParamName {
  template<class ParamType>
  std::string operator()(const ::testing::TestParamInfo<ParamType> &info) const {
    std::string result = "Udp" + std::to_string(info.index);
    std::stringstream buffer;
    buffer << info.param;
    result.append(buffer.str());
    return result;
  }
};

INSTANTIATE_TEST_SUITE_P(UdpSendReceiveTests,
    MqttSnClientNetworkInterfaceTests,
    ::testing::ValuesIn(clientNetworkTestParameter.begin(), clientNetworkTestParameter.end()),
    PrintToStringMqttSnClientUdpNetworkTestValueParameterParamName()
);

#endif //CMQTTSNFORWARDER_UDPMQTTSNCLIENTNETWORKINTERFACETESTS_H