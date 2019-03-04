//
// Created by SomeDude on 03.03.2019.
//

#include "MqttSnFixedSizeRingBufferMockInterface.h"
#include "MqttSnFixedSizeRingBufferMock.h"

extern MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;

void MqttSnFixedSizeRingBufferInit(MqttSnFixedSizeRingBuffer *queue) {
  globalMqttSnFixedSizeRingBufferMock->MqttSnFixedSizeRingBufferInit(queue);
}

int put(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData) {
  return globalMqttSnFixedSizeRingBufferMock->put(queue, messageData);
}

int pop(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData) {
  return globalMqttSnFixedSizeRingBufferMock->put(queue, messageData);
}

int isEmpty(MqttSnFixedSizeRingBuffer *queue) {
  return globalMqttSnFixedSizeRingBufferMock->isEmpty(queue);
}