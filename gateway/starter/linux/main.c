#include <stdlib.h>
#include <logging/MqttSnLoggingInterface.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <gateway/config/gateway_config.h>
#include <platform/platform_compatibility.h>
#include <gateway/MqttSnGateway.h>
#include "gateway_starter.h"

int main(int argc, char *argv[]) {
  MqttSnLogger logger = {0};

  if (MqttSnLoggerInit(&logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
    return -1;
  }

  mqtt_sn_gateway__config egwcfg = {0};
  if (mqtt_sn_gateway__config_init(&egwcfg)) {
    return EXIT_FAILURE;
  }
  int rc = process_mqtt_sn_gateway__config_line(&egwcfg, &logger, argc, argv);
  if (rc) {
    if (rc == MQTT_SN_PARSE_CONFIG_HELP) {
      mqtt_sn_gateway__config_print_usage(&logger);
    } else {
      mqtt_sn_gateway__config_print_see_usage(&logger);
      mqtt_sn_gateway__config_cleanup(&egwcfg);
      return EXIT_FAILURE;
    }
    mqtt_sn_gateway__config_cleanup(&egwcfg);
    return EXIT_SUCCESS;
  }

  MqttSnGateway egw = {0};
  void *clientNetworkContext = NULL;
  rc = start_mqtt_sn_gateway(&egwcfg, &logger, &egw, clientNetworkContext);
  mqtt_sn_gateway__config_cleanup(&egwcfg);

  mqtt_sn_gateway__config_cleanup(&egwcfg);
  return rc;
}
