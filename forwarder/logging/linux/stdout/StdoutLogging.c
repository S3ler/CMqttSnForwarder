//
// Created by SomeDude on 09.04.2019.
//
#include <stdio.h>
#include "StdoutLogging.h"

int stdout_log_init(struct MqttSnLogger_ *logger) {
  return 0;
}

void stdout_log_deinit(struct MqttSnLogger_ *logger) {
  fflush(stdout);
}

int stdout_log_flush(const struct MqttSnLogger_ *logger) {
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
