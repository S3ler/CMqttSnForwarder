//
// Created by SomeDude on 20.07.2019.
//

#include <fstream>
#include <test/shared/MqttSnURLParser/MqttSnURLParser.h>
#include "MqttSnGatewayPahoTestContainer.h"

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
      cmd("") {
}

int32_t MqttSnGatewayPahoTestContainer::initialize() {
  if (running) {
    return -1;
  }
  if (CmdStrToArgcArgv() < 0) {
    return -1;
  }
  if (!createGatewayConfFile(configuration)) {
    return -1;
  }
  if (!createPredefinedTopicConfFile()) {
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
    return -1;
  }
  if (configuration.protocol != MqttSnGatewayProtocolTestType::UDP) {
    throw std::exception();
  }
  if (running) {
    return -1;
  }

  runner = thread(&MqttSnGatewayPahoTestContainer::loop, this);
  runner.detach();

  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  fflush(stdout);
  return 0;
}
void MqttSnGatewayPahoTestContainer::stop() {
  theSignaled = SIGINT;
  stopped = true;
}
bool MqttSnGatewayPahoTestContainer::isStopped() {
  return stopped;
}
bool MqttSnGatewayPahoTestContainer::isRunning() {
  return running;
}
void MqttSnGatewayPahoTestContainer::loop() {
  stopped = false;
  theSignaled = 0;
  running = true;

  gateway.run(); // blocks
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

  // TODO i dont know the parameter yet
  /*
  if (MqttSnURLParser::isLocalhost(cfg.interfaceAddress)) {
    outfile << "GatewayInterface=" << "localhost" << "\n";
  } else {
    outfile << "GatewayInterface=" << MqttSnURLParser::toString(cfg.interfaceAddress, 4) << "\n";
  }
  */

  //"BrokerName=localhost\nBrokerPortNo=1883\nBrokerSecurePortNo=8883\n\n
  //GatewayName=PahoGateway-01\nKeepAlive=900\n\n

  outfile << "BrokerName=" << cfg.mqtt_client_configuration.brokerAddress << "\n";
  outfile << "BrokerPortNo=" << cfg.mqtt_client_configuration.brokerPort << "\n";
  outfile << "GatewayName=" << cfg.mqtt_client_configuration.clientId << "\n";
  if (cfg.mqtt_client_configuration.clientPassword.length() != 0) {
    // TODO i dont know the parameter yet
  }
  outfile << "KeepAlive=" << std::to_string(cfg.mqtt_client_configuration.keepAliveInterval) << "\n";

  outfile << "ClientAuthentication=" << "NO" << "\n";
  outfile << "QoS=" << "YES" << "\n";
  outfile << "Forwarder=" << "NO" << "\n";
  outfile << "AggregatingGateway=" << "NO" << "\n";

  outfile << "PredefinedTopic=" << "YES" << "\n";
  outfile << "PredefinedTopicList=" << "predefinedTopic.conf" << "\n";

  outfile << "GatewayID=" << std::to_string(cfg.gateway_id) << "\n";
  outfile << "GatewayPortNo=" << std::to_string(MqttSnURLParser::getPort(cfg.interfaceAddress)) << "\n";
  outfile << "MulticastIP=" << MqttSnURLParser::toString(cfg.interfaceBroadcastAddress, 4) << "\n";
  outfile << "MulticastPortNo=" << std::to_string(MqttSnURLParser::getPort(cfg.interfaceBroadcastAddress)) << "\n";

  outfile << "ShearedMemory=" << "NO" << "\n";

  /*
  const char data[] = "BrokerName=localhost\nBrokerPortNo=1883\nBrokerSecurePortNo=8883\n\n\
ClientAuthentication=NO\nAggregatingGateway=NO\nQoS-1=YES\nForwarder=NO\n\n\
PredefinedTopic=YES\n\
PredefinedTopicList=predefinedTopic.conf\n\
GatewayID=1\nGatewayName=PahoGateway-01\nKeepAlive=900\n\n\
GatewayPortNo=10000\nMulticastIP=225.1.1.1\nMulticastPortNo=1883\n\n\
ShearedMemory=NO;";
  */
  /*
  const char default_gateway_conf_data[] = "BrokerName=iot.eclipse.org\nBrokerPortNo=1883\nBrokerSecurePortNo=8883\n\n\
ClientAuthentication=NO\nAggregatingGateway=NO\nQoS-1=NO\nForwarder=NO\n\n\
PredefinedTopic=NO\n\n\
GatewayID=1\nGatewayName=PahoGateway-01\nKeepAlive=900\n\n\
GatewayPortNo=10000\nMulticastIP=225.1.1.1\nMulticastPortNo=1883\n\n\
ShearedMemory=NO;";
   */
  // default config file from paho/MqttSnGateway/gateway.conf

  outfile << endl;
  outfile.close();
  return true;
}
bool MqttSnGatewayPahoTestContainer::createPredefinedTopicConfFile() {
  std::ofstream outfile;
  outfile.open("./predefinedTopic.conf", ios::out);
  // default config file from paho/MqttSnGateay/gateway.conf
  const char data[] = "*,/unsubscribed/client/topic/name, 50\n\
*,/another/predefined/topic, 20\nMqttSnTestclient,/unsubscribed/client/topic/name, 50\n";

  outfile << data << endl;

  outfile.close();
  return true;
}
bool MqttSnGatewayPahoTestContainer::removeGatewayConfFile() {
  return remove("./gateway.conf") == 0;
}
bool MqttSnGatewayPahoTestContainer::removePredefinedTopicConfFile() {
  return remove("./predefinedTopic.conf") == 0;
}

