//
// Created by SomeDude on 10.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNPLUGINLOGGER_H
#define CMQTTSNFORWARDER_MQTTSNPLUGINLOGGER_H
#ifdef __cplusplus
extern "C" {
#endif

#include <logging/MqttSnLoggingInterface.h>

int log_client_network_connect(const MqttSnLogger *logger,
                               const char *protocol_name,
                               const device_address *as,
                               const device_address *to);
int log_client_network_connect_fail(const MqttSnLogger *logger,
                                    const char *protocol_name,
                                    const device_address *as,
                                    const device_address *to);
int log_client_network_disconnect(const MqttSnLogger *logger,
                                  const char *protocol_name,
                                  const device_address *address);
int log_client_network_deinitialized(const MqttSnLogger *logger,
                                     const char *protocol_name,
                                     const device_address *address);

int log_dlerror(const MqttSnLogger *logger, const char *error);
int log_max_msg_length_mismatch(const MqttSnLogger *logger, int level, uint16_t gw_max, uint16_t plugin_max);
int log_network_connect(const MqttSnLogger *logger,
                        const char *protocol_name,
                        const char *network_name,
                        const device_address *as,
                        const device_address *to);
int log_network_connect_fail(const MqttSnLogger *logger,
                             const char *protocol_name,
                             const char *network_name,
                             const device_address *as,
                             const device_address *to);
int log_network_connect_result(const MqttSnLogger *logger,
                               const char *protocol_name,
                               const char *network_name,
                               const char *result,
                               const device_address *as,
                               const device_address *to);
int log_network_disconnect(const MqttSnLogger *logger,
                           const char *protocol_name,
                           const char *network_name,
                           const device_address *address);
int log_network_deinitialized(const MqttSnLogger *logger,
                              const char *protocol_name,
                              const char *network_name,
                              const device_address *address);
int log_protocol_mismatch(const MqttSnLogger *logger, const char *expected, const char *actual);
int log_too_long_message(const MqttSnLogger *logger,
                         const device_address *from,
                         MQTT_SN_FORWARDER_NETWORK network,
                         const uint8_t *data,
                         uint16_t data_len);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_MQTTSNPLUGINLOGGER_H
