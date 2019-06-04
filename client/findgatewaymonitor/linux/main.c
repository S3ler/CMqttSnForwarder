//
// Created by SomeDude on 27.05.2019.
//

#include <stdlib.h>
#include <logging/MqttSnLoggingInterface.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <platform/platform_compatibility.h>
#include <client/findgatewaymonitor/config/find_gateway_monitor_config.h>
#include <client/findgatewaymonitor/linux/starter/publish_client_starter.h>
#include <client/MqttSnFindGatewayClient.h>

int main(int argc, char *argv[]) {
  MqttSnLogger logger = {0};

  if (MqttSnLoggerInit(&logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
    return -1;
  }

  find_gateway_monitor_config cfg = {0};
  if (find_gateway_monitor_config_init(&cfg)) {
    return EXIT_FAILURE;
  }
  int32_t rc = find_gateway_monitor_config_process_line(&cfg, &logger, argc, argv);
  if (rc) {
    if (rc == MQTT_SN_PARSE_CONFIG_HELP) {
      find_gateway_monitor_config_print_usage(&logger);
    } else {
      find_gateway_monitor_config_print_see_usage(&logger);
      find_gateway_monitor_config_cleanup(&cfg);
      return EXIT_FAILURE;
    }
    find_gateway_monitor_config_cleanup(&cfg);
    return EXIT_SUCCESS;
  }

  MqttSnFindGatewayClient client;
  MqttSnGatewayNetworkInterface gatewayNetworkInterface = {0};
  void *gatewayNetworkContext = NULL;
  rc = start_find_gateway_monitor(&cfg, &client, &logger, &gatewayNetworkInterface, gatewayNetworkContext);

  find_gateway_monitor_config_cleanup(&cfg);
  return rc;
}
