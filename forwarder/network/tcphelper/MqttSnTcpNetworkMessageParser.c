//
// Created by SomeDude on 16.03.2019.
//

#include <string.h>
#include "MqttSnTcpNetworkMessageParser.h"

uint16_t get_message_length(uint8_t *data) {
  MqttSnMessageHeaderThreeOctetsLengthField
      *threeOctetsLengthField = (MqttSnMessageHeaderThreeOctetsLengthField *) data;
  if (threeOctetsLengthField->three_octets_length_field_indicator == 0x01) {
    return threeOctetsLengthField->length;
  }
  MqttSnMessageHeaderOneOctetLengthField
      *oneOctetLengthField = (MqttSnMessageHeaderOneOctetLengthField *) data;
  return oneOctetLengthField->length;
}

int save_message_into_receive_buffer(uint8_t *data,
                                     uint16_t data_length,
                                     device_address address,
                                     MqttSnFixedSizeRingBuffer *receiveBuffer) {
  MqttSnMessageData receiveMessageData = {0};
  receiveMessageData.data_length = data_length;
  memcpy(receiveMessageData.data, data, receiveMessageData.data_length);
  receiveMessageData.address = address;
  put(receiveBuffer, &receiveMessageData);
  return 0;
}

int save_complete_new_message(uint8_t *data,
                              ssize_t data_length,
                              device_address address,
                              uint8_t *client_buffer,
                              uint16_t *client_buffer_bytes,
                              MqttSnFixedSizeRingBuffer *receiveBuffer) {
  if (data_length != get_message_length(data)) {
    return -1;
  }
  save_message_into_receive_buffer(data, (uint16_t) data_length, address, receiveBuffer);
  memset(client_buffer, 0, CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH);
  *client_buffer_bytes = 0;
  return 0;
}

int save_incomplete_new_message(uint8_t *data,
                                ssize_t data_length,
                                device_address address,
                                uint8_t *client_buffer,
                                uint16_t *client_buffer_bytes,
                                MqttSnFixedSizeRingBuffer *receiveBuffer) {
  if (data_length < get_message_length(data)) {
    return -1;
  }
  memcpy(client_buffer, data, (size_t) data_length);
  *client_buffer_bytes = (uint16_t) data_length;
  return 0;
}

int save_multiple_new_messages(uint8_t *data,
                               ssize_t data_length,
                               device_address address,
                               uint8_t *client_buffer,
                               uint16_t *client_buffer_bytes,
                               MqttSnFixedSizeRingBuffer *receiveBuffer) {
  if (data_length < get_message_length(data)) {
    return -1;
  }
  ssize_t read_bytes = 0;
  int16_t message_length = get_message_length(data);
  for (; data_length >= read_bytes + message_length;
         read_bytes += message_length, message_length = get_message_length(data)) {
    if (save_complete_new_message(data + read_bytes,
                                  message_length,
                                  address,
                                  client_buffer,
                                  client_buffer_bytes,
                                  receiveBuffer) == 0) {
      continue;
    } else {
      return -1;
    }
  }
  if (data_length == read_bytes) {
    // only complete messages in buffer, all messages saved
    return 0;
  }

  if (save_incomplete_new_message(data + read_bytes,
                                  data_length - read_bytes,
                                  address,
                                  client_buffer,
                                  client_buffer_bytes,
                                  receiveBuffer) == 0) {
    return 0;
  }
  return -1;
}

int save_completed_message(uint8_t *data,
                           ssize_t data_length,
                           device_address address,
                           uint8_t *client_buffer,
                           uint16_t *client_buffer_bytes,
                           MqttSnFixedSizeRingBuffer *receiveBuffer) {
  if (*client_buffer_bytes + data_length != get_message_length(client_buffer)) {
    return -1;
  }
  memcpy(client_buffer + *client_buffer_bytes, data, (size_t) data_length);
  *client_buffer_bytes += (uint16_t) data_length;
  save_message_into_receive_buffer(client_buffer, (uint16_t) *client_buffer_bytes, address, receiveBuffer);
  memset(client_buffer, 0, CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH);
  *client_buffer_bytes = 0;
  return 0;
}

int save_incomplete_message(uint8_t *data,
                            ssize_t data_length,
                            device_address address,
                            uint8_t *client_buffer,
                            uint16_t *client_buffer_bytes,
                            MqttSnFixedSizeRingBuffer *receiveBuffer) {
  if (*client_buffer_bytes + data_length < get_message_length(client_buffer)) {
    return -1;
  }
  memcpy(client_buffer + *client_buffer_bytes, data, (size_t) data_length);
  *client_buffer_bytes += (uint16_t) data_length;
  return 0;
}

int save_multiple_messages(uint8_t *data,
                           ssize_t data_length,
                           device_address address,
                           uint8_t *client_buffer,
                           uint16_t *client_buffer_bytes,
                           MqttSnFixedSizeRingBuffer *receiveBuffer) {
  if (*client_buffer_bytes + data_length > get_message_length(client_buffer)) {
    return -1;
  }
  uint16_t completed_message_bytes = get_message_length(client_buffer) - *client_buffer_bytes;

  if (save_completed_message(data,
                             get_message_length(client_buffer) - *client_buffer_bytes,
                             address,
                             client_buffer,
                             client_buffer_bytes,
                             receiveBuffer) != 0) {
    return -1;
  }
  if (save_multiple_messages(data + data_length,
                             data_length - (ssize_t) completed_message_bytes,
                             address,
                             client_buffer,
                             client_buffer_bytes,
                             receiveBuffer) == 0) {
    return -1;
  }
  return 0;
}
