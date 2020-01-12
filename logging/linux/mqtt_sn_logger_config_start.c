//
// Created by SomeDude on 24.09.2019.
//

#include "mqtt_sn_logger_config_start.h"
#include <string.h>
#include <logging/linux/filestdout/FileStdoutLogging.h>
#include <logging/linux/stdout/StdoutLogging.h>
#include <logging/linux/file/FileLogging.h>

int32_t mqtt_sn_logger_config_start(MqttSnLogger *dst_logger, log_level_t log_lvl, const char *log_target, char *log_filepath,
                                    char *log_identifier) {
    if (!strcmp(log_target, "console")) {
        if (log_filepath != NULL) {
            if (MqttSnLoggerInitFile(dst_logger, log_lvl, log_filepath, file_stdout_log_init, NULL) < 0) {
                return -1;
            }
        } else {
            if (MqttSnLoggerInit(dst_logger, log_lvl, stdout_log_init) < 0) {
                return -1;
            }
        }
    } else if (!strcmp(log_target, "file")) {
        if (MqttSnLoggerInitFile(dst_logger, log_lvl, log_filepath, file_log_init, NULL) < 0) {
            return -1;
        }
    }
    if (log_identifier) {
        dst_logger->log_identifier = log_identifier;
    }
    return 0;
}
