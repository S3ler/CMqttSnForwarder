//
// Created by SomeDude on 27.05.2019.
//

#include <stdlib.h>
#include <logging/MqttSnLoggingInterface.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <platform/platform_compatibility.h>
#include <client/sub/config/subscription_client_config.h>

int main(int argc, char *argv[]) {
  MqttSnLogger logger = {0};

  if (MqttSnLoggerInit(&logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
    return -1;
  }

  subscription_client_config cfg = {0};
  if (subscription_client_config_init(&cfg)) {
    return EXIT_FAILURE;
  }
  int rc = subscription_client_config_process_line(&cfg, &logger, argc, argv);
  if (rc) {
    if (rc == MQTT_SN_PARSE_CONFIG_HELP) {
      subscription_client_config_print_usage(&logger);
    } else {
      subscription_client_config_print_see_usage(&logger);
      subscription_client_config_cleanup(&cfg);
      return EXIT_FAILURE;
    }
    subscription_client_config_cleanup(&cfg);
    return EXIT_SUCCESS;
  }

  /*
  MqttSnClientSubscribe client = {0};
  void *gatewayNetworkContext = NULL;
  rc = start_client_subscribe(&cfg, &client, gatewayNetworkContext);
  */

  subscription_client_config_cleanup(&cfg);
  return rc;
}
