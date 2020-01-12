#include <stdio.h>
#include <stdlib.h>
#include "forwarder_starter.h"
#include <platform/platform_compatibility.h>
#include <logging/linux/file/FileLogging.h>
#include <logging/linux/filestdout/FileStdoutLogging.h>
#include <bits/types/sig_atomic_t.h>
#include <logging/linux/mqtt_sn_logger_config_start.h>

int main(int argc, char *argv[]) {
    MqttSnLogger cfg_logger = { 0 };

    if (MqttSnLoggerInit(&cfg_logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
        return -1;
    }

    forwarder_config fcfg = { 0 };
    if (forwarder_config_init(&fcfg)) {
        return EXIT_FAILURE;
    }
    int rc = forwarder_config_process_line(&fcfg, &cfg_logger, argc, argv);
    if (rc) {
        if (rc == 2) {
            forwarder_config_print_usage(&cfg_logger);
        } else {
            forwarder_config_print_see_usage(&cfg_logger);
            forwarder_config_cleanup(&fcfg);
            return EXIT_FAILURE;
        }
        forwarder_config_cleanup(&fcfg);
        return EXIT_SUCCESS;
    }

    MqttSnLogger *mqtt_sn_logger = NULL;
#ifdef WITH_LOGGING
    MqttSnLogger logger = { 0 };
    if (mqtt_sn_logger_config_start(&logger, fcfg.mslcfg.log_lvl, fcfg.mslcfg.log_target, fcfg.mslcfg.log_filepath,
                                    fcfg.mslcfg.log_identifier) < 0) {
        log_str(&cfg_logger, PSTR("Could not initialize mqtt sn logger\n"));
    }
    mqtt_sn_logger = &logger;
#endif

    MqttSnForwarder mqttSnForwarder = { 0 };

    rc = start_forwarder(&fcfg, mqtt_sn_logger, &mqttSnForwarder);
    forwarder_config_cleanup(&fcfg);

    if (mqtt_sn_logger) {
        MqttSnLoggerDeinit(mqtt_sn_logger);
    }

    return rc;
}
