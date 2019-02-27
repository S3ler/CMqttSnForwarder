//
// Created by bele on 27.02.19.
//

#include "GetParameterMqttSnGatewayNetworkTestTypeParameter.h"

template<MockGatewayNetworkAddressGenerator generateNetworkAddress, class MockGatewayNetworkInterfaceType>
::std::vector<MqttSnGatewayNetworkValueParameter> getParameterMqttSnGatewayNetworkTestTypeParameter(
    MqttSnForwarderGatewayNetworkTestConfiguration &mqttSnClientNetworkTestFixture,
    std::vector<std::shared_ptr<MockGatewayNetworkInterfaceType>> &mockGatewayNetworkInterfaces) {
  std::vector<MqttSnGatewayNetworkValueParameter> result;
  std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
  mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
  std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
  uint16_t p = 8888;
  device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
  MockGatewayConfiguration mockGatewayConfiguration(1,
                           mockGatewayDeviceAddress,
                           mockGatewayNetworkInterface.get());
  mockGatewayConfigurations.push_back(mockGatewayConfiguration);
  MqttSnGatewayNetworkValueParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                        1,
                                                                        1,
                                                                        mqttSnClientNetworkTestFixture,
                                                                        mockGatewayConfiguration.address);
  result.push_back(mqttSnGatewayNetworkValueParameter);
  return result;
}