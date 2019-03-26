//
// Created by bele on 27.02.19.
//

#include "GetMqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter.h"


template<MockGatewayNetworkAddressGenerator generateNetworkAddress, class MockGatewayNetworkInterfaceType>
::std::vector<MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter> getMqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter1(
    MqttSnForwarderGatewayNetworkTestConfiguration &mqttSnClientNetworkTestFixture,
    std::vector<std::shared_ptr<MockGatewayNetworkInterfaceType>> &mockGatewayNetworkInterfaces) {
  std::vector<MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter> result;
  uint16_t packetSize = 1024;
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  2,
                                                                                                  3,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  1,
                                                                                                  3,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  7,
                                                                                                  3,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  8,
                                                                                                  3,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  9,
                                                                                                  3,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  10,
                                                                                                  3,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  11,
                                                                                                  3,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  12,
                                                                                                  3,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  20,
                                                                                                  3,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  1,
                                                                                                  255,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  2,
                                                                                                  255,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  3,
                                                                                                  255,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  4,
                                                                                                  255,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  7,
                                                                                                  255,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  8,
                                                                                                  255,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  9,
                                                                                                  255,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  10,
                                                                                                  255,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  11,
                                                                                                  255,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  12,
                                                                                                  255,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  13,
                                                                                                  255,
                                                                                                  packetSize,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  2,
                                                                                                  512,
                                                                                                  509,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  2,
                                                                                                  512,
                                                                                                  510,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  2,
                                                                                                  512,
                                                                                                  511,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  2,
                                                                                                  512,
                                                                                                  512,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  2,
                                                                                                  512,
                                                                                                  513,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  2,
                                                                                                  512,
                                                                                                  514,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(
        mockGatewayDeviceAddress, 1,
        mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                                                  2,
                                                                                                  512,
                                                                                                  515,
                                                                                                  mqttSnClientNetworkTestFixture,
                                                                                                  mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
/*
 *
  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(1,
                                                      mockGatewayDeviceAddress,
                                                      mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                          100,
                                                                          3,
                                                                          packetSize,
                                                                          mqttSnClientNetworkTestFixture,
                                                                          mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }

  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(1,
                                                      mockGatewayDeviceAddress,
                                                      mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                          1,
                                                                          255,
                                                                          packetSize,
                                                                          mqttSnClientNetworkTestFixture,
                                                                          mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }

  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(1,
                                                      mockGatewayDeviceAddress,
                                                      mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                          10,
                                                                          255,
                                                                          packetSize,
                                                                          mqttSnClientNetworkTestFixture,
                                                                          mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }

  {
    std::shared_ptr<MockGatewayNetworkInterfaceType> mockGatewayNetworkInterface(new MockGatewayNetworkInterfaceType);
    mockGatewayNetworkInterfaces.push_back(mockGatewayNetworkInterface);
    std::vector<MockGatewayConfiguration> mockGatewayConfigurations;
    uint16_t p = 8888;
    device_address mockGatewayDeviceAddress({127, 0, 0, 1, (uint8_t) (p >> 8), (uint8_t) (p >> 0)});
    MockGatewayConfiguration mockGatewayConfiguration(1,
                                                      mockGatewayDeviceAddress,
                                                      mockGatewayNetworkInterface.get());
    mockGatewayConfigurations.push_back(mockGatewayConfiguration);
    MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter mqttSnGatewayNetworkValueParameter(mockGatewayConfigurations,
                                                                          100,
                                                                          255,
                                                                          packetSize,
                                                                          mqttSnClientNetworkTestFixture,
                                                                          mockGatewayConfiguration.address);
    result.push_back(mqttSnGatewayNetworkValueParameter);
  }
  */
  return result;
}
