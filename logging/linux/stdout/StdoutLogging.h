//
// Created by SomeDude on 09.04.2019.
//

#ifndef CMQTTSNFORWARDER_STDOUTLOGGING_H
#define CMQTTSNFORWARDER_STDOUTLOGGING_H

#include <logging/MqttSnLoggingInterface.h>
#ifdef __cplusplus
extern "C" {
#endif

int stdout_log_init(MqttSnLogger *logger);
void stdout_log_deinit(MqttSnLogger *logger);
int stdout_log_flush(const MqttSnLogger *logger);
int stdout_log_str(const MqttSnLogger *logger, const char *str);
int stdout_log_char(const MqttSnLogger *logger, char c);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_STDOUTLOGGING_H
