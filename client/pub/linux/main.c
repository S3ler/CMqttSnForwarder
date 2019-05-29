//
// Created by SomeDude on 27.05.2019.
//

#include <stdlib.h>
#include <logging/MqttSnLoggingInterface.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <client/pub/config/publish_client_config.h>
#include <platform/platform_compatibility.h>

int main(int argc, char *argv[]) {
  MqttSnLogger cpfg_logger = {0};

  if (MqttSnLoggerInit(&cpfg_logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
    return -1;
  }

  publish_client_config cpfg = {0};
  if (publish_client_config_init(&cpfg)) {
    return EXIT_FAILURE;
  }
  int rc = publish_client_config_process_line(&cpfg, &cpfg_logger, argc, argv);
  if (rc) {
    if (rc == MQTT_SN_PARSE_CONFIG_HELP) {
      publish_client_config_print_usage(&cpfg_logger);
    } else {
      publish_client_config_print_see_usage(&cpfg_logger);
      publish_client_config_cleanup(&cpfg);
      return EXIT_FAILURE;
    }
    publish_client_config_cleanup(&cpfg);
    return EXIT_SUCCESS;
  }

  /*
  MqttSnClientPub client = {0};
  void *gatewayNetworkContext = NULL;
  rc = start_client_pub(&cpfg, &client, gatewayNetworkContext);
  */

  publish_client_config_cleanup(&cpfg);
  return rc;
}
