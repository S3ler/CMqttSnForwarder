//
// Created by SomeDude on 25.03.2019.
//

#ifndef CMQTTSNFORWARDER_GETMQTTSNCLIENTNETWORKTCPNETWORKDEFRAGMENTATIONTESTPARAMETER_H
#define CMQTTSNFORWARDER_GETMQTTSNCLIENTNETWORKTCPNETWORKDEFRAGMENTATIONTESTPARAMETER_H

#include <TestConfigurations/CartesianProductTestCaseGenerator.h>
#include "MqttSnClientNetworkTcpNetworkDefragmentationTestParameter.h"


template<MockClientNetworkAddressGenerator generateNetworkAddress, class MockClientNetworkInterfaceType>
::std::vector<MqttSnClientNetworkTcpNetworkDefragmentationTestParameter> getMqttSnClientNetworkTcpNetworkDefragmentationTestParameter(
    MqttSnGatewayClientNetworkTestConfiguration &mqttSnClientNetworkTestFixture,
    std::vector<std::shared_ptr<MockClientNetworkInterfaceType>> &mockClientNetworkInterfaces);

template<MockClientNetworkAddressGenerator generateNetworkAddress, class MockClientNetworkInterfaceType>
::std::vector<MqttSnClientNetworkTcpNetworkDefragmentationTestParameter> getMqttSnClientNetworkTcpNetworkDefragmentationTestParameter(
    MqttSnGatewayClientNetworkTestConfiguration &mqttSnClientNetworkTestFixture,
    std::vector<std::shared_ptr<MockClientNetworkInterfaceType>> &mockClientNetworkInterfaces) {
  ::std::vector<MqttSnClientNetworkTcpNetworkDefragmentationTestParameter> result;

  uint16_t min_message_length = CMQTTSNFORWARDER_CARTESIANTESTGENERATOR_MIN_MESSAGE_LENGTH;
  uint16_t message_length_step_count = 6;
  if (mqttSnClientNetworkTestFixture.useIdentifier && min_message_length < 3) {
    min_message_length = 3;
  }

  uint16_t message_length_steps =
      static_cast<uint16_t>((MAX_MESSAGE_LENGTH - min_message_length) / message_length_step_count);
  uint16_t max_message_length = MAX_MESSAGE_LENGTH;

  uint16_t min_client_count = 1;
  uint16_t max_client_count = 10;

  uint16_t min_message_count = 1;
  uint16_t max_message_counter = 10;

  uint16_t packet_size = 512; // TODO make more variantion

  for (uint16_t client_count = min_client_count; client_count <= max_client_count; client_count+=2) {

    for (uint16_t message_count = min_message_count; message_count <= max_message_counter; message_count+=2) {
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
        MqttSnClientNetworkTcpNetworkDefragmentationTestParameter mqttSnClientNetworkTestTypeParameter
            (mockClientIdentifiers, message_count, message_length, packet_size, mqttSnClientNetworkTestFixture);
        result.push_back(mqttSnClientNetworkTestTypeParameter);
      }
    }
  }
  return result;
}


#endif //CMQTTSNFORWARDER_GETMQTTSNCLIENTNETWORKTCPNETWORKDEFRAGMENTATIONTESTPARAMETER_H
