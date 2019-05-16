
#include "MqttSnFixedSizeRingBuffer.h"
#include <string.h>

void MqttSnFixedSizeRingBufferInit(MqttSnFixedSizeRingBuffer *queue) {
  queue->head = 0;
  queue->tail = 0;
  queue->maxlen = CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN;
  queue->item_count = 0;
  memset(queue->items, 0, queue->maxlen * sizeof(MqttSnMessageData));
}

int put(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData) {
  uint32_t next;

  next = queue->head + 1;
  if (next >= queue->maxlen) {
    next = 0;
  }

  if (isFull(queue)) {
    return -1;
  }

  queue->items[queue->head] = *messageData;
  queue->head = next;
  queue->item_count++;
  return 0;
}

int pop(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData) {
  uint32_t next;

  if (isEmpty(queue)) {
    return -1;
  }

  next = queue->tail + 1;
  if (next >= queue->maxlen) {
    next = 0;
  }

  *messageData = queue->items[queue->tail];
  queue->tail = next;
  queue->item_count--;
  return 0;
}

int isEmpty(const MqttSnFixedSizeRingBuffer *queue) {
  return queue->item_count == 0;
}

const MqttSnMessageData *back(const MqttSnFixedSizeRingBuffer *queue) {
  if (isEmpty(queue)) {
    return NULL;
  }
  return &queue->items[queue->tail];
}

const MqttSnMessageData *front(const MqttSnFixedSizeRingBuffer *queue) {
  if (isEmpty(queue)) {
    return NULL;
  }
  return &queue->items[queue->head];
}

int isFull(const MqttSnFixedSizeRingBuffer *queue) {
  return queue->item_count == queue->maxlen;
}

const MqttSnMessageData *getBack(const MqttSnFixedSizeRingBuffer *queue, uint32_t pos) {
  if (isEmpty(queue)) {
    return NULL;
  }
  if (queue->item_count < pos) {
    return NULL;
  }
  int64_t access_pos = queue->tail + pos;
  if (access_pos >= queue->maxlen) {
    access_pos = access_pos - queue->maxlen;
  }
  return &queue->items[access_pos];
}
