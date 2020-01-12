//
// Created by SomeDude on 12.09.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTTOPICNAMEPUBLISHACTION_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTTOPICNAMEPUBLISHACTION_H

#include <string>
#include <vector>
#include "MqttSnClientAction.h"
#include "MqttSnClientPublishTopicIdType.h"
class MqttSnClientTopicNamePublishAction : public MqttSnClientAction{
 public:
  const MqttSnClientPublishTopicIdType topicIdType;
  const int32_t qos;
  const std::string topicName;
  const bool retain;
  const std::vector<uint8_t> payload;

  MqttSnClientTopicNamePublishAction(const MqttSnClientPublishTopicIdType topicIdType, const int32_t qos, const std::string& topicName, const bool retain, const std::vector<uint8_t>& payload);
  //MqttSnClientTopicNamePublishAction(MqttSnClientPublishTopicIdType topicIdType, int32_t qos,  const std::string topicName, const bool retain, const std::vector<uint8_t >& payload);
};

#endif  // CMQTTSNFORWARDER_MQTTSNCLIENTTOPICNAMEPUBLISHACTION_H
