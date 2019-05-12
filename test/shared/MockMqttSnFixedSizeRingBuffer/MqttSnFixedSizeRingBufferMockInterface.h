//
// Created by SomeDude on 03.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERMOCKINTERFACE_H
#define CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERMOCKINTERFACE_H

#include <ringbuffer/MqttSnFixedSizeRingBuffer.h>

class MqttSnFixedSizeRingBufferMockInterface {
 public:
  virtual ~MqttSnFixedSizeRingBufferMockInterface() {}
  virtual void MqttSnFixedSizeRingBufferInit(MqttSnFixedSizeRingBuffer *queue) = 0;
  virtual int put(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData) = 0;
  virtual int pop(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData) = 0;
  virtual const MqttSnMessageData *front(const MqttSnFixedSizeRingBuffer *queue) = 0;
  virtual const MqttSnMessageData *back(const MqttSnFixedSizeRingBuffer *queue) = 0;
  virtual int isEmpty(const MqttSnFixedSizeRingBuffer *queue) = 0;
  virtual int isFull(const MqttSnFixedSizeRingBuffer *queue) = 0;
};

#endif //CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERMOCKINTERFACE_H
