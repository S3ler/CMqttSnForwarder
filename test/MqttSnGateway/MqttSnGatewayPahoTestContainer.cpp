//
// Created by SomeDude on 20.07.2019.
//

#include "MqttSnGatewayPahoTestContainer.h"
#include <test/shared/MqttSnURLParser/MqttSnURLParser.h>
#include <fstream>

using namespace MQTTSNGW;

extern volatile int theSignaled;

MqttSnGatewayPahoTestContainer::MqttSnGatewayPahoTestContainer(const MqttSnGatewayTestContainerConfiguration &configuration)
    : MqttSnGatewayTestContainerInterface(configuration),
      task1(&gateway),
      task2(&gateway),
      task3(&gateway),
      task4(&gateway),
      task5(&gateway),
      gatewayIdentifierString(std::string("PahoMqttSnGateway") + std::to_string(configuration.gateway_id)),
      cmd("") {}

int32_t MqttSnGatewayPahoTestContainer::initialize() {
  if (running) {
    return -1;
  }
  if (!createGatewayConfFile(configuration)) {
    return -1;
  }
  if (!createPredefinedTopicConfFile(configuration)) {
    return -1;
  }
  if (!createClientConfFile(configuration)) {
    return -1;
  }
  try {
    gateway.initialize(argc, argv);
  } catch (Exception &exception) {
    exception.writeMessage();
    fflush(stdout);
    return -1;
  }
  return 0;
}
int32_t MqttSnGatewayPahoTestContainer::start() {
  if (initialize() != 0) {
    return 0;
  }
  if (configuration.protocol != MqttSnGatewayProtocolTestType::UDP) {
    throw std::exception();
  }
  if (running) {
    return 0;
  }

  runner = thread(&MqttSnGatewayPahoTestContainer::loop, this);
  runner.detach();

  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  fflush(stdout);
  return 1;
}
void MqttSnGatewayPahoTestContainer::stop() {
  theSignaled = SIGINT;
  stopped = true;
}
bool MqttSnGatewayPahoTestContainer::isStopped() { return stopped; }
bool MqttSnGatewayPahoTestContainer::isRunning() { return running; }
void MqttSnGatewayPahoTestContainer::loop() {
  stopped = false;
  theSignaled = 0;
  running = true;

  gateway.run();  // blocks
  fflush(stdout);
  removeGatewayConfFile();
  removePredefinedTopicConfFile();
  running = false;
}
bool MqttSnGatewayPahoTestContainer::createGatewayConfFile(const MqttSnGatewayTestContainerConfiguration &cfg) {
  if (cfg.interfaceAddress.size() != 6) {
    throw std::exception();
  }
  if (cfg.interfaceBroadcastAddress.size() != 6) {
    throw std::exception();
  }
  if (cfg.mqtt_client_configuration.protocol_type != MqttClientProtocolTestType::TCP) {
    throw std::exception();
  }

  std::ofstream outfile;
  outfile.open("./gateway.conf", ios::out);

  outfile << "BrokerName=" << cfg.mqtt_client_configuration.brokerAddress << "\n";
  outfile << "BrokerPortNo=" << cfg.mqtt_client_configuration.brokerPort << "\n";
  outfile << "GatewayName=" << cfg.mqtt_client_configuration.clientId << "\n";
  outfile << "Forwarder=YES\n";
  outfile << "KeepAlive=" << std::to_string(cfg.advertise_duration) << "\n";

  outfile << "ClientAuthentication="
          << "NO"
          << "\n";
  outfile << "QoS-1="
          << "YES"
          << "\n";
  outfile << "Forwarder="
          << "NO"
          << "\n";
  outfile << "AggregatingGateway="
          << "NO"
          << "\n";

  outfile << "PredefinedTopic="
          << "YES"
          << "\n";
  outfile << "PredefinedTopicList="
          << "predefinedTopic.conf"
          << "\n";

  outfile << "GatewayID=" << std::to_string(cfg.gateway_id) << "\n";
  outfile << "GatewayPortNo=" << std::to_string(MqttSnURLParser::getPort(cfg.interfaceAddress)) << "\n";
  outfile << "MulticastIP=" << MqttSnURLParser::toString(cfg.interfaceBroadcastAddress, 4) << "\n";
  outfile << "MulticastPortNo=" << std::to_string(MqttSnURLParser::getPort(cfg.interfaceBroadcastAddress)) << "\n";

  outfile << "ShearedMemory="
          << "NO"
          << "\n";

  outfile << "ClientsList="
          << "clients.conf"
          << "\n";

  outfile << endl;
  outfile.close();
  return true;
}
bool MqttSnGatewayPahoTestContainer::createPredefinedTopicConfFile(const MqttSnGatewayTestContainerConfiguration &cfg) {
  // check if file exists
  std::string predefineTopicConfFileName = "./predefinedTopic.conf";
  if (FILE *file = fopen(predefineTopicConfFileName.c_str(), "r")) {
    fclose(file);
    return true;
  }

  std::ofstream outfile;
  outfile.open(predefineTopicConfFileName, ios::out);

  for (std::tuple<uint16_t, std::string> topic : cfg.predefined_topics) {
    outfile << "*," << std::get<1>(topic) << ", " << std::to_string(std::get<0>(topic)) << "\n";
  }
  outfile << "\n";
  // TODO mention paho needs client ids for topic ids
  for (std::tuple<uint16_t, std::string> topic : cfg.predefined_topics) {
    outfile << "MqttSnDefaultClient," << std::get<1>(topic) << ", " << std::to_string(std::get<0>(topic)) << "\n";
    outfile << "InteractiveClient," << std::get<1>(topic) << ", " << std::to_string(std::get<0>(topic)) << "\n";
    outfile << "PublishClient," << std::get<1>(topic) << ", " << std::to_string(std::get<0>(topic)) << "\n";
    outfile << "SubscribeClient," << std::get<1>(topic) << ", " << std::to_string(std::get<0>(topic)) << "\n";
  }

  outfile << "\n";
  for (std::tuple<uint16_t, std::string> topic : cfg.predefined_topics) {
    outfile << "QoS-1_MqttSnDefaultClient," << std::get<1>(topic) << ", " << std::to_string(std::get<0>(topic)) << "\n";
    outfile << "QoS-1_InteractiveClient," << std::get<1>(topic) << ", " << std::to_string(std::get<0>(topic)) << "\n";
    outfile << "QoS-1_PublishClient," << std::get<1>(topic) << ", " << std::to_string(std::get<0>(topic)) << "\n";
    outfile << "QoS-1_SubscribeClient," << std::get<1>(topic) << ", " << std::to_string(std::get<0>(topic)) << "\n";
  }
  // default config file from paho/MqttSnGateay/gateway.conf
  /*
  const char data[] =
      "*,/unsubscribed/client/topic/name, 50\n\
*,/another/predefined/topic, 20\nMqttSnTestclient,/unsubscribed/client/topic/name, 50\n";
  */
  outfile << endl;
  outfile.close();
  return true;
}
bool MqttSnGatewayPahoTestContainer::createClientConfFile(const MqttSnGatewayTestContainerConfiguration &configuration) {
  std::string clientsConfFileName = "./clients.conf";
  if (FILE *file = fopen(clientsConfFileName.c_str(), "r")) {
    fclose(file);
    return true;
  }

  std::ofstream outfile;
  outfile.open(clientsConfFileName, ios::out);
  //  outfile << "QoS-1_InteractiveClient,127.0.0.1:9999,QoS-1"
  outfile << "MqttSnDefaultClient,127.0.0.1:20000"
          << "\n";
  outfile << "InteractiveClient,127.0.0.1:9999"
          << "\n";
  outfile << "PublishClient,127.0.0.1:9998"
          << "\n";
  outfile << "SubscribeClient,127.0.0.1:9997"
          << "\n";
  outfile << "MqttSnDefaultForwarder,127.0.0.1:30000"
          << "\n";
  outfile << endl;
  outfile.close();
  return true;
}

bool MqttSnGatewayPahoTestContainer::removeGatewayConfFile() { return remove("./gateway.conf") == 0; }
bool MqttSnGatewayPahoTestContainer::removePredefinedTopicConfFile() { return remove("./predefinedTopic.conf") == 0; }
