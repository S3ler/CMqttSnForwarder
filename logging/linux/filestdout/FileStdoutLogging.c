//
// Created by SomeDude on 12.07.2019.
//

#include <stdio.h>
#include <string.h>
#include "FileStdoutLogging.h"

int file_stdout_log_init(MqttSnLogger *logger) {
  logger->log_init = file_stdout_log_init;
  logger->log_deinit = file_stdout_log_deinit;
  logger->log_flush = file_stdout_log_flush;
  logger->log_str = file_stdout_log_str;
  logger->log_char = file_stdout_log_char;
  if (logger->context) {
    FileStdoutLoggingContext *context = (FileStdoutLoggingContext *) logger->context;
    if (logger->log_file_path) {
      context->logfile = fopen(logger->log_file_path, "a+");
    } else {
      return -1;
    }
  } else {
    return -1;
  }
  return 0;
}

void file_stdout_log_deinit(MqttSnLogger *logger) {
  FileStdoutLoggingContext *context = (FileStdoutLoggingContext *) logger->context;

  fflush(stdout);
  fflush(context->logfile);
  if (fclose(context->logfile)) {
    // success
  }
  // error
}

int file_stdout_log_flush(const MqttSnLogger *logger) {
  FileStdoutLoggingContext *context = (FileStdoutLoggingContext *) logger->context;

  if (logger->log_str(logger, "\n")) {
    return -1;
  }
  if (fflush(stdout) == EOF) {
    if (fflush(context->logfile) == EOF) {
      return -1;
    }
    return -1;
  }
  if (fflush(context->logfile) == EOF) {
    return -1;
  }
  return 0;
}

int file_stdout_log_str(const MqttSnLogger *logger, const char *str) {
  FileStdoutLoggingContext *context = (FileStdoutLoggingContext *) logger->context;

  if (fputs(str, stdout) == EOF) {
    if (fwrite(str, sizeof(char), strlen(str), context->logfile) != strlen(str)) {
      return -1;
    }
    return -1;
  }
  if (fwrite(str, sizeof(char), strlen(str), context->logfile) != strlen(str)) {
    return -1;
  }
  return 0;
}

int file_stdout_log_char(const MqttSnLogger *logger, char c) {
  FileStdoutLoggingContext *context = (FileStdoutLoggingContext *) logger->context;

  if (fputc(c, stdout) == EOF) {
    if (fputc(c, context->logfile) == EOF) {
      return -1;
    }
    return -1;
  }
  if (fputc(c, context->logfile) == EOF) {
    return -1;
  }
  return 0;
}
