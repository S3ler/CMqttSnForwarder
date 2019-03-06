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

#else

extern ClientNetworkMock *globalClientNetworkMockObj;
extern GatewayNetworkMock *globalGatewayNetworkMockObj;

extern MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;
extern std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *>
    *globalMqttSnFixedSizeRingBufferMockMap;

#endif //CMQTTSNFORWARDER_MQTTSNFORWARDERTESTSGLOBALVARIABLES_H
