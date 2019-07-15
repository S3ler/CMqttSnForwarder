//
// Created by SomeDude on 14.07.2019.
//

#include <platform/platform_compatibility.h>
#include "MqttSnFixedSizeRingBufferLogging.h"

int32_t log_fatal_queue_error(const MqttSnLogger *logger,
                              const MqttSnFixedSizeRingBuffer *queue,
                              const char *file_name,
                              const char *function_name,
                              int line_number,
                              const char *operation) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_QUIET)) {
    return log_status(logger);
  }
  log_msg_start(logger);

  log_str(logger, PSTR("fatal error: queue "));
  print_queue_status(logger, queue);
  log_str(logger, PSTR(" file: "));
  log_str(logger, file_name);
  log_str(logger, PSTR(" function: "));
  log_str(logger, function_name);
  log_str(logger, PSTR(" line: "));
  log_int32(logger, line_number);
  if (operation) {
    log_str(logger, PSTR(" operation: "));
    log_str(logger, operation);
  }

  log_flush(logger);
  return log_status(logger);
}
int32_t print_queue_status(const MqttSnLogger *logger, const MqttSnFixedSizeRingBuffer *queue) {
  log_str(logger, PSTR(" head: "));
  log_uint32(logger, queue->head);
  log_str(logger, PSTR(" tail: "));
  log_uint32(logger, queue->tail);
  log_str(logger, PSTR(" maxlen: "));
  log_uint32(logger, queue->maxlen);
  log_str(logger, PSTR(" item_count: "));
  log_uint32(logger, queue->item_count);
  return log_status(logger);
}
