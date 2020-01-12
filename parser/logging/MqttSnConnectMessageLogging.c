//
// Created by SomeDude on 21.08.2019.
//


#include "MqttSnConnectMessageLogging.h"
#include "MqttSnMessageParserLogging.h"

int32_t log_connect_gen_error(const MqttSnLogger *logger, const char *file_name, const char *function_name, int line_number) {
    return log_msg_gen_error(logger, CONNECT, file_name, function_name, line_number);
}
