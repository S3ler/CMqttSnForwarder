//
// Created by SomeDude on 09.04.2019.
//

#ifndef CMQTTSNFORWARDER_STDERRLOGGING_H
#define CMQTTSNFORWARDER_STDERRLOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <forwarder/MqttSnForwarderLogging.h>

int stderr_log_init(MqttSnLogger *logger);
void stderr_log_deinit(MqttSnLogger *logger);
int stderr_log_flush(const MqttSnLogger *logger);
int stderr_log_str(const char *str);
int stderr_log_char(char c);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_STDERRLOGGING_H
