//
// Created by SomeDude on 26.09.2019.
//

#include <test/MqttBroker/MqttBrokerTestContainerFactory.h>
#include <test/MqttClient/MqttClientTestContainerFactory.h>

int main() {
  auto mqttBroker = MqttBrokerTestContainerFactory::getMqttBroker();
  mqttBroker->start_broker();

  auto mqttClient = MqttClientTestContainerFactory::getMqttClient();
  mqttClient->StartBackgroundHandler();

  auto connectAction = mqttClient->getConnectAction();
  auto delayAction = mqttClient->getDelayAction(1);
  auto disconnectAction = mqttClient->getDisconnectAction();
  mqttClient->addAction(connectAction);
  mqttClient->addAction(delayAction);
  mqttClient->addAction(disconnectAction);
  mqttClient->generateActionSequence(1);

  mqttClient->execActionSequence();

  auto sequenceResult = mqttClient->GetResult();
  std::cout << MqttClientActionResult::GetCsvColumns() << std::endl;
  for (auto& result : sequenceResult) {
    std::cout << result << std::endl;
  }

  mqttClient->StopBackgroundHandler();
  mqttBroker->stop_broker();
  return 0;
}