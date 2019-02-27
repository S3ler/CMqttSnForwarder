//
// Created by bele on 27.02.19.
//

#ifndef CMQTTSNFORWARDER_GETPARAMETERMQTTSNGATEWAYNETWORKTESTTYPEPARAMETER_H
#define CMQTTSNFORWARDER_GETPARAMETERMQTTSNGATEWAYNETWORKTESTTYPEPARAMETER_H

#include "../../../../forwarder/global_defines.h"
#include "MqttSnGatewayNetworkValueParameter.h"
#include <vector>

typedef device_address(*MockGatewayNetworkAddressGenerator)(uint16_t);
template<MockGatewayNetworkAddressGenerator generateNetworkAddress, class MockClientNetworkInterfaceType>
::std::vector<MqttSnGatewayNetworkValueParameter> getParameterMqttSnGatewayNetworkTestTypeParameter(
    MqttSnForwarderGatewayNetworkTestConfiguration &mqttSnClientNetworkTestFixture,
    std::vector<std::shared_ptr<MockClientNetworkInterfaceType>> &mockGatewayNetworkInterfaces);

#endif //CMQTTSNFORWARDER_GETPARAMETERMQTTSNGATEWAYNETWORKTESTTYPEPARAMETER_H
