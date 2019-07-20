//
// Created by SomeDude on 20.07.2019.
//

#ifndef CMQTTSNFORWARDER_EXTERNAL_PAHOGWCONTAINER_H_
#define CMQTTSNFORWARDER_EXTERNAL_PAHOGWCONTAINER_H_


#include <MQTTSNGateway.h>
#include <MQTTSNGWBrokerRecvTask.h>
#include <MQTTSNGWBrokerSendTask.h>
#include <MQTTSNGWClientRecvTask.h>
#include <MQTTSNGWClientSendTask.h>
#include <MQTTSNGWPacketHandleTask.h>

using namespace MQTTSNGW;


class PahoGWContainer {
 public:
  Gateway gateway;
  PacketHandleTask  task1;
  ClientRecvTask    task2;
  ClientSendTask    task3;
  BrokerRecvTask    task4;
  BrokerSendTask    task5;

  PahoGWContainer();
};

#endif //CMQTTSNFORWARDER_EXTERNAL_PAHOGWCONTAINER_H_
