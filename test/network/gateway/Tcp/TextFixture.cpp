//
// Created by bele on 27.02.19.
//

#include "TextFixture.h"

int tf_client_receive(struct MqttSnClientNetworkInterface *n, MqttSnFixedSizeRingBuffer*b, uint32_t t, void* context){
  return TextFixture::_client_receive(n, b, t, context);
}

int tf_client_send(struct MqttSnClientNetworkInterface *, MqttSnFixedSizeRingBuffer*, uint32_t, void* context){

}

int tf_client_network_init(struct MqttSnClientNetworkInterface*, void* context){

}

int tf_client_network_connect(struct MqttSnClientNetworkInterface*, void* context){

}

void tf_client_network_disconnect(struct MqttSnClientNetworkInterface*, void* context){

}
int TextFixture::_client_receive(MqttSnClientNetworkInterface *anInterface,
                                 MqttSnFixedSizeRingBuffer *ptr,
                                 uint32_t t,
                                 void *pVoid) {
  return 0;
}
