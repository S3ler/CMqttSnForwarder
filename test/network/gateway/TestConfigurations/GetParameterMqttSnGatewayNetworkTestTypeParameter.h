//
// Created by bele on 27.02.19.
//

#ifndef CMQTTSNFORWARDER_GETPARAMETERMQTTSNGATEWAYNETWORKTESTTYPEPARAMETER_H
#define CMQTTSNFORWARDER_GETPARAMETERMQTTSNGATEWAYNETWORKTESTTYPEPARAMETER_H

#include "../../../../platform/device_address.h"
#include "MqttSnGatewayNetworkValueParameter.h"
#include <vector>

#define CMQTTSNFORWARDER_CARTESIANTESTGENERATOR_GATEWAY_NETWORK_MIN_MESSAGE_LENGTH 3

typedef device_address(*MockGatewayNetworkAddressGenerator)(uint16_t);

template<MockGatewayNetworkAddressGenerator generateNetworkAddress, class MockGatewayNetworkInterfaceType>
::std::vector<MqttSnGatewayNetworkValueParameter> getParameterMqttSnGatewayNetworkTestTypeParameter(
    MqttSnForwarderGatewayNetworkTestConfiguration &mqttSnClientNetworkTestFixture,
    std::vector<std::shared_ptr<MockGatewayNetworkInterfaceType>> &mockGatewayNetworkInterfaces);

#endif //CMQTTSNFORWARDER_GETPARAMETERMQTTSNGATEWAYNETWORKTESTTYPEPARAMETER_H
