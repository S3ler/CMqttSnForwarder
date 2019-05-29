//
// Created by SomeDude on 28.05.2019.
//

#include <stdlib.h>
#include <logging/MqttSnLoggingInterface.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <gateway/echogateway/config/echogateway_config.h>
#include <platform/platform_compatibility.h>

int main(int argc, char *argv[]) {
  MqttSnLogger cpfg_logger = {0};

  if (MqttSnLoggerInit(&cpfg_logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
    return -1;
  }

  echogateway_config cpfg = {0};
  if (echogateway_config_init(&cpfg)) {
    return EXIT_FAILURE;
  }
  int rc = process_echogateway_config_line(&cpfg, &cpfg_logger, argc, argv);
  if (rc) {
    if (rc == MQTT_SN_PARSE_CONFIG_HELP) {
      echogateway_config_print_usage(&cpfg_logger);
    } else {
      echogateway_config_print_see_usage(&cpfg_logger);
      echogateway_config_cleanup(&cpfg);
      return EXIT_FAILURE;
    }
    echogateway_config_cleanup(&cpfg);
    return EXIT_SUCCESS;
  }

  // TODO implement me

  echogateway_config_cleanup(&cpfg);
  return rc;
}
