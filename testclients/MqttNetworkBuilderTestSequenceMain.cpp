//
// Created by SomeDude on 23.07.2019.
//

#include <memory>
#include <vector>
#include <test/MqttNetworkBuilder/MqttNetworkBuilder.h>
#include <test/MqttSnClient/MqttSnClientAction.h>
#include <test/MqttSnClient/MqttSnClientConnectAction.h>

void configureMqttClients(int32_t rounds, const vector<std::shared_ptr<MqttClientTestContainerInterface>> &clients);
void configureMqttSnClients(int32_t rounds, const vector<std::shared_ptr<MqttSnClientTestContainerInterface>> &clients);

int main() {
  std::cout << "start" << std::endl << std::flush;

  int32_t mqttBrokerCount = 1;
  int32_t mqttClientCount = 1;
  int32_t mqttSnGatewayCount = 1;
  int32_t mqttSnClientCount = 1;
  int32_t mqttSnForwarderCount = 1;
  int32_t mqttSnBehindForwarderClientCount = 1;
  int32_t rounds = 1;
  std::vector<std::shared_ptr<MqttBrokerTestContainerInterface>> mqttBrokers;
  std::vector<std::shared_ptr<MqttClientTestContainerInterface>> mqttClients;
  std::vector<std::shared_ptr<MqttSnGatewayTestContainerInterface>> mqttSnGateways;
  std::vector<std::shared_ptr<MqttSnClientTestContainerInterface>> mqttSnClients;
  std::vector<std::shared_ptr<MqttSnForwarderTestContainerInterface>> mqttSnForwarders;
  std::vector<std::shared_ptr<MqttSnClientTestContainerInterface>> mqttSnBehindForwarderClients;

  auto builder = std::make_shared<MqttNetworkBuilder>();

  for (int32_t i = 0; i < mqttBrokerCount; i++) {
    mqttBrokers.push_back(builder->getMqttBroker());
  }

  for (int32_t i = 0; i < mqttClientCount; i++) {
    mqttClients.push_back(builder->getMqttClient());
  }

  auto mqttSnNetworkBuilder = builder->getMqttSnNetworkBuilder();
  for (int32_t i = 0; i < mqttSnGatewayCount; i++) {
    mqttSnGateways.push_back(mqttSnNetworkBuilder->getMqttSnGateway());
  }

  for (int32_t i = 0; i < mqttSnClientCount; i++) {
    mqttSnClients.push_back(mqttSnNetworkBuilder->getMqttSnClient());
  }

  for (int32_t i = 0; i < mqttSnForwarderCount; i++) {
    // TODO mqttSnForwarders.push_back(mqttSnNetworkBuilder->getMqttSnForwarder());
  }

  for (int32_t i = 0; i < mqttSnBehindForwarderClientCount; i++) {
    // TODO mqttSnBehindForwarderClients.push_back(mqttSnNetworkBuilder->getMqttSnClient());
  }

  for (auto &broker : mqttBrokers) {
    if (!broker->start_broker()) {
      throw std::exception();
    }
  }

  for (auto &client : mqttClients) {
    if (!client->StartBackgroundHandler()) {
      throw std::exception();
    }
  }

  for (auto &gateway : mqttSnGateways) {
    if (gateway->start()) {
      throw std::exception();
    }
  }

  for (auto &mqtt_sn_client : mqttSnClients) {
    mqtt_sn_client->StartBackgroundHandler();
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(5000));

  std::cout << "setup" << std::endl << std::flush;
  configureMqttClients(rounds, mqttClients);
  configureMqttSnClients(rounds, mqttSnClients);
  for (auto &client : mqttClients) {
    client->execActionSequence_async();
  }
  for (auto &mqtt_sn_client : mqttSnClients) {
    mqtt_sn_client->execActionSequence_async();
  }


  bool IsActionSequenceExecuted = true;
  while (IsActionSequenceExecuted) {
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    IsActionSequenceExecuted = false;
    uint32_t running_clients = 0, finished_clients = 0;
    for (auto &client : mqttClients) {
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

  for (auto &client : mqttClients) {
    client->StopBackgroundHandler();
  }

  bool isBackgroundHandlerRunning = true;
  while (isBackgroundHandlerRunning) {
    isBackgroundHandlerRunning = false;
    for (auto &client : mqttClients) {
      if (client->IsBackgroundHandlerRunning()) {
        isBackgroundHandlerRunning = true;
        break;
      }
    }
  }



  for (auto &mqtt_sn_gateway : mqttSnGateways) {
    mqtt_sn_gateway->stop();
  }

  bool isMqttSnGwRunning = true;
  while (isMqttSnGwRunning) {
    isMqttSnGwRunning = false;
    for (auto &mqtt_sn_gateway : mqttSnGateways) {
      if (mqtt_sn_gateway->isRunning()) {
        isMqttSnGwRunning = true;
        break;
      }
    }
  }

  for (auto &broker : mqttBrokers) {
    broker->stop_broker();
    while (broker->isRunning()) {}
  }

  std::cout << "end" << std::endl << std::flush;

  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << MqttClientActionResult::GetCsvColumns() << std::endl;

  for (auto &client : mqttClients) {
    for (const auto &action_result : client->GetResult()) {
      std::cout << action_result << std::endl;
    }
  }

  return 0;
}
void configureMqttClients(int32_t rounds, const vector<std::shared_ptr<MqttClientTestContainerInterface>> &clients) {
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
}
void configureMqttSnClients(int32_t rounds, const vector<std::shared_ptr<MqttSnClientTestContainerInterface>> &clients)  {
  for (auto &client : clients) {
    auto connect_action = client->configuration.GetConnectAction();
    // TODO client->addAction(connect_action);
  }
}
