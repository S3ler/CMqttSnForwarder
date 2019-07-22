//
// Created by SomeDude on 22.07.2019.
//

#include <string>
#include <memory>
#include <test/MqttClient/MqttClientConnectAction.h>
#include <test/MqttClient/MqttClientTestContainerInterface.h>
#include <test/MqttClient/MqttClientTestContainerFactory.h>
#include <test/MqttClient/MqttClientConnectActionResult.h>
#include <iostream>
#include <thread>

int main() {
  int32_t client_count = 100;
  std::vector<std::shared_ptr<MqttClientTestContainerInterface>> clients;
  std::vector<MqttClientActionResult> connect_results;
  std::vector<MqttClientActionResult> disconnect_results;
  disconnect_results.reserve(client_count);
  connect_results.reserve(client_count);
  clients.reserve(client_count);

  for (int32_t i = 0; i < client_count; i++) {
    MqttClientConnectAction connect_action(std::string("tcp"),
                                           std::string("localhost"),
                                           1883,
                                           std::string("MqttClientContainer") + std::to_string(i),
                                           std::string(""),
                                           20,
                                           true);
    clients.push_back(MqttClientTestContainerFactory::getMqttClient(MqttClientTestType::PAHO_CPP, connect_action));
  }

  for (auto &client : clients) {
    if (client->StartBackgroundHandler()) {
      connect_results.push_back(client->connect());
    }
  }

  for (auto &client : clients) {
    disconnect_results.push_back(client->disconnect());
  }
  for (auto &client : clients) {
    client->StopBackgroundHandler();
  }

  bool isBackgroundHandlerRunning = true;
  while (isBackgroundHandlerRunning) {
    isBackgroundHandlerRunning = false;
    for (auto &client : clients) {
      if (client->IsBackgroundHandlerRunning()) {
        isBackgroundHandlerRunning = true;
        break;
      }
    }
  }

  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << MqttClientActionResult::GetCsvColumns() << std::endl;
  for (auto &action_result : connect_results) {
    std::cout << action_result << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << MqttClientActionResult::GetCsvColumns() << std::endl;
  for (auto &action_result : disconnect_results) {
    std::cout << action_result << std::endl;
  }
  return 0;
}