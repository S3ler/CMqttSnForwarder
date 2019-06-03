#ifndef WITH_PLATFORMIO

#include <stdio.h>
#include <stdlib.h>
#include "forwarder_starter.h"
#include <platform/platform_compatibility.h>

int main(int argc, char *argv[]) {
  MqttSnLogger logger = {0};

  if (MqttSnLoggerInit(&logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
    return -1;
  }

  forwarder_config fcfg = {0};
  if (forwarder_config_init(&fcfg)) {
    return EXIT_FAILURE;
  }
  int rc = forwarder_config_process_line(&fcfg, &logger, argc, argv);
  if (rc) {
    if (rc == 2) {
      forwarder_config_print_usage(&logger);
    } else {
      forwarder_config_print_see_usage(&logger);
      forwarder_config_cleanup(&fcfg);
      return EXIT_FAILURE;
    }
    forwarder_config_cleanup(&fcfg);
    return EXIT_SUCCESS;
  }

  MqttSnForwarder mqttSnForwarder = {0};
  void *gatewayNetworkContext = NULL;
  void *clientNetworkContext = NULL;
  rc = start_forwarder(&fcfg, &logger, &mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
  forwarder_config_cleanup(&fcfg);

  return rc;
}

#endif
