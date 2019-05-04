//
// Created by SomeDude on 16.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNTCPNETWORKMESSAGEPARSER_H
#define CMQTTSNFORWARDER_MQTTSNTCPNETWORKMESSAGEPARSER_H

#include <forwarder/global_defines.h>
#include <forwarder/MqttSnFixedSizeRingBuffer.h>
#include <forwarder/MqttSnForwarderLogging.h>
#include <stdio.h>
#include <stdint.h>

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH
#define CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH 1024
#endif

#ifdef WITH_LOGGING

int log_select_error(const MqttSnLogger *logger);

#endif //WITH_LOGGING

int save_tcp_messages_into_receive_buffer(uint8_t *data,
                                          ssize_t data_length,
                                          device_address address,
                                          uint8_t *client_buffer,
                                          uint16_t *client_buffer_bytes,
                                          MqttSnFixedSizeRingBuffer *receiveBuffer);

uint16_t get_tcp_message_length(uint8_t *data);

int isCompleteThreeBytesTcpHeader(uint8_t *data, ssize_t data_length);

int isThreeBytesTcpHeader(uint8_t *data, ssize_t data_length);

int save_tcp_message_into_receive_buffer(uint8_t *data,
                                         uint16_t data_length,
                                         device_address address,
                                         MqttSnFixedSizeRingBuffer *receiveBuffer);

int save_complete_new_tcp_message(uint8_t *data,
                                  ssize_t data_length,
                                  device_address address,
                                  uint8_t *client_buffer,
                                  uint16_t *client_buffer_bytes,
                                  MqttSnFixedSizeRingBuffer *receiveBuffer);

int save_incomplete_new_tcp_message(uint8_t *data,
                                    ssize_t data_length,
                                    device_address address,
                                    uint8_t *client_buffer,
                                    uint16_t *client_buffer_bytes,
                                    MqttSnFixedSizeRingBuffer *receiveBuffer);

int save_multiple_complete_new_tcp_messages(uint8_t *data,
                                            ssize_t data_length,
                                            device_address address,
                                            uint8_t *client_buffer,
                                            uint16_t *client_buffer_bytes,
                                            MqttSnFixedSizeRingBuffer *receiveBuffer);

int save_completed_tcp_message(uint8_t *data,
                               ssize_t data_length,
                               device_address address,
                               uint8_t *client_buffer,
                               uint16_t *client_buffer_bytes,
                               MqttSnFixedSizeRingBuffer *receiveBuffer);

int save_incomplete_tcp_message(uint8_t *data,
                                ssize_t data_length,
                                device_address address,
                                uint8_t *client_buffer,
                                uint16_t *client_buffer_bytes,
                                MqttSnFixedSizeRingBuffer *receiveBuffer);

int save_multiple_tcp_messages(uint8_t *data,
                               ssize_t data_length,
                               device_address address,
                               uint8_t *client_buffer,
                               uint16_t *client_buffer_bytes,
                               MqttSnFixedSizeRingBuffer *receiveBuffer);

#endif //CMQTTSNFORWARDER_MQTTSNTCPNETWORKMESSAGEPARSER_H
