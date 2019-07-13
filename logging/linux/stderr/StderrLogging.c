//
// Created by SomeDude on 09.04.2019.
//
#include "StderrLogging.h"
#include <stdio.h>

int stderr_log_init(MqttSnLogger *logger) {
  logger->log_init = stderr_log_init;
  logger->log_deinit = stderr_log_deinit;
  logger->log_flush = stderr_log_flush;
  logger->log_str = stderr_log_str;
  logger->log_char = stderr_log_char;
  return 0;
}

void stderr_log_deinit(MqttSnLogger *logger) {
  fflush(stderr);
}

int stderr_log_flush(const MqttSnLogger *logger) {
  if (logger->log_str(logger, "\n") || fflush(stderr) == EOF) {
    return -1;
  }
  return 0;
}

int stderr_log_str(const MqttSnLogger *logger, const char *str) {
  if (fputs(str, stderr) == EOF) {
    return -1;
  }
  return 0;
}

int stderr_log_char(const MqttSnLogger *logger, char c) {
  if (fputc(c, stderr) == EOF) {
    return -1;
  }
  return 0;
}
