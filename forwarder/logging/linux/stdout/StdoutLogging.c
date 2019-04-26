//
// Created by SomeDude on 09.04.2019.
//
#include "StdoutLogging.h"
#include <stdio.h>

int stdout_log_init(MqttSnLogger *logger) {
  return 0;
}

void stdout_log_deinit(MqttSnLogger *logger) {
  fflush(stdout);
}

int stdout_log_flush(const MqttSnLogger *logger) {
  if (logger->log_str("\n") || fflush(stdout) == EOF) {
    return -1;
  }
  return 0;
}

int stdout_log_str(const char *str) {
  if (fputs(str, stdout) == EOF) {
    return -1;
  }
  return 0;
}

int stdout_log_char(char c) {
  if (fputc(c, stdout) == EOF) {
    return -1;
  }
  return 0;
}
