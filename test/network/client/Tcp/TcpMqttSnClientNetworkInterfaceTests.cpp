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

typedef device_address(*MockClientNetworkAddressGenerator)(uint16_t);
template<MockClientNetworkAddressGenerator generateNetworkAddress, class MockClientNetworkInterfaceType>
::std::vector<MqttSnClientNetworkTestValueParameter> getParameterMqttSnClientNetworkTestTypeParameter(
    MqttSnGatewayClientNetworkTestConfiguration &mqttSnClientNetworkTestFixture,
    std::vector<std::shared_ptr<MockClientNetworkInterfaceType>> &mockClientNetworkInterfaces) {
  ::std::vector<MqttSnClientNetworkTestValueParameter> result;

  uint16_t min_message_length = 1;
  uint16_t max_message_length = MAX_MESSAGE_LENGTH;

  uint16_t min_client_count = 1;
  uint16_t max_client_count = CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_CLIENTS;

  uint16_t min_message_count = 1;
  uint16_t max_message_counter = 2;

  for (uint16_t client_count = min_client_count; client_count <= max_client_count; ++client_count) {

    for (uint16_t message_count = min_message_count; message_count <= max_message_counter; ++message_count) {
      for (uint16_t message_length = min_message_length; message_length <= max_message_length;
           message_length += 255 / 2) {
        std::vector<MockClientConfiguration> mockClientIdentifiers;

        for (uint16_t i = min_client_count; i <= client_count; ++i) {
          std::shared_ptr<MockClientNetworkInterfaceType>
              mockClientNetworkInterfaceType(new MockClientNetworkInterfaceType);
          mockClientNetworkInterfaces.push_back(mockClientNetworkInterfaceType);
          MockClientConfiguration
              mockClientIdentifier
              (generateNetworkAddress(client_count), client_count, mockClientNetworkInterfaceType.get());
          mockClientIdentifiers.push_back(mockClientIdentifier);
        }
        MqttSnClientNetworkTestValueParameter mqttSnClientNetworkTestTypeParameter
            (mockClientIdentifiers, message_count, message_length, mqttSnClientNetworkTestFixture);
        result.push_back(mqttSnClientNetworkTestTypeParameter);
      }
    }
  }
  return result;
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
