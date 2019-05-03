#include <stdio.h>
#include <stdlib.h>
#include "main/forwarder_config.h"
#include "forwarder_starter.h"
#ifdef Arduino_h
#include <forwarder/logging/linux/arduino/ArduinoSerial.h>
#else
#include <forwarder/logging/linux/stderr/StderrLogging.h>
#endif

int main(int argc, char *argv[]) {
  MqttSnLogger fcfg_logger = {0};
#ifdef Arduino_h
  if (MqttSnLoggerInit(&fcfg_logger, LOG_LEVEL_VERBOSE, arduino_serial_log_init)) {
    return -1;
  }
#else
  if (MqttSnLoggerInit(&fcfg_logger, LOG_LEVEL_VERBOSE, stderr_log_init)) {
    return -1;
  }
#endif
  forwarder_config fcfg = {0};
  if (forwarder_config_init(&fcfg, &fcfg_logger)) {
    return EXIT_FAILURE;
  }
  int rc = process_forwarder_config_line(&fcfg, argc, argv);
  if (rc) {
    if (rc == 2) {
      print_usage(&fcfg_logger);
    } else {
      log_flush(&fcfg_logger);
      log_str(&fcfg_logger, "Use 'cmqttsnforwarder --help' to see usage.");
      log_flush(&fcfg_logger);
      forwarder_config_cleanup(&fcfg);
      return EXIT_FAILURE;
    }
    forwarder_config_cleanup(&fcfg);
    return EXIT_SUCCESS;
  }

  MqttSnForwarder mqttSnForwarder = {0};
  void *gatewayNetworkContext = NULL;
  void *clientNetworkContext = NULL;
  rc = start_forwarder(&fcfg, &mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
  forwarder_config_cleanup(&fcfg);

  return rc;
}
