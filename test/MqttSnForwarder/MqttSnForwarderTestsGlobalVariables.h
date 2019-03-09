//
// Created by SomeDude on 06.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERTESTSGLOBALVARIABLES_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDERTESTSGLOBALVARIABLES_H

ClientNetworkMock *globalClientNetworkMockObj = nullptr;
GatewayNetworkMock *globalGatewayNetworkMockObj = nullptr;

MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock = nullptr;
std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap = nullptr;

#endif //CMQTTSNFORWARDER_MQTTSNFORWARDERTESTSGLOBALVARIABLES_H
