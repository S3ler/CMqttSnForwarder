//
// Created by SomeDude on 09.04.2019.
//
#include "StdoutLogging.h"
#include <stdio.h>

int stdout_log_init(MqttSnLogger *logger) {
  logger->log_init = stdout_log_init;
  logger->log_deinit = stdout_log_deinit;
  logger->log_flush = stdout_log_flush;
  logger->log_str = stdout_log_str;
  logger->log_char = stdout_log_char;
  return 0;
}

void stdout_log_deinit(MqttSnLogger *logger) {
  fflush(stdout);
}

int stdout_log_flush(const MqttSnLogger *logger) {
  if (logger->log_str(logger, "\n") || fflush(stdout) == EOF) {
    return -1;
  }
  return 0;
}

int stdout_log_str(const MqttSnLogger *logger, const char *str) {
  if (fputs(str, stdout) == EOF) {
    return -1;
  }
  return 0;
}

int stdout_log_char(const MqttSnLogger *logger, char c) {
  if (fputc(c, stdout) == EOF) {
    return -1;
  }
  return 0;
}
