//
// Created by bele on 27.02.19.
//

#include "GetParameterMqttSnGatewayNetworkTestTypeParameter.h"

template<MockGatewayNetworkAddressGenerator generateNetworkAddress, class MockGatewayNetworkInterfaceType>
::std::vector<MqttSnGatewayNetworkValueParameter> getParameterMqttSnGatewayNetworkTestTypeParameter(
    MqttSnForwarderGatewayNetworkTestConfiguration &mqttSnClientNetworkTestFixture,
    std::vector<std::shared_ptr<MockGatewayNetworkInterfaceType>> &mockGatewayNetworkInterfaces) {
  std::vector<MqttSnGatewayNetworkValueParameter> result;

  uint16_t min_message_length = CMQTTSNFORWARDER_CARTESIANTESTGENERATOR_GATEWAY_NETWORK_MIN_MESSAGE_LENGTH;
  uint16_t message_length_step_count = 6;
  if (mqttSnClientNetworkTestFixture.useIdentifier && min_message_length < 3) {
    min_message_length = 3;
  }

  uint16_t message_length_steps =
      static_cast<uint16_t>((MAX_MESSAGE_LENGTH - min_message_length) / message_length_step_count);
  uint16_t max_message_length = MAX_MESSAGE_LENGTH;

  uint16_t min_gateway_count = 1;
  uint16_t max_gateway_count = 2;

  uint16_t min_message_count = 1;
  uint16_t max_message_counter = 10;

  for (uint16_t gateway_count = min_gateway_count; gateway_count <= max_gateway_count; ++gateway_count) {

    for (uint16_t message_count = min_message_count; message_count <= max_message_counter; ++message_count) {
      for (uint16_t message_length = min_message_length; message_length <= max_message_length;
           message_length += message_length_steps) {
        std::vector<MockGatewayConfiguration> mockGatewayConfigurations;

        for (uint16_t gateway_identifier = 0; gateway_identifier < gateway_count; ++gateway_identifier) {
          std::shared_ptr<MockGatewayNetworkInterfaceType>
              mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);

          mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);

          MockGatewayConfiguration mockGatewayConfiguration(generateNetworkAddress(gateway_identifier),
                                                            gateway_identifier,
                                                            mockGatewayNetworkInterface.get());

          mockGatewayConfigurations.push_back(mockGatewayConfiguration);

        }
        MqttSnGatewayNetworkValueParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                              message_count,
                                                                              message_length,
                                                                              mqttSnClientNetworkTestFixture,
                                                                              mockGatewayConfigurations.front().address);
        result.push_back(mqttSnGatewayNetworkValueParameter);
      }
    }
  }

  return result;
}