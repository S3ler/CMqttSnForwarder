//
// Created by SomeDude on 28.05.2019.
//

#include <stdlib.h>
#include <logging/MqttSnLoggingInterface.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <gateway/echogateway/config/echogateway_config.h>
#include <platform/platform_compatibility.h>
#include <forwarder/starter/linux/forwarder_starter.h>
#include <gateway/echogateway/echogateway.h>
#include <gateway/echogateway/linux/starter/echogateway_starter.h>

int main(int argc, char *argv[]) {
  MqttSnLogger logger = {0};

  if (MqttSnLoggerInit(&logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
    return -1;
  }

  echogateway_config egwcfg = {0};
  if (echogateway_config_init(&egwcfg)) {
    return EXIT_FAILURE;
  }
  int rc = process_echogateway_config_line(&egwcfg, &logger, argc, argv);
  if (rc) {
    if (rc == MQTT_SN_PARSE_CONFIG_HELP) {
      echogateway_config_print_usage(&logger);
    } else {
      echogateway_config_print_see_usage(&logger);
      echogateway_config_cleanup(&egwcfg);
      return EXIT_FAILURE;
    }
    echogateway_config_cleanup(&egwcfg);
    return EXIT_SUCCESS;
  }

  EchoGateway egw = {0};
  void *clientNetworkContext = NULL;
  rc = start_echogateway(&egwcfg, &logger, &egw, clientNetworkContext);
  echogateway_config_cleanup(&egwcfg);

  echogateway_config_cleanup(&egwcfg);
  return rc;
}
