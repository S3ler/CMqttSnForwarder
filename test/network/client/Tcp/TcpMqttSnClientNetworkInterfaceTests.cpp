//
// Created by bele on 26.02.19.
//

#ifndef CMQTTSNFORWARDER_TCPMQTTSNCLIENTNETWORKINTERFACETESTS_H
#define CMQTTSNFORWARDER_TCPMQTTSNCLIENTNETWORKINTERFACETESTS_H

#include "../Tests/MqttSnClientNetworkInterfaceTests.h"
#include <vector>
#include "../../../../forwarder/network/client/tcp/MqttSnClientTcpNetwork.h"
#include "../TestConfigurations/MqttSnGatewayClientNetworkTestConfiguration.h"
#include "../TestConfigurations/MqttSnClientNetworkTestValueParameter.h"
#include "MockClientLinuxTcpNetworkImplementation.h"
#include "../TestConfigurations/CartesianTestGenerator.h"
#include "../TestConfigurations/CartesianTestGenerator.cpp"

MqttSnClientTcpNetwork clientNetworkContext = {0};
device_address forwarderAddress({127, 0, 0, 1, (uint8_t) ((uint16_t) 9999 >> 8), (uint8_t) ((uint16_t) 9999 >> 0)});
std::vector<std::shared_ptr<MockClientLinuxTcpNetworkImplementation>> mockClientTcpNetworkInterfaces;
MqttSnGatewayClientNetworkTestConfiguration mqttSnGatewayClientNetworkTestConfiguration(forwarderAddress,
                                                                                        &clientNetworkContext,
                                                                                        ClientLinuxTcpInit,
                                                                                        true);
device_address generateMockClientTcpNetworkAddress(uint16_t mockClientIdentifier) {
  // a zeroed device address tells the test set to use the given device address
  device_address mockClientNetworkAddress({0, 0, 0, 0, 0, 0});
  return mockClientNetworkAddress;
}

::std::vector<MqttSnClientNetworkTestValueParameter>
    clientNetworkTestParameter = getParameterMqttSnClientNetworkTestTypeParameter<generateMockClientTcpNetworkAddress,
                                                                                  MockClientLinuxTcpNetworkImplementation>(
    mqttSnGatewayClientNetworkTestConfiguration,
    mockClientTcpNetworkInterfaces
);

struct PrintToStringMqttSnClientNetworkTestValueParameterParamName {
  template<class ParamType>
  std::string operator()(const ::testing::TestParamInfo<ParamType> &info) const {
    std::string result = std::to_string(info.index);
    std::stringstream buffer;
    buffer << info.param;
    result.append(buffer.str());
    return result;
  }
};

INSTANTIATE_TEST_SUITE_P(SendReceiveTests,
    MqttSnClientNetworkInterfaceTests,
    ::testing::ValuesIn(clientNetworkTestParameter.begin(), clientNetworkTestParameter.end()),
    PrintToStringMqttSnClientNetworkTestValueParameterParamName());

#endif //CMQTTSNFORWARDER_TCPMQTTSNCLIENTNETWORKINTERFACETESTS_H
