//
// Created by SomeDude on 25.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKARDUINO_HPP
#define CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKARDUINO_HPP

// shared
#include <MqttSnSharedNetwork.hpp>

// client network
#include <network/MqttSnClientNetworkInterface.c>
#include <network/MqttSnClientNetworkInterface.h>
#ifdef WITH_ARDUINO_CLIENT_NETWORK_UDP
#include <network/arduino/client/ip/udp/MqttSnClientUdpNetwork.cpp>
#include <network/arduino/client/ip/udp/MqttSnClientUdpNetwork.hpp>
#include <network/arduino/client/ip/udp/MqttSnArduinoClientUdpNetworkContainer.cpp>
#include <network/arduino/client/ip/udp/MqttSnArduinoClientUdpNetworkContainer.hpp>
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKARDUINO_HPP
