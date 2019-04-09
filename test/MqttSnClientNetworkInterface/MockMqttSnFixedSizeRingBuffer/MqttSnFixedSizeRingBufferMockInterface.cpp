//
// Created by SomeDude on 03.03.2019.
//

#include "MqttSnFixedSizeRingBufferMockInterface.h"
#include "MqttSnFixedSizeRingBufferMock.h"

extern MqttSnFixedSizeRingBufferMock *globalMqttSnFixedSizeRingBufferMock;

extern std::map<MqttSnFixedSizeRingBuffer *, MqttSnFixedSizeRingBufferMock *> *globalMqttSnFixedSizeRingBufferMockMap;

void MqttSnFixedSizeRingBufferInit(MqttSnFixedSizeRingBuffer *queue) {
  auto iterator = globalMqttSnFixedSizeRingBufferMockMap->find(queue);
  if (iterator == globalMqttSnFixedSizeRingBufferMockMap->end()) {
    globalMqttSnFixedSizeRingBufferMock->MqttSnFixedSizeRingBufferInit(queue);
    return;
  }
  iterator->second->MqttSnFixedSizeRingBufferInit(queue);
}

int put(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData) {
  auto iterator = globalMqttSnFixedSizeRingBufferMockMap->find(queue);
  if (iterator == globalMqttSnFixedSizeRingBufferMockMap->end()) {
    return globalMqttSnFixedSizeRingBufferMock->put(queue, messageData);
  }
  return iterator->second->put(queue, messageData);
}

int pop(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData) {
  auto iterator = globalMqttSnFixedSizeRingBufferMockMap->find(queue);
  if (iterator == globalMqttSnFixedSizeRingBufferMockMap->end()) {
    return globalMqttSnFixedSizeRingBufferMock->pop(queue, messageData);
  }
  return iterator->second->pop(queue, messageData);
}

int isEmpty(const MqttSnFixedSizeRingBuffer *queue) {
  auto iterator = globalMqttSnFixedSizeRingBufferMockMap->find(queue);
  if (iterator == globalMqttSnFixedSizeRingBufferMockMap->end()) {
    return globalMqttSnFixedSizeRingBufferMock->isEmpty(queue);
  }
  return iterator->second->isEmpty(queue);
}