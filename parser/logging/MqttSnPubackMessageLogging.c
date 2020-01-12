//
// Created by SomeDude on 23.08.2019.
//


#include "MqttSnPubackMessageLogging.h"
#include "MqttSnMessageParserLogging.h"
int32_t log_puback_gen_error(const MqttSnLogger *logger, const char *file_name, const char *function_name, int line_number) {
    return log_msg_gen_error(logger, PUBACK, file_name, function_name, line_number);
}
