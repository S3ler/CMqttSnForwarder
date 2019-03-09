//
// Created by bele on 26.02.19.
//

#include "CartesianProductTestCaseGenerator.h"

typedef device_address(*MockClientNetworkAddressGenerator)(uint16_t);
template<MockClientNetworkAddressGenerator generateNetworkAddress, class MockClientNetworkInterfaceType>
::std::vector<MqttSnClientNetworkTestValueParameter> getParameterMqttSnClientNetworkTestTypeParameter(
    MqttSnGatewayClientNetworkTestConfiguration &mqttSnClientNetworkTestFixture,
    std::vector<std::shared_ptr<MockClientNetworkInterfaceType>> &mockClientNetworkInterfaces) {
  ::std::vector<MqttSnClientNetworkTestValueParameter> result;

  uint16_t min_message_length = 1;
  uint16_t max_message_length = MAX_MESSAGE_LENGTH;

  uint16_t min_client_count = 1;
  uint16_t max_client_count = 10;

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