//
// Created by bele on 27.02.19.
//

#ifndef CMQTTSNFORWARDER_LINUXTCPMQTTSNGATEWAYNETWORKINTERFACETESTS_H
#define CMQTTSNFORWARDER_LINUXTCPMQTTSNGATEWAYNETWORKINTERFACETESTS_H

#include <vector>
#include <gtest/gtest.h>
#include "../../../../forwarder/global_defines.h"
#include "../../../../forwarder/network/gateway/tcp/MqttSnGatewayTcpNetwork.h"
#include "MockGatewayLinuxTcpNetworkImplementation.h"
#include "../TestConfigurations/MqttSnGatewayNetworkValueParameter.h"
#include "../TestConfigurations/GetParameterMqttSnGatewayNetworkTestTypeParameter.h"
#include "../Tests/MqttSnGatewayNetworkInterfaceSendReceiveTests.h"

device_address generateMockGatewayTcpNetworkAddress(uint16_t mockClientIdentifier) {
  // a zeroed device address tells the test set to use the given device address
  device_address mockClientNetworkAddress({0, 0, 0, 0, 0, 0});
  return mockClientNetworkAddress;
}

device_address forwarderGatewayNetworkAddress({0, 0, 0, 0, 0, 0});
MqttSnGatewayTcpNetwork gatewayNetworkContext = {0};
std::vector<std::shared_ptr<MockGatewayLinuxTcpNetworkImplementation>> mockGatewayTcpNetworkInterfaces;

MqttSnForwarderGatewayNetworkTestConfiguration mqttSnForwarderGatewayNetworkTestConfiguration(
    forwarderGatewayNetworkAddress,
    &gatewayNetworkContext,
    GatewayLinuxTcpInit,
    true);

std::vector<MqttSnGatewayNetworkValueParameter> gatewayNetworkTestParameter =
    getParameterMqttSnGatewayNetworkTestTypeParameter<generateMockGatewayTcpNetworkAddress,
                                                      MockGatewayLinuxTcpNetworkImplementation>(
        mqttSnForwarderGatewayNetworkTestConfiguration,
        mockGatewayTcpNetworkInterfaces
    );

struct PrintToStringMqttSnClientGatewayNetworkTestValueParameterParamName {
  template<class ParamType>
  std::string operator()(const ::testing::TestParamInfo<ParamType> &info) const {
    std::string result = std::to_string(info.index);
    std::stringstream buffer;
    buffer << info.param;
    result.append(buffer.str());
    return result;
  }
};

INSTANTIATE_TEST_SUITE_P(GatewaySendReceiveTests,
    MqttSnGatewayNetworkInterfaceSendReceiveTests,
    ::testing::ValuesIn(gatewayNetworkTestParameter.begin(), gatewayNetworkTestParameter.end()),
    PrintToStringMqttSnClientGatewayNetworkTestValueParameterParamName());


#endif //CMQTTSNFORWARDER_LINUXTCPMQTTSNGATEWAYNETWORKINTERFACETESTS_H
