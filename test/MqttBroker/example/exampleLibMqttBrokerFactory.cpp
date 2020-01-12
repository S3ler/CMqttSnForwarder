//
// Created by SomeDude on 26.09.2019.
//

#include <test/MqttBroker/MqttBrokerTestContainerFactory.h>
#include <thread>
int main() {
  auto mqttBroker = MqttBrokerTestContainerFactory::getMqttBroker();
  mqttBroker->start_broker();
  std::this_thread::sleep_for (std::chrono::seconds(10));
  mqttBroker->stop_broker();
  return 0;
}