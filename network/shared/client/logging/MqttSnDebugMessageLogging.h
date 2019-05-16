//
// Created by SomeDude on 11.05.2019.
//

#ifndef CMQTTSNFORWARDER_FORWARDER_NETWORK_SHARED_CLIENT_LOGGING_MQTTSNDEBUGMESSAGELOGGING_H_
#define CMQTTSNFORWARDER_FORWARDER_NETWORK_SHARED_CLIENT_LOGGING_MQTTSNDEBUGMESSAGELOGGING_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <logging/MqttSnLoggingInterface.h>

int log_db_rec_client_message(const MqttSnLogger *logger,
                              const device_address *from,
                              const device_address *to,
                              const uint8_t *data,
                              uint16_t data_len);
int log_db_send_client_message(const MqttSnLogger *logger,
                               const device_address *from,
                               const device_address *dst,
                               const uint8_t *data,
                               uint16_t data_len);
int log_incomplete_client_message(const MqttSnLogger *logger,
                                  const device_address *from,
                                  const uint8_t *data,
                                  uint16_t data_len);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_FORWARDER_NETWORK_SHARED_CLIENT_LOGGING_MQTTSNDEBUGMESSAGELOGGING_H_
