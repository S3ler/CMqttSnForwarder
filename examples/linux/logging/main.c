//
// Created by SomeDude on 20.09.2019.
//

int32_t mqtt_sn_start_logger(mqtt_sn_interactive_client *client, const mqtt_sn_logger_config *cfg, MqttSnLogger *logger);
int32_t log_mqtt_sn_logger_log_level(const MqttSnLogger *logger, uint64_t loop_counter);

int main() {
    MqttSnLogger cfg_logger = { 0 };
    if (MqttSnLoggerInit(&cfg_logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
        return -1;
    }

    const char *          executable_name = "MqttSnLogger";
    mqtt_sn_logger_config mslcfg;
    if (mqtt_sn_logger_config_init(&mslcfg)) {
        return EXIT_SUCCESS;
    }

    int32_t mslcfg_rc = mqtt_sn_logger_config_process_args(&mslcfg, logger, argc, argv);
    if (mslcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE) {
        interactive_client_config_print_see_usage(cfg_logger);
        interactive_client_config_cleanup(cfg);
        return EXIT_FAILURE;
    }
    if (mslcfg_rc != (argc - 1)) {
        for (int i = 1; i < argc; i++) {
            if (!is_mqtt_sn_logger_config_command(argv[i], &i)) {
                print_unknown_option(cfg_logger, argv[i]);
                interactive_client_config_cleanup(cfg);
                return EXIT_FAILURE;
            }
        }
    }

    MqttSnLogger logger = { 0 };
    if (mqtt_sn_start_logger(&interactive_client, &mslcfg, &logger) < 0) {
        log_str(&cfg_logger, PSTR("Could not initialize mqtt sn logger\n"));
        return EXIT_FAILURE;
    }

    uint64_t loop_counter = 0;
    while (loop_counter < 3) {
        log_mqtt_sn_logger_log_level(&logger, loop_counter++)
    }
    return EXIT_SUCCESS;
};
int32_t log_mqtt_sn_logger_log_level(const MqttSnLogger *logger, uint64_t loop_counter) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return 0;
    }

    log_msg_start(logger);
    log_str(logger, PSTR("log_level: "));
#ifdef WITH_DEBUG_LOGGING
    if (logger->log_level == LOG_LEVEL_DEBUG) {
        log_str(logger, PSTR("LOG_LEVEL_DEBUG"));
    } else
#endif
      if (logger->log_level == LOG_LEVEL_DEFAULT) {
        log_str(logger, PSTR("LOG_LEVEL_DEFAULT"));
    } else if (logger->log_level == LOG_LEVEL_VERBOSE) {
        log_str(logger, PSTR("LOG_LEVEL_VERBOSE"));
    } else {
        log_str(logger, PSTR("UNKNOWN"));
    }
    log_str(logger, PSTR(" - "));
    log_uint64(logger, loop_counter);

    log_flush(logger);
    return log_status(logger);
}
int32_t mqtt_sn_start_logger(mqtt_sn_interactive_client *client, const mqtt_sn_logger_config *cfg, MqttSnLogger *logger) {
    if (cfg->log_identifier) {
        logger->log_identifier = cfg->log_identifier;
    }

    if (!strcmp(cfg->log_target, "console")) {
        if (MqttSnLoggerInit(logger, cfg->log_lvl, stdout_log_init) < 0) {
            return -1;
        }
    } else if (!strcmp(cfg->log_target, "file")) {
        if (MqttSnLoggerInitFile(logger, cfg->log_lvl, cfg->log_file_path, file_log_init, &client->file_log_context) < 0) {
            return -1;
        }
    } else if (!strcmp(cfg->log_target, "consolefile")) {
        if (cfg->log_file_path == NULL) {
            return -1;
        }
        if (MqttSnLoggerInitFile(logger, cfg->log_lvl, cfg->log_file_path, file_stdout_log_init, &client->fileconsole_log_context) < 0) {
            return -1;
        }
    }

    return 0;
}