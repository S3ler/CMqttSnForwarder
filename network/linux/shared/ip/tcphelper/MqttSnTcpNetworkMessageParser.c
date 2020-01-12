//
// Created by SomeDude on 16.03.2019.
//

#include "MqttSnTcpNetworkMessageParser.h"
#include <parser/MqttSnMessageParser.h>
#include <string.h>
#include <errno.h>
#include <network/linux/shared/ip/MqttSnIpNetworkHelper.h>
#include <unistd.h>

/*
// TODO rewrite: -1 on error 0 if nothing, else number of messages
int save_tcp_messages_into_receive_buffer(uint8_t *data,
                                          ssize_t data_length,
                                          device_address address,
                                          uint8_t *client_buffer,
                                          uint16_t *client_buffer_bytes,
                                          MqttSnFixedSizeRingBuffer *receiveBuffer) {
  if (client_buffer[0] == 0) {
    // no message in buffer, new message, read message bytes
    if (save_incomplete_new_tcp_message(data,
                                        data_length,
                                        address,
                                        client_buffer,
                                        client_buffer_bytes,
                                        receiveBuffer) == 0) {
      return 0;
    }
    if (save_multiple_complete_new_tcp_messages(data,
                                                data_length,
                                                address,
                                                client_buffer,
                                                client_buffer_bytes,
                                                receiveBuffer) == 0) {
      return 0;
    }
    return -1;
  }
  if (isThreeBytesTcpHeader(client_buffer, *client_buffer_bytes) &&
      !isCompleteThreeBytesTcpHeader(client_buffer, *client_buffer_bytes)) {
    if (*client_buffer_bytes == 1) {
      if (data_length == 1) {
        // copy and increase - then we are done
        client_buffer[*client_buffer_bytes] = data[0];
        *client_buffer_bytes += 1;
        return 0;
      } else if (data_length == 2) {
        // copy and increase - then we are done
        client_buffer[*client_buffer_bytes] = data[0];
        client_buffer[*client_buffer_bytes + 1] = data[1];
        *client_buffer_bytes += 2;
        return 0;
      } else {
        // copy into buffer but do not increase - rest ist handled in the following methods
        client_buffer[*client_buffer_bytes] = data[0];
        client_buffer[*client_buffer_bytes + 1] = data[1];
      }
    } else if (*client_buffer_bytes == 2) {
      if (data_length == 1) {
        // copy and increase - then we are done
        client_buffer[*client_buffer_bytes] = data[0];
        *client_buffer_bytes += 1;
        return 0;
      } else {
        // copy into buffer but do not increase - rest ist handled in the following methods
        client_buffer[*client_buffer_bytes] = data[0];
        client_buffer[*client_buffer_bytes + 1] = data[1];
      }
    }

  }
  // messages already in buffer, new message, read message bytes
  if (save_incomplete_tcp_message(data,
                                  data_length,
                                  address,
                                  client_buffer,
                                  client_buffer_bytes,
                                  receiveBuffer) == 0) {
    return 0;
  }
  if (save_multiple_tcp_messages(data,
                                 data_length,
                                 address,
                                 client_buffer,
                                 client_buffer_bytes,
                                 receiveBuffer) == 0) {
    return 0;
  }
  return -1;
}
*/
uint16_t get_tcp_message_length(uint8_t *data) {
  return get_message_length((data));
}

int isCompleteThreeBytesTcpHeader(uint8_t *data, ssize_t data_length) {
  MqttSnMessageHeaderThreeOctetsLengthField
      *threeOctetsLengthField = (MqttSnMessageHeaderThreeOctetsLengthField *) data;
  if (threeOctetsLengthField->indicator == 0x01 && data_length > 2) {
    return 1;
  }
  return 0;
}

int isCompleteTwoBytesTcpHeader(uint8_t *data, ssize_t data_length) {
  return data_length > 1;
}

int isThreeBytesTcpHeader(uint8_t *data, ssize_t data_length) {
  MqttSnMessageHeaderThreeOctetsLengthField
      *threeOctetsLengthField = (MqttSnMessageHeaderThreeOctetsLengthField *) data;
  if (threeOctetsLengthField->indicator == 0x01) {
    return 1;
  }
  return 0;
}
/*
int save_tcp_message_into_receive_buffer(uint8_t *data,
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
*/
/*
int save_complete_new_tcp_message(uint8_t *data,
                                  ssize_t data_length,
                                  device_address address,
                                  uint8_t *client_buffer,
                                  uint16_t *client_buffer_bytes,
                                  MqttSnFixedSizeRingBuffer *receiveBuffer) {
  if (data_length != get_tcp_message_length(data)) {
    return -1;
  }
  save_tcp_message_into_receive_buffer(data, (uint16_t) data_length, address, receiveBuffer);
  memset(client_buffer, 0, CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH);
  *client_buffer_bytes = 0;
  return 0;
}
*/
/*
int save_incomplete_new_tcp_message(uint8_t *data,
                                    ssize_t data_length,
                                    device_address address,
                                    uint8_t *client_buffer,
                                    uint16_t *client_buffer_bytes,
                                    MqttSnFixedSizeRingBuffer *receiveBuffer) {
  if (isThreeBytesTcpHeader(data, data_length)) {
    if (isCompleteThreeBytesTcpHeader(data, data_length) && (data_length >= get_tcp_message_length(data))) {
      return -1;
    }
  } else {
    if (data_length >= get_tcp_message_length(data)) {
      return -1;
    }
  }

  memcpy(client_buffer, data, (size_t) data_length);
  *client_buffer_bytes = (uint16_t) data_length;
  return 0;
}
*/
/*
int save_multiple_complete_new_tcp_messages(uint8_t *data,
                                            ssize_t data_length,
                                            device_address address,
                                            uint8_t *client_buffer,
                                            uint16_t *client_buffer_bytes,
                                            MqttSnFixedSizeRingBuffer *receiveBuffer) {
  if (data_length < get_tcp_message_length(data)) {
    return -1;
  }
  ssize_t read_bytes = 0;
  uint16_t message_length = get_tcp_message_length(data);
  for (; data_length >= read_bytes + message_length;
         read_bytes += message_length, message_length = get_tcp_message_length(data)) {
    if (save_complete_new_tcp_message(data + read_bytes,
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

  if (save_incomplete_new_tcp_message(data + read_bytes,
                                      data_length - read_bytes,
                                      address,
                                      client_buffer,
                                      client_buffer_bytes,
                                      receiveBuffer) == 0) {
    return 0;
  }
  return -1;
}
*/
/*
int save_completed_tcp_message(uint8_t *data,
                               ssize_t data_length,
                               device_address address,
                               uint8_t *client_buffer,
                               uint16_t *client_buffer_bytes,
                               MqttSnFixedSizeRingBuffer *receiveBuffer) {
  if (*client_buffer_bytes + data_length != get_tcp_message_length(client_buffer)) {
    return -1;
  }
  memcpy(client_buffer + *client_buffer_bytes, data, (size_t) data_length);
  *client_buffer_bytes += (uint16_t) data_length;
  save_tcp_message_into_receive_buffer(client_buffer, (uint16_t) *client_buffer_bytes, address, receiveBuffer);
  memset(client_buffer, 0, CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_DATA_LENGTH);
  *client_buffer_bytes = 0;
  return 0;
}
*/
/*
int save_incomplete_tcp_message(uint8_t *data,
                                ssize_t data_length,
                                device_address address,
                                uint8_t *client_buffer,
                                uint16_t *client_buffer_bytes,
                                MqttSnFixedSizeRingBuffer *receiveBuffer) {
  if (*client_buffer_bytes + data_length >= get_tcp_message_length(client_buffer)) {
    return -1;
  }
  memcpy(client_buffer + *client_buffer_bytes, data, (size_t) data_length);
  *client_buffer_bytes += (uint16_t) data_length;
  return 0;
}
*/
/*
int save_multiple_tcp_messages(uint8_t *data,
                               ssize_t data_length,
                               device_address address,
                               uint8_t *client_buffer,
                               uint16_t *client_buffer_bytes,
                               MqttSnFixedSizeRingBuffer *receiveBuffer) {
  if (*client_buffer_bytes + data_length < get_tcp_message_length(client_buffer)) {
    return -1;
  }
  uint16_t completed_message_bytes = get_tcp_message_length(client_buffer) - *client_buffer_bytes;

  if (save_completed_tcp_message(data,
                                 get_tcp_message_length(client_buffer) - *client_buffer_bytes,
                                 address,
                                 client_buffer,
                                 client_buffer_bytes,
                                 receiveBuffer) != 0) {
    return -1;
  }
  if (data_length - (ssize_t) completed_message_bytes == 0) {
    return 0;
  }
  if (save_incomplete_new_tcp_message(data + completed_message_bytes,
                                      data_length - (ssize_t) completed_message_bytes,
                                      address,
                                      client_buffer,
                                      client_buffer_bytes,
                                      receiveBuffer) == 0) {
    return 0;
  }


  // check for for incomplete
  if (save_multiple_complete_new_tcp_messages(data + completed_message_bytes,
                                              data_length - (ssize_t) completed_message_bytes,
                                              address,
                                              client_buffer,
                                              client_buffer_bytes,
                                              receiveBuffer) != 0) {
    return -1;
  }
  return 0;
}
*/
int32_t save_received_tcp_packet_into_receive_buffer(int socket_fd,
                                                     device_address *from,
                                                     uint8_t *buffer,
                                                     uint16_t *buffer_length,
                                                     uint16_t max_buffer_length,
                                                     uint8_t *data,
                                                     uint16_t max_data_length,
                                                     uint32_t *to_drop_bytes) {

  int msg_rc = 0;
  if (*to_drop_bytes > 0) {
    // read bytes from tcp socket until we have dropped enough bytes
    uint8_t receive_buffer[*to_drop_bytes];
    ssize_t read_bytes;
    if ((read_bytes = read(socket_fd, receive_buffer, *to_drop_bytes)) < 0) {
      return -1;
    }
    *to_drop_bytes = *to_drop_bytes - read_bytes;
    if (*to_drop_bytes > 0) {
      return 0;
    }
  }

  if (*buffer_length > 0) {
    // TODO data_length;
    uint16_t data_length = 0;
    msg_rc = get_next_message_from_buffer(buffer,
                                          buffer_length,
                                          max_buffer_length,
                                          data,
                                          &data_length,
                                          max_data_length,
                                          to_drop_bytes);
    if (msg_rc < 0) {
      return msg_rc;
    }
  }

  if (*to_drop_bytes > 0) {
    // read bytes from tcp socket until we have dropped enough bytes
    uint8_t receive_buffer[*to_drop_bytes];
    ssize_t read_bytes;
    if ((read_bytes = read(socket_fd, receive_buffer, *to_drop_bytes)) < 0) {
      return -1;
    }
    *to_drop_bytes = *to_drop_bytes - read_bytes;
    if (*to_drop_bytes > 0) {
      return 0;
    }
  }

  uint16_t readable_bytes = max_buffer_length - *buffer_length;

  ssize_t read_bytes;
  if ((read_bytes = read(socket_fd, buffer, readable_bytes)) < 0) {
    return -1;
  }
  if (read_bytes == 0) {
    return -2;
  }
  *buffer_length = read_bytes;
  *from = get_device_address_from_tcp_file_descriptor(socket_fd);

  if (msg_rc > 0) {
    return 0;
  }
  if (*buffer_length > 0) {
    // TODO data_length;
    uint16_t data_length = 0;
    msg_rc = get_next_message_from_buffer(buffer,
                                          buffer_length,
                                          max_buffer_length,
                                          data,
                                          &data_length,
                                          max_data_length,
                                          to_drop_bytes);
    if (msg_rc < 0) {
      return msg_rc;
    }
  }

  return msg_rc;
}
int32_t get_next_message_from_buffer(uint8_t *buffer,
                                     uint16_t *buffer_length,
                                     uint32_t max_buffer_length,
                                     uint8_t *data,
                                     uint16_t *data_length,
                                     uint16_t max_data_length,
                                     uint32_t *to_drop_bytes) {
  while (*buffer_length > 0) {
    if (isThreeBytesTcpHeader(buffer, *buffer_length)) {
      if (!isCompleteThreeBytesTcpHeader(buffer, *buffer_length)) {
        // incomplete three byte header in buffer => read out as much data as possible
        return 0;
      }
    } else {
      // two byte header
      if (!isCompleteTwoBytesTcpHeader(data, *buffer_length)) {
        // incomplete two byte header in buffer => read out as much data as possible
        return 0;
      }
    }

    uint16_t msg_length = get_tcp_message_length(buffer);
    if (msg_length > max_buffer_length) {
      // complete messages does not fit into buffer => drop the message by dropping bytes
      *to_drop_bytes = msg_length - *data_length;
      memset(buffer, 0, *data_length);
      *buffer_length = 0;
      return 0;
    }
    if (*buffer_length < msg_length) {
      // buffer contains incomplete message
      return 0;
    }
    // packet is full in buffer
    if (max_data_length < msg_length) {
      // message cannot fit into destination buffer => drop it
      uint32_t new_buffer_length = *buffer_length - msg_length;
      memmove(buffer, &buffer[msg_length], new_buffer_length); //TODO check memmove
      *buffer_length = new_buffer_length;
      return 0;
    }
    // packet is full in buffer and can be written to data
    *data_length = msg_length;
    memcpy(data, buffer, *data_length);

    uint32_t new_buffer_length = *buffer_length - msg_length;
    memmove(buffer, &buffer[msg_length], new_buffer_length); //TODO check memmove
    *buffer_length = new_buffer_length;
    return *data_length;
  }
  return 0;
}
