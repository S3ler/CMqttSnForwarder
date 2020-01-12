//
// Created by SomeDude on 17.08.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNPUBLISHMESSAGELOGGING_H
#define CMQTTSNFORWARDER_MQTTSNPUBLISHMESSAGELOGGING_H

#include <logging/MqttSnLogging.h>
#include <parser/MqttSnMessageParser.h>
#include <parser/MqttSnPublishMessage.h>
#ifdef __cplusplus
extern "C" {
#endif

int32_t log_publish_message(const MqttSnLogger *logger, const MqttSnPublish *publish);
int32_t log_publish_gen_error(const MqttSnLogger *logger, const char *file_name, const char *function_name, int line_number);

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNPUBLISHMESSAGELOGGING_H
