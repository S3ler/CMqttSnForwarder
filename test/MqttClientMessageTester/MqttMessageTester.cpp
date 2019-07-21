//
// Created by SomeDude on 21.07.2019.
//

#include "MqttMessageTester.h"
bool MqttMessageTester::start() {
  running = true;
  return running;
}
void MqttMessageTester::stop() {
  stopped = true;
  running = false;
}
bool MqttMessageTester::isRunning() {
  return running;
}

