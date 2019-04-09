//
// Created by SomeDude on 09.04.2019.
//

#ifndef CMQTTSNFORWARDER_STDOUTLOGGING_H
#define CMQTTSNFORWARDER_STDOUTLOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../MqttSnForwarderLogging.h"

int stdout_log_init(struct MqttSnLogger_ *logger);
void stdout_log_deinit(struct MqttSnLogger_ *logger);
int stdout_log_flush(const struct MqttSnLogger_ *logger);
int stdout_log_str(const char *str);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_STDOUTLOGGING_H
