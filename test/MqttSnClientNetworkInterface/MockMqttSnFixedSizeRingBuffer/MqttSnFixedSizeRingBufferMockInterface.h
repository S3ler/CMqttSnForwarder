//
// Created by SomeDude on 03.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERMOCKINTERFACE_H
#define CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERMOCKINTERFACE_H

#include "../../../forwarder/MqttSnFixedSizeRingBuffer.h"

class MqttSnFixedSizeRingBufferMockInterface {
 public:
  virtual ~MqttSnFixedSizeRingBufferMockInterface() {}
  virtual void MqttSnFixedSizeRingBufferInit(MqttSnFixedSizeRingBuffer *queue) = 0;
  virtual int put(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData) = 0;
  virtual int pop(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData) = 0;
  virtual int isEmpty(MqttSnFixedSizeRingBuffer *queue) = 0;
};

#endif //CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERMOCKINTERFACE_H
