//
// Created by SomeDude on 01.03.2019.
//

#ifndef CMQTTSNFORWARDER_MOCKMQTTSNFIXEDSIZERINGBUFFER_H
#define CMQTTSNFORWARDER_MOCKMQTTSNFIXEDSIZERINGBUFFER_H

#include "MockMqttSnFixedSizeRingBufferInterface.h"

class MockMqttSnFixedSizeRingBuffer : public MockMqttSnFixedSizeRingBufferInterface {
 public:
  virtual void MqttSnFixedSizeRingBufferInit(MqttSnFixedSizeRingBuffer *queue){
    
  }

  virtual int put(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData){

  }

  virtual int pop(MqttSnFixedSizeRingBuffer *queue, MqttSnMessageData *messageData){

  }

  virtual int isEmpty(MqttSnFixedSizeRingBuffer *queue){

  }
};

#endif //CMQTTSNFORWARDER_MOCKMQTTSNFIXEDSIZERINGBUFFER_H
