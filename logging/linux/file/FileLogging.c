//
// Created by SomeDude on 12.07.2019.
//

#include <stdio.h>
#include <string.h>
#include "FileLogging.h"


int file_log_init(MqttSnLogger *logger) {
  logger->log_init = file_log_init;
  logger->log_deinit = file_log_deinit;
  logger->log_flush = file_log_flush;
  logger->log_str = file_log_str;
  logger->log_char = file_log_char;
  if (logger->context) {
    FileLoggingContext *context = (FileLoggingContext *) logger->context;
    if (logger->log_filepath) {
      context->logfile = fopen(logger->log_filepath, "a+");
    } else {
      return -1;
    }
  } else {
    return -1;
  }
  return 0;
}

void file_log_deinit(MqttSnLogger *logger) {
  FileLoggingContext *context = (FileLoggingContext *) logger->context;

  fflush(context->logfile);
  if (fclose(context->logfile)) {
    // success
  }
  // error
}

int file_log_flush(const MqttSnLogger *logger) {
  FileLoggingContext *context = (FileLoggingContext *) logger->context;

  if (logger->log_str(logger, "\n") == EOF) {
    return -1;
  }
  if (fflush(context->logfile) == EOF) {
    return -1;
  }
  return 0;
}

int file_log_str(const MqttSnLogger *logger, const char *str) {
  FileLoggingContext *context = (FileLoggingContext *) logger->context;

  if (fwrite(str, sizeof(char), strlen(str), context->logfile) != strlen(str)) {
    return -1;
  }
  return 0;
}

int file_log_char(const MqttSnLogger *logger, char c) {
  FileLoggingContext *context = (FileLoggingContext *) logger->context;

  if (fputc(c, context->logfile) == EOF) {
    return -1;
  }
  return 0;
}
