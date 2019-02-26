//
// Created by bele on 25.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_H
#define CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "global_defines.h"

#define CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN 100

#ifndef MAX_MESSAGE_LENGTH
#define MAX_MESSAGE_LENGTH 255
#endif

typedef struct MqttSnMessageData_{
  device_address address;
  uint16_t data_length;
  uint8_t data[MAX_MESSAGE_LENGTH];
} MqttSnMessageData;

typedef struct {
    uint32_t head;
    uint32_t tail;
    uint32_t maxlen;
    uint32_t item_count;
    MqttSnMessageData items[CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN];
} MqttSnFixedSizeRingBuffer;

void MqttSnFixedSizeRingBufferInit(MqttSnFixedSizeRingBuffer *queue);

int put(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData);

int pop(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData);

int isEmpty(MqttSnFixedSizeRingBuffer *queue);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_H

