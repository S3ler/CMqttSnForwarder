//
// Created by SomeDude on 16.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNTCPNETWORKMESSAGEPARSER_H
#define CMQTTSNFORWARDER_MQTTSNTCPNETWORKMESSAGEPARSER_H

#include <stdint.h>
#include <global_defines.h>
#include <stdio.h>
#include <MqttSnFixedSizeRingBuffer.h>
#include <network/client/tcp/MqttSnClientTcpNetwork.h>

#pragma pack(push)
#pragma pack(1)
typedef struct MqttSnMessageHeaderOneOctetLengthField {
  uint8_t length;
  uint8_t msg_type;
} MqttSnMessageHeaderOneOctetLengthField;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct MqttSnMessageHeaderThreeOctetsLengthField {
  uint8_t three_octets_length_field_indicator;
  uint8_t msb_length;
  uint8_t lsb_length;
  uint8_t msg_type;
} MqttSnMessageHeaderThreeOctetsLengthField;
#pragma pack(pop)

int save_messages_into_receive_buffer(uint8_t *data,
                                      ssize_t data_length,
                                      device_address address,
                                      uint8_t *client_buffer,
                                      uint16_t *client_buffer_bytes,
                                      MqttSnFixedSizeRingBuffer *receiveBuffer);

uint16_t get_message_length(uint8_t *data);

int isCompleteThreeBytesHeader(uint8_t *data, ssize_t data_length);

int isThreeBytesHeader(uint8_t *data, ssize_t data_length);

int save_received_messages_from_tcp_socket_into_receive_buffer(MqttSnClientTcpNetwork *clientTcpNetwork,
                                                               MqttSnFixedSizeRingBuffer *receiveBuffer,
                                                               int client_socket_position);

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
