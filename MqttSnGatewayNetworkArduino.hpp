//
// Created by SomeDude on 25.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKARDUINO_HPP
#define CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKARDUINO_HPP

// shared
#include <MqttSnSharedNetwork.hpp>

// gateway network
#include <network/MqttSnGatewayNetworkInterface.c>
#include <network/MqttSnGatewayNetworkInterface.h>
#ifdef WITH_ARDUINO_GATEWAY_NETWORK_UDP
#include <network/arduino/gateway/ip/udp/MqttSnGatewayUdpNetwork.cpp>
#include <network/arduino/gateway/ip/udp/MqttSnGatewayUdpNetwork.hpp>
#include <network/arduino/gateway/ip/udp/MqttSnArduinoGatewayUdpNetworkContainer.cpp>
#include <network/arduino/gateway/ip/udp/MqttSnArduinoGatewayUdpNetworkContainer.hpp>
#endif

#endif  //CMQTTSNFORWARDER_MQTTSNGATEWAYNETWORKARDUINO_HPP
