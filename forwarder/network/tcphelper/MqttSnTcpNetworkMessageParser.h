//
// Created by SomeDude on 16.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNTCPNETWORKMESSAGEPARSER_H
#define CMQTTSNFORWARDER_MQTTSNTCPNETWORKMESSAGEPARSER_H

#include <stdint.h>
#include <global_defines.h>
#include <stdio.h>
#include <MqttSnFixedSizeRingBuffer.h>
#include <MqttSnForwarderLogging.h>

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH
#define CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH 1024
#endif

// 1554750679: New client connection from 127.0.0.1 on port 1883 [34, 11].
// device_address, port [0 - 65535]
// LOG_LEVEL_DEFAULT
int log_new_tcp_connection(const MqttSnLogger *logger, int level, const device_address *address);

// 1554750679: Closed client connection from 127.0.0.1 on port 1883 [34, 11].
// 1554750679: Client 127.0.0.1 on port 1883 [34, 11] disconnected.
// device_address, port [0 - 65535]
// LOG_LEVEL_DEFAULT
int log_close_tcp_connection(const MqttSnLogger *logger, int level, const device_address *address);



int save_messages_into_receive_buffer(uint8_t *data,
                                      ssize_t data_length,
                                      device_address address,
                                      uint8_t *client_buffer,
                                      uint16_t *client_buffer_bytes,
                                      MqttSnFixedSizeRingBuffer *receiveBuffer);

uint16_t get_tcp_message_length(uint8_t *data);

int isCompleteThreeBytesTcpHeader(uint8_t *data, ssize_t data_length);

int isThreeBytesTcpHeader(uint8_t *data, ssize_t data_length);

int save_message_into_receive_buffer(uint8_t *data,
                                     uint16_t data_length,
                                     device_address address,
                                     MqttSnFixedSizeRingBuffer *receiveBuffer);

int save_complete_new_message(uint8_t *data,
                              ssize_t data_length,
                              device_address address,
                              uint8_t *client_buffer,
                              uint16_t *client_buffer_bytes,
                              MqttSnFixedSizeRingBuffer *receiveBuffer);

int save_incomplete_new_message(uint8_t *data,
                                ssize_t data_length,
                                device_address address,
                                uint8_t *client_buffer,
                                uint16_t *client_buffer_bytes,
                                MqttSnFixedSizeRingBuffer *receiveBuffer);

int save_multiple_complete_new_messages(uint8_t *data,
                                        ssize_t data_length,
                                        device_address address,
                                        uint8_t *client_buffer,
                                        uint16_t *client_buffer_bytes,
                                        MqttSnFixedSizeRingBuffer *receiveBuffer);

int save_completed_message(uint8_t *data,
                           ssize_t data_length,
                           device_address address,
                           uint8_t *client_buffer,
                           uint16_t *client_buffer_bytes,
                           MqttSnFixedSizeRingBuffer *receiveBuffer);

int save_incomplete_message(uint8_t *data,
                            ssize_t data_length,
                            device_address address,
                            uint8_t *client_buffer,
                            uint16_t *client_buffer_bytes,
                            MqttSnFixedSizeRingBuffer *receiveBuffer);

int save_multiple_messages(uint8_t *data,
                           ssize_t data_length,
                           device_address address,
                           uint8_t *client_buffer,
                           uint16_t *client_buffer_bytes,
                           MqttSnFixedSizeRingBuffer *receiveBuffer);

#endif //CMQTTSNFORWARDER_MQTTSNTCPNETWORKMESSAGEPARSER_H
