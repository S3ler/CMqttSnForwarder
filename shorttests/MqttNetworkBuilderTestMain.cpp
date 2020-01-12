//
// Created by SomeDude on 23.07.2019.
//

#include <memory>
#include <vector>
#include <test/MqttNetworkBuilder/MqttNetworkBuilder.h>

int main() {
  int32_t clientCount = 3;
  int32_t rounds = 3;
  std::vector<std::shared_ptr<MqttClientTestContainerInterface>> clients;
  std::vector<MqttClientActionResult> connect_results;
  std::vector<MqttClientActionResult> subscription_results;
  std::vector<MqttClientActionResult> unsubscription_results;
  std::vector<MqttClientActionResult> publish_result;
  std::vector<MqttClientActionResult> publish_receive_publish_results;
  std::vector<MqttClientActionResult> disconnect_results;

  clients.reserve(clientCount * rounds);
  connect_results.reserve(clientCount * rounds);
  subscription_results.reserve(clientCount * rounds);
  unsubscription_results.reserve(clientCount * rounds);
  publish_result.reserve(clientCount * rounds);
  publish_receive_publish_results.reserve(clientCount * rounds);
  disconnect_results.reserve(clientCount * rounds);

  auto builder = std::make_shared<MqttNetworkBuilder>();
  auto broker = builder->getMqttBroker();

  for (int32_t i = 0; i < clientCount; i++) {
    clients.push_back(builder->getMqttClient());
  }

  if (!broker->start_broker()) {
    throw std::exception();
  }

  std::cout << "start" << std::endl << std::flush;
  for (int32_t i = 0; i < rounds; i++) {
    std::cout << "round " << std::to_string(i) << std::endl << std::flush;
    for (auto &client : clients) {
      if (client->StartBackgroundHandler()) {
        connect_results.push_back(client->connect());
      } else {
        throw std::exception();
      }
    }

    for (auto &client : clients) {
      MqttClientSubscribeAction subscribe_action(client->configuration.clientId, 0);
      subscription_results.push_back(client->subscribe(subscribe_action));
    }

    for (auto &client : clients) {
      std::vector<uint8_t> payload;
      MqttClientPublishAction publish_action(client->configuration.clientId, payload, 0, false);
      MqttClientPublishReceivePublishAction publish_receive_publish_action(publish_action);
      publish_receive_publish_results.push_back(client->publishReceivePublish(publish_receive_publish_action));
    }

    for (auto &client : clients) {
      MqttClientUnsubscribeAction unsubscribe_action(client->configuration.clientId);
      unsubscription_results.push_back(client->unsubscribe(unsubscribe_action));
    }

    for (auto &client : clients) {
      std::vector<uint8_t> payload;
      MqttClientPublishAction publish_action(client->configuration.clientId, payload, 0, false);
      publish_result.push_back(client->publish(publish_action));
    }

    for (auto &client : clients) {
      disconnect_results.push_back(client->disconnect());
    }

    for (auto &client : clients) {
      client->StopBackgroundHandler();
    }
  }
  std::cout << "end" << std::endl << std::flush;

  bool isBackgroundHandlerRunning = true;
  while (isBackgroundHandlerRunning) {
    isBackgroundHandlerRunning = false;
    int32_t running_clients = 0;
    int32_t finished_clients = 0;
    for (auto &client : clients) {
      if (client->IsBackgroundHandlerRunning()) {
        isBackgroundHandlerRunning = true;
        running_clients += 1;
      } else {
        finished_clients += 1;
      }
    }
    std::cout << "finished: " << std::to_string(running_clients)
              << " running: " << std::to_string(finished_clients)
              << std::endl << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  }

  broker->stop_broker();
  while (broker->isRunning()) {}

  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << MqttClientActionResult::GetCsvColumns() << std::endl;
  for (auto &action_result : connect_results) {
    std::cout << action_result << std::endl;
  }
  std::cout << std::endl;
  std::cout << MqttClientActionResult::GetCsvColumns() << std::endl;
  for (auto &action_result : subscription_results) {
    std::cout << action_result << std::endl;
  }
  std::cout << std::endl;
  std::cout << MqttClientActionResult::GetCsvColumns() << std::endl;
  for (auto &action_result : unsubscription_results) {
    std::cout << action_result << std::endl;
  }
  std::cout << std::endl;
  std::cout << MqttClientActionResult::GetCsvColumns() << std::endl;
  for (auto &action_result : publish_result) {
    std::cout << action_result << std::endl;
  }
  std::cout << std::endl;
  std::cout << MqttClientActionResult::GetCsvColumns() << std::endl;
  for (auto &action_result : publish_receive_publish_results) {
    std::cout << action_result << std::endl;
  }
  std::cout << std::endl;
  std::cout << MqttClientActionResult::GetCsvColumns() << std::endl;
  for (auto &action_result : disconnect_results) {
    std::cout << action_result << std::endl;
  }

  return 0;
}