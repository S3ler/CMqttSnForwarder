//
// Created by SomeDude on 16.03.2019.
//

#include "MqttSnTcpNetworkMessageParser.h"
#include <network/shared/ip/MqttSnIpNetworkHelper.h>
#include <logging/MqttSnForwarderLoggingBasic.h>
#include <MqttSnMessageParser.h>
#include <string.h>
#include <errno.h>

#ifdef WITH_LOGGING

int log_select_error(const MqttSnLogger *logger) {
  const char *select_error_str = "select error - ";
  log_msg_start(logger);
  log_str(logger, select_error_str);
  log_str(logger, strerror(errno));
  log_flush(logger);
  return log_status(logger);
}

#endif //WITH_LOGGING

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

int isThreeBytesTcpHeader(uint8_t *data, ssize_t data_length) {
  MqttSnMessageHeaderThreeOctetsLengthField
      *threeOctetsLengthField = (MqttSnMessageHeaderThreeOctetsLengthField *) data;
  if (threeOctetsLengthField->indicator == 0x01) {
    return 1;
  }
  return 0;
}

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
