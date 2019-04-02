//
// Created by SomeDude on 16.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNUDPNETWORKMESSAGEPARSER_H
#define CMQTTSNFORWARDER_MQTTSNUDPNETWORKMESSAGEPARSER_H

#include <stdint.h>
#include <global_defines.h>
#include <MqttSnFixedSizeRingBuffer.h>
#include <unistd.h>

int save_udp_messages_into_receive_buffer(uint8_t *buffer,
                                          ssize_t read_bytes,
                                          device_address gateway_address,
                                          MqttSnFixedSizeRingBuffer *receiveBuffer);

uint16_t get_udp_message_length(uint8_t *data);

int isCompleteThreeBytesUdpHeader(uint8_t *data, ssize_t data_length);

int isThreeBytesUdpHeader(uint8_t *data, ssize_t data_length);

int save_complete_new_udp_message(uint8_t *data,
                                  ssize_t data_length,
                                  device_address address,
                                  MqttSnFixedSizeRingBuffer *receiveBuffer);

int save_udp_message_into_receive_buffer(uint8_t *data,
                                         uint16_t data_length,
                                         device_address address,
                                         MqttSnFixedSizeRingBuffer *receiveBuffer);

#endif //CMQTTSNFORWARDER_MQTTSNUDPNETWORKMESSAGEPARSER_H
