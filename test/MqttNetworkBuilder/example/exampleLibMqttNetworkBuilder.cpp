//
// Created by SomeDude on 26.09.2019.
//

#include <test/MqttNetworkBuilder/MqttNetworkBuilder.h>

int main() {
  MqttNetworkBuilder mqttNetworkBuilder;
  mqttNetworkBuilder.getMqttBroker();
  mqttNetworkBuilder.startMqttBrokers();

  auto mqttClient = mqttNetworkBuilder.getMqttClient();
  mqttNetworkBuilder.startMqttClients();

  auto connectAction = mqttClient->getConnectAction();
  auto delayAction = mqttClient->getDelayAction(1);
  auto disconnectAction = mqttClient->getDisconnectAction();
  mqttClient->addAction(connectAction);
  mqttClient->addAction(delayAction);
  mqttClient->addAction(disconnectAction);
  mqttClient->generateActionSequence(1);

  mqttNetworkBuilder.execMqttClientsActionSequenceAsync();

  std::tuple<int, int> mqttClientsRunningFinished = mqttNetworkBuilder.getMqttClientsActionsSequenceExecutingFinished();
  while (std::get<0>(mqttClientsRunningFinished)) {
    std::cout << "Mqtt Clients running:    " << std::to_string(std::get<0>(mqttClientsRunningFinished)) << " finished: " << std::to_string(std::get<1>(mqttClientsRunningFinished)) << std::endl;
    std::cout << std::flush;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    mqttClientsRunningFinished = mqttNetworkBuilder.getMqttClientsActionsSequenceExecutingFinished();
  }

  auto sequenceResult = mqttClient->GetResult();
  std::cout << MqttClientActionResult::GetCsvColumns() << std::endl;
  for (auto& result : sequenceResult) {
    std::cout << result << std::endl;
  }

  mqttNetworkBuilder.stopMqttClients();
  mqttNetworkBuilder.stopMqttBrokers();
  return 0;
}