//
// Created by SomeDude on 09.04.2019.
//

#ifndef CMQTTSNFORWARDER_STDERRLOGGING_H
#define CMQTTSNFORWARDER_STDERRLOGGING_H

#include <logging/MqttSnLoggingInterface.h>
#ifdef __cplusplus
extern "C" {
#endif

int stderr_log_init(MqttSnLogger *logger);
void stderr_log_deinit(MqttSnLogger *logger);
int stderr_log_flush(const MqttSnLogger *logger);
int stderr_log_str(const MqttSnLogger *logger, const char *str);
int stderr_log_char(const MqttSnLogger *logger, char c);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_STDERRLOGGING_H
