//
// Created by bele on 26.02.19.
//

#ifndef CMQTTSNFORWARDER_CARTESIANTESTGENERATOR_H
#define CMQTTSNFORWARDER_CARTESIANTESTGENERATOR_H

#include "../../../../forwarder/global_defines.h"
#include "MqttSnClientNetworkTestValueParameter.h"

#define CMQTTSNFORWARDER_CARTESIANTESTGENERATOR_MIN_MESSAGE_LENGTH 2

typedef device_address(*MockClientNetworkAddressGenerator)(uint16_t);
template<MockClientNetworkAddressGenerator generateNetworkAddress, class MockClientNetworkInterfaceType>
::std::vector<MqttSnClientNetworkTestValueParameter> getParameterMqttSnClientNetworkTestTypeParameter(
    MqttSnGatewayClientNetworkTestConfiguration &mqttSnClientNetworkTestFixture,
    std::vector<std::shared_ptr<MockClientNetworkInterfaceType>> &mockClientNetworkInterfaces);

#endif //CMQTTSNFORWARDER_CARTESIANTESTGENERATOR_H
