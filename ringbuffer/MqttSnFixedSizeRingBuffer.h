//
// Created by bele on 25.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_H
#define CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_H


#include <stdint.h>
#include <platform/device_address.h>
#include <platform/MqttSnMessageData.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN
#define CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN 100
#endif


typedef struct MqttSnFixedSizeRingBuffer_ {
  uint32_t head;
  uint32_t tail;
  uint32_t maxlen;
  uint32_t item_count;
  MqttSnMessageData items[CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN];
} MqttSnFixedSizeRingBuffer;

void MqttSnFixedSizeRingBufferInit(MqttSnFixedSizeRingBuffer *queue);

int put(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData);

int pop(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData);

const MqttSnMessageData *front(const MqttSnFixedSizeRingBuffer *queue);
const MqttSnMessageData *back(const MqttSnFixedSizeRingBuffer *queue);

const MqttSnMessageData *getBack(const MqttSnFixedSizeRingBuffer *queue, uint32_t pos);

int isEmpty(const MqttSnFixedSizeRingBuffer *queue);
int isFull(const MqttSnFixedSizeRingBuffer *queue);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_H
