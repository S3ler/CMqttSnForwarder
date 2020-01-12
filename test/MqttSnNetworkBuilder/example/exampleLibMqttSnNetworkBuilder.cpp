//
// Created by SomeDude on 27.09.2019.
//

#include <test/MqttNetworkBuilder/MqttNetworkBuilder.h>
#include <test/MqttSnNetworkBuilder/MqttSnNetworkBuilder.h>
int main() {
  MqttNetworkBuilder mqttNetworkBuilder;
  mqttNetworkBuilder.getMqttBroker();
  mqttNetworkBuilder.startMqttBrokers();

  MqttSnNetworkBuilder mqttSnNetworkBuilder(&mqttNetworkBuilder);
  mqttSnNetworkBuilder.getMqttSnGateway();
  mqttSnNetworkBuilder.getMqttSnForwarder();
  mqttSnNetworkBuilder.startMqttSnGateways();
  mqttSnNetworkBuilder.startMqttSnForwarders();

  auto mqttSnClient = mqttSnNetworkBuilder.getMqttSnClient();
  auto forwardedMqttSnClient = mqttSnNetworkBuilder.getForwardedMqttSnClient();
  mqttSnNetworkBuilder.startMqttSnClients();
  {
    auto connectAction = mqttSnClient->getConnectAction();
    auto delayAction = mqttSnClient->getDelayAction(5);
    auto disconnectAction = mqttSnClient->getDisconnectAction();
    mqttSnClient->addAction(connectAction);
    mqttSnClient->addAction(delayAction);
    mqttSnClient->addAction(disconnectAction);
    mqttSnClient->generateActionSequence(1);
  }
  {
    auto connectAction = forwardedMqttSnClient->getConnectAction();
    auto delayAction = forwardedMqttSnClient->getDelayAction(5);
    auto disconnectAction = forwardedMqttSnClient->getDisconnectAction();
    forwardedMqttSnClient->addAction(connectAction);
    forwardedMqttSnClient->addAction(delayAction);
    forwardedMqttSnClient->addAction(disconnectAction);
    forwardedMqttSnClient->generateActionSequence(1);
  }
  mqttSnNetworkBuilder.execMqttSnClientsActionSequenceAsync();
  std::this_thread::sleep_for(std::chrono::milliseconds(10000));

  std::tuple<int, int> mqttSnClientsRunningFinished = mqttSnNetworkBuilder.getMqttSnClientsActionsSequenceExecutingFinished();
  while (std::get<0>(mqttSnClientsRunningFinished)) {
    std::cout << "Mqtt-Sn Clients running: " << std::to_string(std::get<0>(mqttSnClientsRunningFinished)) << " finished: " << std::to_string(std::get<1>(mqttSnClientsRunningFinished)) << std::endl;
    std::cout << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    mqttSnClientsRunningFinished = mqttSnNetworkBuilder.getMqttSnClientsActionsSequenceExecutingFinished();
  }

  mqttSnNetworkBuilder.stopMqttSnClients();
  mqttSnNetworkBuilder.stopMqttSnGateways();
  mqttSnNetworkBuilder.stopMqttSnForwarders();
  mqttNetworkBuilder.stopMqttClients();
  mqttNetworkBuilder.stopMqttBrokers();

  auto sequenceResult = mqttSnClient->GetResult();
  auto forwardedSequenceResult = forwardedMqttSnClient->GetResult();
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << MqttSnClientActionResult::GetCsvColumns() << std::endl;
  for (auto& result : sequenceResult) {
    std::cout << result << std::endl;
  }
  std::cout << std::endl;
  for (auto& result : forwardedSequenceResult) {
    std::cout << result << std::endl;
  }
  std::cout << std::endl;


  return 0;
}