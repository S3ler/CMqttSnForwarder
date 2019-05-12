//
// Created by SomeDude on 12.05.2019.
//

#include <errno.h>
#include <string.h>
#include "MqttSnIpNetworkHelperLogging.h"

int log_select_error(const MqttSnLogger *logger) {
  const char *select_error_str = "select error - ";
  log_msg_start(logger);
  log_str(logger, select_error_str);
  log_str(logger, strerror(errno));
  log_flush(logger);
  return log_status(logger);
}
