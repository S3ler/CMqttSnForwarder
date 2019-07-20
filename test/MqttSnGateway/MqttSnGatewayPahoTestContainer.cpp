//
// Created by SomeDude on 20.07.2019.
//

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
  gateway.initialize(argc, argv);
  return 0;
}
int32_t MqttSnGatewayPahoTestContainer::start() {
  if (running) {
    return -1;
  }

  runner = thread(&MqttSnGatewayPahoTestContainer::loop, this);
  runner.detach();

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
  running = true;

  gateway.run(); // blocks

  running = false;
}
