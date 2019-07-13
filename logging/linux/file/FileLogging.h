//
// Created by SomeDude on 12.07.2019.
//

#ifndef CMQTTSNFORWARDER_LOGGING_LINUX_FILE_FILELOGGING_H_
#define CMQTTSNFORWARDER_LOGGING_LINUX_FILE_FILELOGGING_H_

#include <stdio.h>
#include <logging/MqttSnLoggingInterface.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct FileLoggingContext_ {
  FILE *logfile;
} FileLoggingContext;

int file_log_init(MqttSnLogger *logger);
void file_log_deinit(MqttSnLogger *logger);
int file_log_flush(const MqttSnLogger *logger);
int file_log_str(const MqttSnLogger *logger, const char *str);
int file_log_char(const MqttSnLogger *logger, char c);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_LOGGING_LINUX_FILE_FILELOGGING_H_
