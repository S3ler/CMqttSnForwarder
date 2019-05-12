//
// Created by bele on 26.02.19.
//

#ifndef CMQTTSNFORWARDER_CARTESIANTESTGENERATOR_H
#define CMQTTSNFORWARDER_CARTESIANTESTGENERATOR_H

#include "../../../../platform/device_address.h"
#include "MqttSnClientNetworkTestValueParameter.h"

#define CMQTTSNFORWARDER_CARTESIANTESTGENERATOR_MIN_MESSAGE_LENGTH 3

typedef device_address(*MockClientNetworkAddressGenerator)(uint16_t);
template<MockClientNetworkAddressGenerator generateNetworkAddress, class MockClientNetworkInterfaceType>
::std::vector<MqttSnClientNetworkTestValueParameter> getParameterMqttSnClientNetworkTestTypeParameter(
    MqttSnGatewayClientNetworkTestConfiguration &mqttSnClientNetworkTestFixture,
    std::vector<std::shared_ptr<MockClientNetworkInterfaceType>> &mockClientNetworkInterfaces);

typedef device_address(*MockClientNetworkAddressGenerator)(uint16_t);
template<MockClientNetworkAddressGenerator generateNetworkAddress, class MockClientNetworkInterfaceType>
::std::vector<MqttSnClientNetworkTestValueParameter> getParameterMqttSnClientNetworkTestTypeParameter(
    MqttSnGatewayClientNetworkTestConfiguration &mqttSnClientNetworkTestFixture,
    std::vector<std::shared_ptr<MockClientNetworkInterfaceType>> &mockClientNetworkInterfaces) {
  ::std::vector<MqttSnClientNetworkTestValueParameter> result;

  uint16_t min_message_length = CMQTTSNFORWARDER_CARTESIANTESTGENERATOR_MIN_MESSAGE_LENGTH;
  uint16_t message_length_step_count = 6;
  if (mqttSnClientNetworkTestFixture.useIdentifier && min_message_length < 3) {
    min_message_length = 3;
  }

  uint16_t message_length_steps =
      static_cast<uint16_t>((CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH - min_message_length) / message_length_step_count);
  uint16_t max_message_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;

  uint16_t min_client_count = 1;
  uint16_t max_client_count = 10;

  uint16_t min_message_count = 1;
  uint16_t max_message_counter = 10;

  for (uint16_t client_count = min_client_count; client_count <= max_client_count; client_count += 3) {

    for (uint16_t message_count = min_message_count; message_count <= max_message_counter; message_count += 3) {
      for (uint16_t message_length = min_message_length; message_length <= max_message_length;
           message_length += message_length_steps) {
        std::vector<MockClientConfiguration> mockClientIdentifiers;

        for (uint16_t i = 0; i < client_count; ++i) {
          std::shared_ptr<MockClientNetworkInterfaceType>
              mockClientNetworkInterfaceType(new MockClientNetworkInterfaceType);
          mockClientNetworkInterfaces.push_back(mockClientNetworkInterfaceType);
          MockClientConfiguration
              mockClientIdentifier(generateNetworkAddress(i), i, mockClientNetworkInterfaceType.get());
          // client_count-min_client_count => MockClient identfier start with 0
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

#endif //CMQTTSNFORWARDER_CARTESIANTESTGENERATOR_H
