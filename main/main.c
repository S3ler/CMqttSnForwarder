#include <stdio.h>
#include <stdlib.h>
#include <linux/stdout/StdoutLogging.h>
#include "forwarder_config.h"
#include "forwarder_starter.h"

int main(int argc, char *argv[]) {

  forwarder_config fcfg = {0};
  if (forwarder_config_init(&fcfg)) {
    return EXIT_FAILURE;
  }
  int rc = process_forwarder_config_line(&fcfg, argc, argv);
  if (rc) {
    forwarder_config_cleanup(&fcfg);
    MqttSnLogger logger = {0};
    MqttSnLoggerInit(&logger, LOG_LEVEL_VERBOSE, stdout_log_init);
    if (rc == 2) {
      print_usage(&logger);
    } else {
      log_flush(&logger);
      log_str(&logger, "Use 'cmqttsnforwarder --help' to see usage.");
      log_flush(&logger);
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }

  MqttSnForwarder mqttSnForwarder = {0};
  void *gatewayNetworkContext = NULL;
  void *clientNetworkContext = NULL;
  rc = start_forwarder(&fcfg, &mqttSnForwarder, gatewayNetworkContext, clientNetworkContext);
  forwarder_config_cleanup(&fcfg);

  return rc;
}
