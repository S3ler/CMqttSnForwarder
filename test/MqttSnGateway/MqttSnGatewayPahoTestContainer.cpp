//
// Created by SomeDude on 20.07.2019.
//

#include <fstream>
#include "MqttSnGatewayPahoTestContainer.h"

using namespace MQTTSNGW;

extern volatile int theSignaled;

MqttSnGatewayPahoTestContainer::MqttSnGatewayPahoTestContainer(const std::string &identifier, const std::string &cmd)
    : MqttSnGatewayTestContainerInterface(identifier, cmd),
      task1(&gateway),
      task2(&gateway),
      task3(&gateway),
      task4(&gateway),
      task5(&gateway) {}

int32_t MqttSnGatewayPahoTestContainer::initialize() {
  if (running) {
    return -1;
  }
  if (CmdStrToArgcArgv() < 0) {
    return -1;
  }
  if (!createGatewayConfFile()) {
    return -1;
  }
  if (!createPredefinedTopicConfFile()) {
    return -1;
  }
  try {
    gateway.initialize(argc, argv);
  } catch (Exception &exception) {
    exception.writeMessage();
    return -1;
  }
  return 0;
}
int32_t MqttSnGatewayPahoTestContainer::start() {
  if (running) {
    return -1;
  }

  runner = thread(&MqttSnGatewayPahoTestContainer::loop, this);
  runner.detach();

  std::this_thread::sleep_for(std::chrono::milliseconds(5000));

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

  removeGatewayConfFile();
  removePredefinedTopicConfFile();
  running = false;
}
bool MqttSnGatewayPahoTestContainer::createGatewayConfFile() {
  std::ofstream outfile;
  outfile.open("./gateway.conf", ios::out);
  // default config file from paho/MqttSnGateay/gateway.conf
  const char data[] = "BrokerName=localhost\nBrokerPortNo=1883\nBrokerSecurePortNo=8883\n\n\
ClientAuthentication=NO\nAggregatingGateway=NO\nQoS-1=YES\nForwarder=NO\n\n\
PredefinedTopic=YES\n\
PredefinedTopicList=predefinedTopic.conf\n\
GatewayID=1\nGatewayName=PahoGateway-01\nKeepAlive=900\n\n\
GatewayPortNo=10000\nMulticastIP=225.1.1.1\nMulticastPortNo=1883\n\n\
ShearedMemory=NO;";
  /*
  const char default_gateway_conf_data[] = "BrokerName=iot.eclipse.org\nBrokerPortNo=1883\nBrokerSecurePortNo=8883\n\n\
ClientAuthentication=NO\nAggregatingGateway=NO\nQoS-1=NO\nForwarder=NO\n\n\
PredefinedTopic=NO\n\n\
GatewayID=1\nGatewayName=PahoGateway-01\nKeepAlive=900\n\n\
GatewayPortNo=10000\nMulticastIP=225.1.1.1\nMulticastPortNo=1883\n\n\
ShearedMemory=NO;";
   */
  outfile << data << endl;

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