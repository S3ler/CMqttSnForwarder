//
// Created by SomeDude on 25.09.2019.
//

#include "mqtt_sn_logger_config_start.hpp"

int32_t mqtt_sn_logger_config_start(MqttSnLogger *dst_logger, log_level_t log_lvl, const char *log_target, char *log_filepath,
                                    char *log_identifier) {
    if (!strcmp(log_target, "console")) {
        if (log_filepath != NULL) {
            return -1;
            //if (MqttSnLoggerInitFile(dst_logger, log_lvl, log_filepath, file_stdout_log_init, NULL) < 0) { //TODO context
            //    return -1;
            //}
        } else {
            if (MqttSnLoggerInit(dst_logger, log_lvl, arduino_serial_log_init) < 0) {
                return -1;
            }
        }
    } else if (!strcmp(log_target, "screen")) {
        if (log_filepath != NULL) {
            return -1;
            //if (MqttSnLoggerInitFile(dst_logger, log_lvl, log_filepath, file_stdout_log_init, NULL) < 0) { //TODO context
            //    return -1;
            //}
        } else {
            if (MqttSnLoggerInitFile(dst_logger, log_lvl, ssd1306_screen_log_init) < 0) {
                return -1;
            }
        }
    }
    if (log_identifier) {
        dst_logger->log_identifier = log_identifier;
    }
    return 0;
}