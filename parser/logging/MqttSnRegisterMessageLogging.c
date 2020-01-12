//
// Created by SomeDude on 25.08.2019.
//

#include "MqttSnRegisterMessageLogging.h"
#include "MqttSnMessageParserLogging.h"

int32_t log_register_gen_error(const MqttSnLogger *logger, const char *file_name, const char *function_name, int line_number) {
    return log_msg_gen_error(logger, REGISTER, file_name, function_name, line_number);
}
