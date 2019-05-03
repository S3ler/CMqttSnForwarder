//
// Created by SomeDude on 06.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERTESTGLOBALVARIABLES_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDERTESTGLOBALVARIABLES_H

#include <test/shared/MockMqttSnClientNetwork/ClientNetworkMock.h>
#include <test/shared/MockMqttSnGatewayNetwork/GatewayNetworkMock.h>
#include <test/shared/MockMqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferMock.h>

ClientNetworkMock *globalClientNetworkMockObj = nullptr;
GatewayNetworkMock *globalGatewayNetworkMockObj = nullptr;

MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock = nullptr;
std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap = nullptr;

#endif //CMQTTSNFORWARDER_MQTTSNFORWARDERTESTGLOBALVARIABLES_H
