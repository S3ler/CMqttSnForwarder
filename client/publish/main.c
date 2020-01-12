
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <client/MqttSnClient.h>
#include <client/MqttSnClientMethod.h>
#include <client/MqttSnClientMsgHandlerReturnValue.h>
#include <client/interactive/config/interactive_client_config.h>
#include <client/interactive/starter/mqtt_sn_client_interactive.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <platform/platform_compatibility.h>
#include <unistd.h>
#include <config/common/starter/starter_helper.h>
#include <client/publish/config/publish_client_config.h>

int main(int argc, char *argv[]) {
    MqttSnLogger cfg_logger = { 0 };

    if (MqttSnLoggerInit(&cfg_logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
        return -1;
    }

    publish_client_config cfg = { 0 };
    if (publish_client_config_init(&cfg)) {
        return EXIT_FAILURE;
    }
    int publish_client_config = parse_initial_publish_client_config(&cfg, &cfg_logger, argc, argv);
    if (publish_client_config == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    if (publish_client_config == MQTT_SN_PARSE_CONFIG_HELP) {
        return EXIT_SUCCESS;
    }

#ifdef WITH_LOGGING
    print_program_started(&cfg_logger, &cfg.msvcfg, cfg.executable_name);
#endif
    mqtt_sn_interactive_client interactive_client = { 0 };
    mqtt_sn_interactive_client_init(&interactive_client);

    MqttSnLogger *logger = NULL;
#ifdef WITH_LOGGING
    MqttSnLogger client_logger = { 0 };
    if (mqtt_sn_client_interactive_start_logger(&interactive_client, &cfg.mslcfg, &client_logger) < 0) {
        log_str(&cfg_logger, PSTR("Could not initialize mqtt sn logger\n"));
        return EXIT_FAILURE;
    }
    logger = &client_logger;
#endif

    mqtt_sn_client_interactive_cmd_result exec_rc;
    uint8_t                               error = 0;
    exec_rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_INIT_CLIENT,
                                                 (interactive_client_config_container *)&cfg, logger);
    if (exec_rc.action_result != MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS) {
        error = 1;
    }

    if (!error) {
        exec_rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_AUTO_CONNECT_GATEWAY,
                                                     (interactive_client_config_container *)&cfg, logger);
        if (exec_rc.action_result != MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS) {
            error = 1;
        }
    }
    if (!error) {
        exec_rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_REGISTER_GATEWAY,
                                                     (interactive_client_config_container *)&cfg, logger);
        if (exec_rc.action_result != MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS) {
            error = 1;
        }
    }
    if (!error) {
        exec_rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_PUBLISH_GATEWAY,
                                                     (interactive_client_config_container *)&cfg, logger);
        if (exec_rc.action_result != MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS) {
            error = 1;
        }
    }
    if (!error) {
        mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DISCONNECT_GATEWAY,
                                           (interactive_client_config_container *)&cfg, logger);
    }

    mqtt_sn_cmd_buffer_execute_command(&interactive_client, MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DEINIT_CLIENT,
                                       (interactive_client_config_container *)&cfg, logger);

    if (logger) {
        MqttSnLoggerDeinit(logger);
    }
    // not needed interactive_client_config_cleanup(&cfg);
    if (error) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
