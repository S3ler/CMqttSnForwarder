//
// Created by SomeDude on 18.07.2019.
//

#include "MqttBrokerMosquittoDockerTestContainer.h"
#include <string>
#include <thread>
#include <chrono>

MqttBrokerMosquittoDockerTestContainer::MqttBrokerMosquittoDockerTestContainer() {}

bool MqttBrokerMosquittoDockerTestContainer::isRunning() {
  // TODO check if docker container still exists and is running
  return false;
}

void MqttBrokerMosquittoDockerTestContainer::stop_broker() {
  std::string command = "docker rm -f mosquitto-test-broker 2> /dev/null  1> /dev/null";
  std::system(command.c_str());
  // TODO instead of waiting check if docker container still exists
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}
bool MqttBrokerMosquittoDockerTestContainer::start_broker() {
  stop_broker();
  std::string command = "docker run -d --name mosquitto-test-broker -p 1883:1883 eclipse-mosquitto:1.6.3 2> /dev/null 1> /dev/null";
  std::system(command.c_str());
  // TODO instead of waiting check if docker run was successful and broker is started
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return true;
}

