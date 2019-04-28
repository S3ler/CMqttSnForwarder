//
// Created by SomeDude on 09.04.2019.
//

#ifndef CMQTTSNFORWARDER_STDOUTLOGGING_H
#define CMQTTSNFORWARDER_STDOUTLOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <forwarder/MqttSnForwarderLogging.h>

int stdout_log_init(MqttSnLogger *logger);
void stdout_log_deinit(MqttSnLogger *logger);
int stdout_log_flush(const MqttSnLogger *logger);
int stdout_log_str(const char *str);
int stdout_log_char(char c);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_STDOUTLOGGING_H
