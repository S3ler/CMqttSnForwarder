
#include <stdio.h>
#include <stdlib.h>
#include "forwarder_starter.h"
#include <platform/platform_compatibility.h>
#include <forwarder/config/forwarder_config_logger.h>

int main(int argc, char *argv[]) {
  MqttSnLogger fcfg_logger = {0};

  if (MqttSnLoggerInit(&fcfg_logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
    return -1;
  }

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
