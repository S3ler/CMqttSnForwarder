//
// Created by SomeDude on 23.07.2019.
//

#include <memory>
#include <vector>
#include <test/MqttNetworkBuilder/MqttNetworkBuilder.h>

int main() {
  std::cout << "start" << std::endl << std::flush;

  int32_t clientCount = 1000;
  int32_t rounds = 2;
  std::vector<std::shared_ptr<MqttClientTestContainerInterface>> clients;
  clients.reserve(clientCount * rounds);

  auto builder = std::make_shared<MqttNetworkBuilder>();
  auto broker = builder->getMqttBroker();

  for (int32_t i = 0; i < clientCount; i++) {
    clients.push_back(builder->getMqttClient());
  }

  if (!broker->start_broker()) {
    throw std::exception();
  }

  for (auto &client : clients) {
    if (!client->StartBackgroundHandler()) {
      throw std::exception();
    }
  }

  std::cout << "setup" << std::endl << std::flush;
  for (auto &client : clients) {
    auto connect_action = std::unique_ptr<MqttClientAction>(new MqttClientConnectAction());
    client->addAction(connect_action);
  }

  for (auto &client : clients) {
    auto subscribe_action =
        std::unique_ptr<MqttClientAction>(new MqttClientSubscribeAction(client->initial_connect_action.clientId, 0));
    client->addAction(subscribe_action);
  }

  for (auto &client : clients) {
    std::vector<uint8_t> payload;
    MqttClientPublishAction publish_action(client->initial_connect_action.clientId, payload, 0, false);
    auto publish_receive_publish_action =
        std::unique_ptr<MqttClientAction>(new MqttClientPublishReceivePublish(publish_action));
    client->addAction(publish_receive_publish_action);
  }

  for (auto &client : clients) {
    auto unsubscribe_action =
        std::unique_ptr<MqttClientAction>(new MqttClientUnsubscribeAction(client->initial_connect_action.clientId));
    client->addAction(unsubscribe_action);
  }

  for (auto &client : clients) {
    std::vector<uint8_t> payload;
    auto publish_action =
        std::unique_ptr<MqttClientAction>(new MqttClientPublishAction(client->initial_connect_action.clientId,
                                                                      payload,
                                                                      0,
                                                                      false));
    client->addAction(publish_action);
  }

  for (auto &client : clients) {
    auto disconnect_action =
        std::unique_ptr<MqttClientAction>(new MqttClientDisconnectAction());
    client->addAction(disconnect_action);
  }

  for (auto &client : clients) {
    std::shared_ptr<MqttClientActionSequence> action_sequence = client->generateActionSequence(rounds);
  }

  for (auto &client : clients) {
    client->execActionSequence_async();
  }


  bool IsActionSequenceExecuted = true;
  while (IsActionSequenceExecuted) {
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    IsActionSequenceExecuted = false;
    uint32_t running_clients = 0, finished_clients = 0;
    for (auto &client : clients) {
      if (client->IsActionSequenceExecuted()) {
        IsActionSequenceExecuted = true;
        running_clients += 1;
      } else {
        finished_clients += 1;
      }
    }
    std::cout << "running: " << std::to_string(running_clients)
              << " finished: " << std::to_string(finished_clients)
              << std::endl << std::flush;
  }
  std::cout << "end" << std::endl << std::flush;

  std::cout << "teardown" << std::endl << std::flush;

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

  broker->stop_broker();
  while (broker->isRunning()) {}

  std::cout << "end" << std::endl << std::flush;

  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << MqttClientActionResult::GetCsvColumns() << std::endl;

  for (auto &client : clients) {
    for (const auto &action_result : client->GetResult()) {
      std::cout << action_result << std::endl;
    }
  }

  return 0;
}