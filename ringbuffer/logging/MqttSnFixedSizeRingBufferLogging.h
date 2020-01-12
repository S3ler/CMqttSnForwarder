//
// Created by SomeDude on 14.07.2019.
//


#ifndef CMQTTSNFORWARDER_RINGBUFFER_LOGGING_MQTTSNFIXEDSIZERINGBUFFERLOGGING_H_
#define CMQTTSNFORWARDER_RINGBUFFER_LOGGING_MQTTSNFIXEDSIZERINGBUFFERLOGGING_H_

#include <stdint.h>
#include <ringbuffer/MqttSnFixedSizeRingBuffer.h>
#include <logging/MqttSnLogging.h>
#ifdef __cplusplus
extern "C" {
#endif

int32_t log_fatal_queue_error(const MqttSnLogger *logger,
                              const MqttSnFixedSizeRingBuffer *queue,
                              const char *file_name,
                              const char *function_name,
                              int line_number,
                              const char *operation);
int32_t print_queue_status(const MqttSnLogger *logger, const MqttSnFixedSizeRingBuffer *queue);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_RINGBUFFER_LOGGING_MQTTSNFIXEDSIZERINGBUFFERLOGGING_H_
