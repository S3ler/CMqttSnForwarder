//
// Created by SomeDude on 22.07.2019.
//

#include "MqttClientTestContainerPublish.h"

MqttClientTestContainerPublish::MqttClientTestContainerPublish(const std::vector<uint8_t> &data,
                                                               const std::string &topic,
                                                               int32_t qos) : data(data), topic(topic), qos(qos) {}
