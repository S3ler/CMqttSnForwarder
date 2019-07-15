//
// Created by SomeDude on 14.07.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_LOGGING_MQTTSNGATEWAYDISCOVERYLOGGING_H_
#define CMQTTSNFORWARDER_GATEWAY_LOGGING_MQTTSNGATEWAYDISCOVERYLOGGING_H_

#include <stdint.h>
#include <logging/MqttSnLoggingInterface.h>
#include <gateway/database/db_entry_mqtt_sn_gateway_discovered.h>
#include <gateway/database/db_handler_result.h>
#ifdef __cplusplus
extern "C" {
#endif

int32_t log_discovery_handler_initialized(const MqttSnLogger *logger, int32_t result);

int32_t log_fatal_discovery_db_error(const MqttSnLogger *logger,
                                     const char *file_name,
                                     const char *function_name,
                                     int line_number,
                                     DB_ENTRY_MQTT_SN_GATEWAY_DISCOVERED_RESULT db_entry_result,
                                     DB_HANDLER_RESULT db_handler_result);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_LOGGING_MQTTSNGATEWAYDISCOVERYLOGGING_H_
