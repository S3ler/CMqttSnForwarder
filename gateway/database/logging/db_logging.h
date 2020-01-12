//
// Created by SomeDude on 19.06.2019.
//

#ifndef CMQTTSNFORWARDER_GATEWAY_DATABASE_LOGGING_DB_LOGGING_H_
#define CMQTTSNFORWARDER_GATEWAY_DATABASE_LOGGING_DB_LOGGING_H_

#include <stdint.h>
#include <stdbool.h>
#include <logging/MqttSnLogging.h>
#include <gateway/database/db_handler.h>
#ifdef __cplusplus
extern "C" {
#endif
int32_t log_database_error(const MqttSnLogger *logger, const char* message);
int32_t log_start_client_transaction(const MqttSnLogger *logger,
                                     DB_HANDLER_RESULT db_handler_result,
                                     const device_address *address,
                                     DB_ENTRY_MQTT_SN_CLIENT_RESULT client_result,
                                     const char *client_id,
                                     const char *file_number);
int32_t log_apply_client_transaction(const MqttSnLogger *logger,
                                     DB_HANDLER_RESULT db_handler_result,
                                     DB_ENTRY_MQTT_SN_CLIENT_RESULT client_result,
                                     bool database_error);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_GATEWAY_DATABASE_LOGGING_DB_LOGGING_H_
