//
// Created by SomeDude on 21.07.2019.
//

#include "MqttBrokerExternalTestContainer.h"

MqttBrokerExternalTestContainer::MqttBrokerExternalTestContainer() {}
void MqttBrokerExternalTestContainer::stop_broker() {
  started = false;
}
bool MqttBrokerExternalTestContainer::isRunning() {
  return started;
}
bool MqttBrokerExternalTestContainer::start_broker() {
  started = true;
  return started;
}
