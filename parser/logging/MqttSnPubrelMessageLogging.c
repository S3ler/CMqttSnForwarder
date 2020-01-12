//
// Created by SomeDude on 25.08.2019.
//

#include "MqttSnPubrelMessageLogging.h"
#include "MqttSnMessageParserLogging.h"

int32_t log_pubrel_gen_error(const MqttSnLogger *logger, const char *file_name, const char *function_name, int line_number) {
    return log_msg_gen_error(logger, PUBREL, file_name, function_name, line_number);
}