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
#include "../TestConfigurations/CartesianProductTestCaseGenerator.h"
#include "../TestConfigurations/CartesianProductTestCaseGenerator.cpp"
#include "MqttSnClientNetworkTcpNetworkDefragmentationTestParameter.h"
#include "GetMqttSnClientNetworkTcpNetworkDefragmentationTestParameter.h"
#include "MqttSnClientNetworkInterfaceMessageDefragmentationTests.h"

MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock = nullptr;
std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap = nullptr;

device_address getDeviceAddressFromMqttSnClientTcpNetworkContext(uint16_t identifier, void *context) {
  MqttSnClientTcpNetwork *clientTcpNetwork = (MqttSnClientTcpNetwork *) context;
  device_address peer_address = {0};
  if (clientTcpNetwork->client_socket[identifier] <= 0) {
    throw std::bad_exception();
  }
  getDeviceAddressFromFileDescriptor(clientTcpNetwork->client_socket[identifier], &peer_address);
  return peer_address;
}

MqttSnClientTcpNetwork clientNetworkContext = {0};
device_address forwarderAddress({127, 0, 0, 1, (uint8_t)((uint16_t) 9999 >> 8), (uint8_t)((uint16_t) 9999 >> 0)});
std::vector<std::shared_ptr<MockClientLinuxTcpNetworkImplementation>> mockClientTcpNetworkInterfaces;
MqttSnGatewayClientNetworkTestConfiguration mqttSnGatewayClientNetworkTestConfiguration(forwarderAddress,
                                                                                        &clientNetworkContext,
                                                                                        ClientLinuxTcpInit,
                                                                                        getDeviceAddressFromMqttSnClientTcpNetworkContext,
                                                                                        true);
device_address generateMockClientTcpNetworkAddress(uint16_t mockClientIdentifier) {
  // a zeroed device forwarderAddress tells the test set to use the given device forwarderAddress
  // device_address mockClientNetworkAddress({0, 0, 0, 0, 0, 0});
  device_address mockClientNetworkAddress({127, 0, 0, 1, 0, 0});
  return mockClientNetworkAddress;
}

std::vector<MqttSnClientNetworkTestValueParameter>
    clientNetworkTestParameter = getParameterMqttSnClientNetworkTestTypeParameter<generateMockClientTcpNetworkAddress,
                                                                                  MockClientLinuxTcpNetworkImplementation>(
    mqttSnGatewayClientNetworkTestConfiguration,
    mockClientTcpNetworkInterfaces
);

std::vector<MqttSnClientNetworkTcpNetworkDefragmentationTestParameter>
    clientNetworkDefragmentationTestParameter =
    getMqttSnClientNetworkTcpNetworkDefragmentationTestParameter<generateMockClientTcpNetworkAddress,
                                                                 MockClientLinuxTcpNetworkImplementation>(
        mqttSnGatewayClientNetworkTestConfiguration,
        mockClientTcpNetworkInterfaces
    );

struct PrintToStringMqttSnClientTcpNetworkTestValueParameterParamName {
  template<class ParamType>
  std::string operator()(const ::testing::TestParamInfo<ParamType> &info) const {
    std::string result = "Tcp" + std::to_string(info.index);
    std::stringstream buffer;
    buffer << info.param;
    result.append(buffer.str());
    return result;
  }
};

INSTANTIATE_TEST_SUITE_P(SendReceiveTests,
    MqttSnClientNetworkInterfaceTests,
    ::testing::ValuesIn(clientNetworkTestParameter.begin(), clientNetworkTestParameter.end()),
    PrintToStringMqttSnClientTcpNetworkTestValueParameterParamName()
);

INSTANTIATE_TEST_SUITE_P(DefragmentationTests,
    MqttSnClientNetworkInterfaceMessageDefragmentationTests,
    ::testing::ValuesIn(clientNetworkDefragmentationTestParameter.begin(),
                        clientNetworkDefragmentationTestParameter.end()),
    PrintToStringMqttSnClientTcpNetworkTestValueParameterParamName()
);

#endif //CMQTTSNFORWARDER_TCPMQTTSNCLIENTNETWORKINTERFACETESTS_H
