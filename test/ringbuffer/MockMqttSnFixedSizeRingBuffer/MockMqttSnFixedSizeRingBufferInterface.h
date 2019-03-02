//
// Created by SomeDude on 01.03.2019.
//

#ifndef CMQTTSNFORWARDER_MOCKMQTTSNFIXEDSIZERINGBUFFERINTERFACE_H
#define CMQTTSNFORWARDER_MOCKMQTTSNFIXEDSIZERINGBUFFERINTERFACE_H

#include "../../../forwarder/MqttSnFixedSizeRingBuffer.h"

class MockMqttSnFixedSizeRingBufferInterface {
 public:
  virtual void MqttSnFixedSizeRingBufferInit(MqttSnFixedSizeRingBuffer *queue) = 0;

  virtual int put(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData) = 0;

  virtual int pop(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData) = 0;

  virtual int isEmpty(MqttSnFixedSizeRingBuffer *queue) = 0;
};

#endif //CMQTTSNFORWARDER_MOCKMQTTSNFIXEDSIZERINGBUFFERINTERFACE_H
