//
// Created by SomeDude on 12.05.2019.
//

#ifndef CMQTTSNFORWARDER_UTILITY_LINUX_DIGITAL_FAKE_DIGITAL_H_
#define CMQTTSNFORWARDER_UTILITY_LINUX_DIGITAL_FAKE_DIGITAL_H_
#ifdef __cplusplus
extern "C" {
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 0
#endif

#ifndef HIGH
#define HIGH 1
#endif

#ifndef LOW
#define LOW 0
#endif

void digitalWrite(int pin, int value);

void delay(int ms);

long millis();

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_UTILITY_LINUX_DIGITAL_FAKE_DIGITAL_H_
