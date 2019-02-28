//
// Created by bele on 27.02.19.
//

#ifndef CMQTTSNFORWARDER_TEXTFIXTURE_H
#define CMQTTSNFORWARDER_TEXTFIXTURE_H
#include "../../../../forwarder/MqttSnGatewayNetworkInterface.h"

int tf_client_receive(struct MqttSnClientNetworkInterface *, MqttSnFixedSizeRingBuffer*, uint32_t, void* context);

int tf_client_send(struct MqttSnClientNetworkInterface *, MqttSnFixedSizeRingBuffer*, uint32_t, void* context);

int tf_client_network_init(struct MqttSnClientNetworkInterface*, void* context);

int tf_client_network_connect(struct MqttSnClientNetworkInterface*, void* context);

void tf_client_network_disconnect(struct MqttSnClientNetworkInterface*, void* context);

class TextFixture {

 public:
  static int _client_receive(MqttSnClientNetworkInterface *anInterface,
                             MqttSnFixedSizeRingBuffer *ptr,
                             uint32_t t,
                             void *pVoid);
};
#endif //CMQTTSNFORWARDER_TEXTFIXTURE_H
