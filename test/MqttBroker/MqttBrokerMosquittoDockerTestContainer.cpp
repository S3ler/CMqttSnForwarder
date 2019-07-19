//
// Created by SomeDude on 18.07.2019.
//

#include "MqttBrokerMosquittoDockerTestContainer.h"
#include <string>
#include <thread>
#include <chrono>

void MqttBrokerMosquittoDockerTestContainer::stop_broker() {
  std::string command = "docker rm -f test-broker 2> /dev/null  1> /dev/null";
  std::system(command.c_str());
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}
bool MqttBrokerMosquittoDockerTestContainer::start_broker() {
  stop_broker();
#if defined(__arm__)
  std::string command = "docker run -d --name test-broker -p 1884:1883 fstehle/rpi-mosquitto 2> /dev/null 1> /dev/null";
#else
  std::string command = "docker run -d --name test-broker -p 1884:1883 jllopis/mosquitto:v1.4.10 2> /dev/null 1> /dev/null";
#endif
  std::system(command.c_str());
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return true;
}
MqttBrokerMosquittoDockerTestContainer::MqttBrokerMosquittoDockerTestContainer() {}
bool MqttBrokerMosquittoDockerTestContainer::isRunning() {
  return false;
}
