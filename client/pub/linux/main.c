//
// Created by SomeDude on 27.05.2019.
//

#include <stdlib.h>
#include <logging/MqttSnLoggingInterface.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <client/pub/config/publish_client_config.h>
#include <platform/platform_compatibility.h>
#include <client/pub/linux/starter/publish_client_starter.h>

int main(int argc, char *argv[]) {
  MqttSnLogger logger = {0};

  if (MqttSnLoggerInit(&logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
    return -1;
  }

  publish_client_config cfg = {0};
  if (publish_client_config_init(&cfg)) {
    return EXIT_FAILURE;
  }
  int rc = publish_client_config_process_line(&cfg, &logger, argc, argv);
  if (rc) {
    if (rc == MQTT_SN_PARSE_CONFIG_HELP) {
      publish_client_config_print_usage(&logger);
    } else {
      publish_client_config_print_see_usage(&logger);
      publish_client_config_cleanup(&cfg);
      return EXIT_FAILURE;
    }
    publish_client_config_cleanup(&cfg);
    return EXIT_SUCCESS;
  }

  MqttSnClient publish_client = {0};
  void* gatewayNetworkContext = NULL;
  rc = start_publish_client(&cfg, &logger, &publish_client, gatewayNetworkContext);

  publish_client_config_cleanup(&cfg);
  return rc;
}
