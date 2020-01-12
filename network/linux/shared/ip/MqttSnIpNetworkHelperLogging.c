//
// Created by SomeDude on 12.05.2019.
//

#include <errno.h>
#include <string.h>
#include "MqttSnIpNetworkHelperLogging.h"

int log_select_error(const MqttSnLogger *logger) {
    const char *select_error_str = "select error - ";
    log_msg_start(logger);
    log_str(logger, select_error_str);
    log_str(logger, strerror(errno));
    log_flush(logger);
    return log_status(logger);
}

int log_debug_select_error(const MqttSnLogger *logger, const char *file_name, const char *function_name, int line_number) {
    if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
        return log_status(logger);
    }
    log_msg_start(logger);

    const char *select_error_str = "select error - ";
    log_msg_start(logger);
    log_str(logger, select_error_str);
    log_str(logger, strerror(errno));

    log_str(logger, " file: ");
    log_str(logger, file_name);
    log_str(logger, " function: ");
    log_str(logger, function_name);
    log_str(logger, " line: ");
    log_int32(logger, line_number);

    log_flush(logger);
    return log_status(logger);
}
