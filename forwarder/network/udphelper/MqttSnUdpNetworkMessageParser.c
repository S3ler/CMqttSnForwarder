//
// Created by SomeDude on 16.03.2019.
//

#include "MqttSnUdpNetworkMessageParser.h"
#include <string.h>
#include <network/gateway/udp/MqttSnGatewayUdpNetwork.h>

int save_udp_messages_into_receive_buffer(uint8_t *buffer,
                                          ssize_t read_bytes,
                                          device_address gateway_address,
                                          MqttSnFixedSizeRingBuffer *receiveBuffer) {

  if (isThreeBytesUdpHeader(buffer, read_bytes) &&
      !isCompleteThreeBytesUdpHeader(buffer, read_bytes)) {
    return 0;
  }

  if (get_udp_message_length(buffer) < 3) {
    return 0;
  }

  if (save_complete_new_udp_message(buffer,
                                    read_bytes,
                                    gateway_address,
                                    receiveBuffer) == 0) {
    return 0;
  }
  return 0;
}

uint16_t get_udp_message_length(uint8_t *data) {
  MqttSnMessageHeaderThreeOctetsLengthField
      *threeOctetsLengthField = (MqttSnMessageHeaderThreeOctetsLengthField *) data;
  if (threeOctetsLengthField->three_octets_length_field_indicator == 0x01) {
    uint16_t length = 0;
    length += (((uint16_t) threeOctetsLengthField->msb_length) << 8);
    length += (((uint16_t) threeOctetsLengthField->lsb_length) << 0);
    return length;
  }
  MqttSnMessageHeaderOneOctetLengthField
      *oneOctetLengthField = (MqttSnMessageHeaderOneOctetLengthField *) data;
  return oneOctetLengthField->length;
}

int isCompleteThreeBytesUdpHeader(uint8_t *data, ssize_t data_length) {
  MqttSnMessageHeaderThreeOctetsLengthField
      *threeOctetsLengthField = (MqttSnMessageHeaderThreeOctetsLengthField *) data;
  if (threeOctetsLengthField->three_octets_length_field_indicator == 0x01 && data_length > 2) {
    return 1;
  }
  return 0;
}

int isThreeBytesUdpHeader(uint8_t *data, ssize_t data_length) {
  MqttSnMessageHeaderThreeOctetsLengthField
      *threeOctetsLengthField = (MqttSnMessageHeaderThreeOctetsLengthField *) data;
  if (threeOctetsLengthField->three_octets_length_field_indicator == 0x01) {
    return 1;
  }
  return 0;
}


int save_complete_new_udp_message(uint8_t *data,
                                  ssize_t data_length,
                                  device_address address,
                                  MqttSnFixedSizeRingBuffer *receiveBuffer) {
  if (data_length != get_udp_message_length(data)) {
    return -1;
  }
  save_udp_message_into_receive_buffer(data, (uint16_t) data_length, address, receiveBuffer);
  return 0;
}

int save_udp_message_into_receive_buffer(uint8_t *data,
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
