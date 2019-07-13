//
// Created by SomeDude on 12.07.2019.
//

#ifndef CMQTTSNFORWARDER_LOGGING_LINUX_FILESTDOUT_FILESTDOUTLOGGING_H_
#define CMQTTSNFORWARDER_LOGGING_LINUX_FILESTDOUT_FILESTDOUTLOGGING_H_

#include <stdio.h>
#include <logging/MqttSnLoggingInterface.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct FileStdoutLoggingContext_ {
  FILE *logfile;
} FileStdoutLoggingContext;

int file_stdout_log_init(MqttSnLogger *logger);
void file_stdout_log_deinit(MqttSnLogger *logger);
int file_stdout_log_flush(const MqttSnLogger *logger);
int file_stdout_log_str(const MqttSnLogger *logger, const char *str);
int file_stdout_log_char(const MqttSnLogger *logger, char c);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_LOGGING_LINUX_FILESTDOUT_FILESTDOUTLOGGING_H_
