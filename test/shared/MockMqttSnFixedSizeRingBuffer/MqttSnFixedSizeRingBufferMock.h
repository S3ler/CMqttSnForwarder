//
// Created by SomeDude on 03.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERMOCK_H
#define CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERMOCK_H

#include <gmock/gmock-function-mocker.h>
#include "MqttSnFixedSizeRingBufferMockInterface.h"

using ::testing::_;
using ::testing::Invoke;

class MqttSnFixedSizeRingBufferMock : public MqttSnFixedSizeRingBufferMockInterface {
 public:
  virtual ~MqttSnFixedSizeRingBufferMock() {}
  MOCK_METHOD1(MqttSnFixedSizeRingBufferInit, void(MqttSnFixedSizeRingBuffer
      *queue));
  MOCK_METHOD2(put, int(MqttSnFixedSizeRingBuffer
      *queue, MqttSnMessageData * messageData));
  MOCK_METHOD2(pop, int(MqttSnFixedSizeRingBuffer
      *queue, MqttSnMessageData * messageData));
  MOCK_METHOD1(front, const MqttSnMessageData * (
      const MqttSnFixedSizeRingBuffer *queue));
  MOCK_METHOD1(back, const MqttSnMessageData * (
      const MqttSnFixedSizeRingBuffer *queue));
  MOCK_METHOD1(isEmpty, int (
      const MqttSnFixedSizeRingBuffer *queue));
  MOCK_METHOD1(isFull, int (
      const MqttSnFixedSizeRingBuffer *queue));
};

#endif //CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERMOCK_H
