//
// Created by SomeDude on 16.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNUDPNETWORKMESSAGEPARSER_H
#define CMQTTSNFORWARDER_MQTTSNUDPNETWORKMESSAGEPARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <forwarder/global_defines.h>
#include <forwarder/MqttSnFixedSizeRingBuffer.h>
#include <stdint.h>
#include <unistd.h>

int save_udp_messages_into_receive_buffer(uint8_t *buffer,
                                          ssize_t read_bytes,
                                          uint8_t broadcast_radius,
                                          device_address gateway_address,
                                          MqttSnFixedSizeRingBuffer *receiveBuffer);

uint16_t get_udp_message_length(uint8_t *data);

int isCompleteThreeBytesUdpHeader(uint8_t *data, ssize_t data_length);

int isThreeBytesUdpHeader(uint8_t *data, ssize_t data_length);

int save_complete_new_udp_message(uint8_t *data,
                                  ssize_t data_length,
                                  uint8_t broadcast_radius,
                                  device_address address,
                                  MqttSnFixedSizeRingBuffer *receiveBuffer);

int save_udp_message_into_receive_buffer(uint8_t *data,
                                         uint16_t data_length,
                                         uint8_t broadcast_radius,
                                         device_address address,
                                         MqttSnFixedSizeRingBuffer *receiveBuffer);

int initialize_udp_socket(uint16_t port);

ssize_t send_udp_message(int socket_fd, const device_address *to, const uint8_t *data, uint16_t data_length);

/*
 * 
 *
 * @return -1 on error, 1 if message is receive, 0 if no message is received.
 */
int is_udp_message_received(int socket_fd, int timeout_ms);

int receive_udp_message(int socket_fd,
                        uint8_t *buffer,
                        ssize_t *read_bytes,
                        uint16_t buffer_max_length,
                        device_address *from);

int receive_and_save_udp_message_into_receive_buffer(int socket_fd,
                                                     MqttSnFixedSizeRingBuffer *receiveBuffer,
                                                     uint16_t max_data_length);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNUDPNETWORKMESSAGEPARSER_H
