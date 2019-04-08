//
// Created by bele on 25.01.19.
//

#include <memory.h>
#include "MqttSnFixedSizeRingBuffer.h"

// source: https://embedjournal.com/implementing-circular-buffer-embedded-c/

void MqttSnFixedSizeRingBufferInit(MqttSnFixedSizeRingBuffer *queue) {
  queue->head = 0;
  queue->tail = 0;
  queue->maxlen = CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN;
  queue->item_count = 0;
  memset(queue->items, 0, queue->maxlen * sizeof(MqttSnMessageData));
}

int put(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData) {
  uint32_t next;

  next = queue->head + 1; // next is where head will point to after this write.
  if (next >= queue->maxlen) {
    next = 0;
  }

  if (queue->item_count == queue->maxlen) {
    return -1;
  }
  /*
  if(next == queue->tail){ // if the head + 1 == tail, circular buffer is full
      return false;
  }
  */
  queue->items[queue->head] = *messageData; // Load data and then move
  queue->head = next;                  // head to next data offset.
  queue->item_count++;
  return 0; // return success to indicate successful push.
}

int pop(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData) {
  uint32_t next;

  if (queue->item_count == 0) {
    return -1;
  }
  /*
  if(queue->head == queue->tail){ // if the head == tail, we don't have any dat
      return false;
  }
  */

  next = queue->tail + 1; // next is where tail will point to after this read.
  if (next >= queue->maxlen) {
    next = 0;
  }

  *messageData = queue->items[queue->tail];
  queue->tail = next;
  queue->item_count--;
  return 0;
}

int isEmpty(MqttSnFixedSizeRingBuffer *queue) {
  return queue->item_count == 0;
}