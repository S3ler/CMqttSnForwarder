//
// Created by SomeDude on 20.06.2019.
//

#include "MqttSnGatewayLogging.h"
int32_t log_something(const MqttSnLogger *logger) {

  log_flush(logger);
  return log_status(logger);
}
