//
// Created by SomeDude on 16.03.2019.
//

#include <string.h>
#include <network/iphelper/MqttSnIpNetworkHelper.h>
#include <MqttSnMessageParser.h>
#include <logging/MqttSnForwarderLoggingBasic.h>
#include "MqttSnTcpNetworkMessageParser.h"
#include "../../../main/forwarder_config.h"

#ifdef WITH_LOGGING
int log_new_tcp_connection(const MqttSnLogger *logger, int level, const device_address *address) {
  if (level <= LOG_LEVEL_QUIET) {
    return 0;
  }
  const char *new_client = ": New client connection from ";
  const char *on_port = " on port ";
  const char *dot = ".";
  uint32_t port = get_port_from_device_address(address);

  return (log_current_time(logger) ||
      logger->log_str(new_client) ||
      log_device_address(logger, address) ||
      logger->log_str(on_port) ||
      log_uint32(logger, port) ||
      logger->log_str(dot) ||
      log_flush(logger) != 0);
}

int log_close_tcp_connection(const MqttSnLogger *logger, int level, const device_address *address) {
  if (level <= LOG_LEVEL_QUIET) {
    return 0;
  }
  const char *close_client = ": Closed client connection from ";
  const char *on_port = " on port ";
  const char *dot = ".";
  uint32_t port = get_port_from_device_address(address);

  return (log_current_time(logger) ||
      log_str(logger, close_client) ||
      log_device_address(logger, address) ||
      log_str(logger, on_port) ||
      log_uint32(logger, port) ||
      log_str(logger, dot) ||
      log_flush(logger) != 0);
}
#endif //WITH_LOGGING

int save_messages_into_receive_buffer(uint8_t *buffer,
                                      ssize_t read_bytes,
                                      device_address gateway_address,
                                      uint8_t *client_buffer,
                                      uint16_t *client_buffer_bytes,
                                      MqttSnFixedSizeRingBuffer *receiveBuffer) {
  if (client_buffer[0] == 0) {
    // no message in buffer, new message, read message bytes
    if (save_incomplete_new_message(buffer,
                                    read_bytes,
                                    gateway_address,
                                    client_buffer,
                                    client_buffer_bytes,
                                    receiveBuffer) == 0) {
      return 0;
    }
    if (save_multiple_complete_new_messages(buffer,
                                            read_bytes,
                                            gateway_address,
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
      if (read_bytes == 1) {
        // copy and increase - then we are done
        client_buffer[*client_buffer_bytes] = buffer[0];
        *client_buffer_bytes += 1;
        return 0;
      } else if (read_bytes == 2) {
        // copy and increase - then we are done
        client_buffer[*client_buffer_bytes] = buffer[0];
        client_buffer[*client_buffer_bytes + 1] = buffer[1];
        *client_buffer_bytes += 2;
        return 0;
      } else {
        // copy into buffer but do not increase - rest ist handled in the following methods
        client_buffer[*client_buffer_bytes] = buffer[0];
        client_buffer[*client_buffer_bytes + 1] = buffer[1];
      }
    } else if (*client_buffer_bytes == 2) {
      if (read_bytes == 1) {
        // copy and increase - then we are done
        client_buffer[*client_buffer_bytes] = buffer[0];
        *client_buffer_bytes += 1;
        return 0;
      } else {
        // copy into buffer but do not increase - rest ist handled in the following methods
        client_buffer[*client_buffer_bytes] = buffer[0];
        client_buffer[*client_buffer_bytes + 1] = buffer[1];
      }
    }

  }
  // messages already in buffer, new message, read message bytes
  if (save_incomplete_message(buffer,
                              read_bytes,
                              gateway_address,
                              client_buffer,
                              client_buffer_bytes,
                              receiveBuffer) == 0) {
    return 0;
  }
  if (save_multiple_messages(buffer,
                             read_bytes,
                             gateway_address,
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
  if (data_length != get_tcp_message_length(data)) {
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

int save_multiple_complete_new_messages(uint8_t *data,
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
  if (*client_buffer_bytes + data_length != get_tcp_message_length(client_buffer)) {
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
  if (*client_buffer_bytes + data_length >= get_tcp_message_length(client_buffer)) {
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
  if (*client_buffer_bytes + data_length < get_tcp_message_length(client_buffer)) {
    return -1;
  }
  uint16_t completed_message_bytes = get_tcp_message_length(client_buffer) - *client_buffer_bytes;

  if (save_completed_message(data,
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
  if (save_incomplete_new_message(data + completed_message_bytes,
                                  data_length - (ssize_t) completed_message_bytes,
                                  address,
                                  client_buffer,
                                  client_buffer_bytes,
                                  receiveBuffer) == 0) {
    return 0;
  }


  // check for for incomplete
  if (save_multiple_complete_new_messages(data + completed_message_bytes,
                                          data_length - (ssize_t) completed_message_bytes,
                                          address,
                                          client_buffer,
                                          client_buffer_bytes,
                                          receiveBuffer) != 0) {
    return -1;
  }
  return 0;
}
