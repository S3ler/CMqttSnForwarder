//
// Created by SomeDude on 27.05.2019.
//

#include <bits/types/sig_atomic_t.h>
#include <pthread.h>
#include <signal.h>
#include <logging/MqttSnLogging.h>
#include <platform/platform_compatibility.h>
#include <client/interactive/starter/mqtt_sn_client_interactive.h>
#include <client/interactive/config/interactive_client_config.h>
#include <config/common/starter/starter_helper.h>
#include <config/common/confg_command_buffer.h>

static volatile sig_atomic_t thread_mqtt_sn_client_interactive_keep_running = 1;
static volatile sig_atomic_t thread_mqtt_sn_client_interactive_stopped      = 0;
static volatile sig_atomic_t thread_mqtt_sn_client_interactive_running      = 0;

static void sig_handler(int _) {
    (void)_;
    thread_mqtt_sn_client_interactive_keep_running = 0;
}

typedef struct thread_mqtt_sn_client_interactive_ptr_ {
    MqttSnClient *c_ptr;
} thread_mqtt_sn_client_interactive_ptr;

pthread_t                             thread;
pthread_mutex_t                       thread_mqtt_sn_client_interactive_lock;
thread_mqtt_sn_client_interactive_ptr global_ptr;

void *thread_mqtt_sn_client_interactive_thread_function(void *_) {
    (void)_;

    MqttSnClient *client                      = global_ptr.c_ptr;
    thread_mqtt_sn_client_interactive_stopped = 0;
    thread_mqtt_sn_client_interactive_running = 1;
    while (thread_mqtt_sn_client_interactive_keep_running && !thread_mqtt_sn_client_interactive_stopped) {
        pthread_mutex_lock(&thread_mqtt_sn_client_interactive_lock);
        MQTT_SN_CLIENT_LOOP_RETURN_CODE loop_rc = MqttSnClientLoop(client);
        pthread_mutex_unlock(&thread_mqtt_sn_client_interactive_lock);
        if (loop_rc == MQTT_SN_CLIENT_LOOP_RETURN_ERROR) {
            break;
        }
    }
    thread_mqtt_sn_client_interactive_running = 0;
    return (void *)EXIT_SUCCESS;
}

int32_t start_interactive_thread(MqttSnClient *interactive_client);
int32_t stop_interactive_thread();

int main(int argc, char *argv[]) {
    MqttSnLogger cfg_logger = { 0 };

    if (MqttSnLoggerInit(&cfg_logger, LOG_LEVEL_VERBOSE, stdout_log_init)) {
        return -1;
    }

    interactive_client_config_container cfg;
    if (interactive_client_config_init(&cfg)) {
        return EXIT_FAILURE;
    }
    int initial_interactive_client_config = parse_initial_interactive_client_config(&cfg, &cfg_logger, argc, argv);
    if (initial_interactive_client_config == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    if (initial_interactive_client_config == MQTT_SN_PARSE_CONFIG_HELP) {
        return EXIT_SUCCESS;
    }

#ifdef WITH_LOGGING
    print_program_started(&cfg_logger, &cfg.msvcfg, cfg.executable_name);
#endif
    int32_t                       main_rc = EXIT_SUCCESS;
    mqtt_sn_config_command_buffer cmd_buffer;
    mqtt_sn_interactive_client    interactive_client = { 0 };

    mqtt_sn_config_command_buffer_init(&cmd_buffer);
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

    uint8_t exited = 0;
    // REPL for all configurations
    while (!exited) {
        // READ
        if (fgets(cmd_buffer.buffer, sizeof(cmd_buffer.buffer), stdin) == NULL) {
            main_rc = EXIT_FAILURE;
            log_str(&cfg_logger, PSTR("Error stdin broken.\n"));
            exited = 1;
            continue;
        }

        // EVAL & EXEC
        // check for exit
        cmd_buffer.buffer[strlen(cmd_buffer.buffer) - 1] = 0;
        if (mqtt_sn_client_interactive_is_exit(cmd_buffer.buffer)) {
            mqtt_sn_config_command_buffer_reset(&cmd_buffer);
            exited = 1;
            continue;
        }
        // check for help
        if (mqtt_sn_client_interactive_is_help(cmd_buffer.buffer)) {
            mqtt_sn_client_interactive_print_usage(&cfg_logger);
            mqtt_sn_config_command_buffer_reset(&cmd_buffer);
            continue;
        }
        // handle interactive commands
        MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE command_token;
        if ((command_token = mqtt_sn_client_interactive_parse_action(cmd_buffer.buffer, strlen(cmd_buffer.buffer))) !=
            MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_NO_COMMAND) {
            mqtt_sn_client_interactive_cmd_result exec_rc;
            pthread_mutex_lock(&thread_mqtt_sn_client_interactive_lock);
            exec_rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, command_token, &cfg, logger);
            pthread_mutex_unlock(&thread_mqtt_sn_client_interactive_lock);
            print_mqtt_sn_client_interactive_cmd_result(logger, &exec_rc);
            if (command_token == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_INIT_CLIENT &&
                exec_rc.action_result == MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS &&
                !thread_mqtt_sn_client_interactive_running) {
                if (start_interactive_thread(&interactive_client.client) < 0) {
                    log_str(&cfg_logger, PSTR("Could not start background handler.\n"));
                }
            }
            if (command_token == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_CONNECT_GATEWAY &&
                exec_rc.action_result == MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS &&
                !thread_mqtt_sn_client_interactive_running) {
                if (start_interactive_thread(&interactive_client.client) < 0) {
                    log_str(&cfg_logger, PSTR("Could not start background handler.\n"));
                }
            }
            if (command_token == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DEINIT_CLIENT &&
                exec_rc.action_result == MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS &&
                thread_mqtt_sn_client_interactive_running) {
                if (stop_interactive_thread()) {
                    log_str(&cfg_logger, PSTR("Could not stop background handler.\n"));
                }
            }

            mqtt_sn_config_command_buffer_reset(&cmd_buffer);
            continue;
        }

        // update configuration
        // create a copy for simulating
        mqtt_sn_config_command_buffer cmd_buffer_cp = mqtt_sn_config_command_buffer_cp(&cmd_buffer);
        if (mqtt_sn_config_command_buffer_parse_string(&cmd_buffer_cp) < 0) {
            log_str(&cfg_logger, PSTR("Error tokenizing.\n"));
            continue;
        }
        int rc;
        if ((rc = mqtt_sn_cmd_buffer_interactive_client_simulate(&cfg, &cfg_logger, cmd_buffer_cp.argc, cmd_buffer_cp.argv)) < 0) {
            // no valid configuration - no update
            mqtt_sn_config_command_buffer_reset(&cmd_buffer);
            continue;
        }
        if (rc != MQTT_SN_PARSE_CONFIG_SUCCESS) {
            if (rc == MQTT_SN_PARSE_CONFIG_HELP) {
                interactive_client_config_print_usage(&cfg_logger);
                mqtt_sn_config_command_buffer_reset(&cmd_buffer);
                continue;
            } else {
                interactive_client_config_print_see_usage(&cfg_logger);
                mqtt_sn_config_command_buffer_reset(&cmd_buffer);
                continue;
            }
        }

        // simulation successful now write real values into configuration
        if (mqtt_sn_config_command_buffer_parse_string(&cmd_buffer) < 0) {
            log_str(&cfg_logger, PSTR("Error tokenizing.\n"));
            continue;
        }
        if (cmd_buffer.argc == 1) {
            mqtt_sn_config_command_buffer_reset(&cmd_buffer);
            continue;
        }
        interactive_client_config_process_line(&cfg, &cfg_logger, cmd_buffer.argc, cmd_buffer.argv);
        interactive_client_config_copy_to_buffer(&cfg);
        mqtt_sn_config_command_buffer_reset(&cmd_buffer);
        interactive_client_config_print_updated_config(&cfg_logger, 1);
    }
    stop_interactive_thread();

    if (logger) {
        MqttSnLoggerDeinit(logger);
    }
    // not needed interactive_client_config_cleanup(&cfg);
    return main_rc;
}

int32_t start_interactive_thread(MqttSnClient *interactive_client) {
    if (thread_mqtt_sn_client_interactive_running) {
        return -1;
    }

    signal(SIGINT, sig_handler);

    global_ptr.c_ptr = interactive_client;
    if (pthread_create(&thread, NULL, thread_mqtt_sn_client_interactive_thread_function, NULL)) {
        return EXIT_FAILURE;
    }

    return 0;
}
int32_t stop_interactive_thread() {
    if (!thread_mqtt_sn_client_interactive_running) {
        return 0;
    }
    thread_mqtt_sn_client_interactive_stopped = 1;
    signal(SIGINT, SIG_DFL);
    return 0;
}
