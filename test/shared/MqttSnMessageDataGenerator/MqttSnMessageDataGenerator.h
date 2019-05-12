//
// Created by SomeDude on 14.04.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_SHARED_MQTTSNTESTMESSAGEDATAGENERATOR_H_
#define CMQTTSNFORWARDER_TEST_SHARED_MQTTSNTESTMESSAGEDATAGENERATOR_H_

#include <cstdint>
#include <vector>
#include <cstring>
#include <forwarder/parser/MqttSnMessageParser.h>

class MqttSnMessageDataGenerator {
 public:

  static void generateData(uint8_t *data, uint16_t data_length) {
    for (uint16_t i = 0; i < data_length; ++i) {
      data[i] = static_cast<uint8_t>(i % UINT8_MAX);
    }
  }

  static void generateDeviceAddressFromUint32(device_address *address, uint32_t address_number) {
    int16_t shift = 3;
    for (uint8_t i = 0; i < sizeof(device_address); ++i, shift = (static_cast<int16_t>(3 - i))) {
      if (shift >= 0) {
        address->bytes[i] = static_cast<uint8_t>(address_number >> (shift * 8));
      } else {
        address->bytes[i] = 0;
      }
    }
  }

  static std::vector<uint8_t> defaultMessageDataGenerator(uint16_t len) {
    std::vector<uint8_t> result;
    result.resize(len);
    for (uint16_t i = 0; i < len; i++) {
      result[i] = static_cast<uint8_t>(i % UINT8_MAX);
    }
    return result;
  }

  static MqttSnMessageData generateM1MqttSnPublishMqttSnMessageData(uint16_t data_length,
                                                                    std::vector<uint8_t> (*msgDataGenerator)(uint16_t)
                                                                    = defaultMessageDataGenerator) {
    if (data_length > CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH) {
      data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
    }

    std::vector<uint8_t> buffer;
    buffer.resize(data_length);

    bool isThreeBytesHeader = false;
    if (data_length > UINT8_MAX) {
      isThreeBytesHeader = true;
    }
    std::vector<uint8_t> data = msgDataGenerator(
        data_length - MQTT_SN_PUBLISH_MESSAGE_HEADER_LENGTH(isThreeBytesHeader));

    uint16_t msg_id = 0;
    uint16_t topic_id = 0;
    generate_publish(&buffer[0],
                     (uint16_t) buffer.size(),
                     MQTT_SN_FLAG_DUP_FIRST_TIME,
                     -1,
                     MQTT_SN_FLAG_RETAIN_FALSE,
                     MQTT_SN_FLAG_TOPIC_ID_TYPE_PREDEFINED_TOPIC_ID,
                     msg_id,
                     topic_id,
                     &data[0],
                     (uint16_t) data.size());

    MqttSnMessageData result = {0};
    memcpy(result.data, &buffer[0], buffer.size());
    return result;
  }

  static uint8_t *defaultMessageDataGenerator(uint8_t *arr, uint16_t len) {
    // TODO anpassen
    memset(arr, 0, len * sizeof(uint8_t));
    for (uint16_t i = 0; i < len; i++) {
      arr[i] = static_cast<uint8_t>(i % UINT8_MAX);
    }
    return arr;
  }

  static MqttSnMessageData generateMqttSnMessageData(uint16_t data_length,
                                                     uint8_t *(*msgDataGenerator)(uint8_t *, uint16_t)
                                                     = defaultMessageDataGenerator) {
    uint16_t headerLength = MQTT_SN_ENCAPSULATED_MESSAGE_HEADER_LENGTH(false); // TODO akutell nur bis zu 255 daten
    uint32_t generationDataLength = data_length + headerLength;
    if (generationDataLength > CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH) {
      generationDataLength = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
    }

    MqttSnMessageData result = {0};
    result.data_length = generationDataLength;
    msgDataGenerator(&result.data[headerLength], static_cast<uint16_t>(generationDataLength));
    result.data[0] = static_cast<uint8_t>(generationDataLength);     // TODO akutell nur bis zu 255 daten
    result.data[1] = ENCAPSULATED_MESSAGE;
    return result;
  }

};

#endif //CMQTTSNFORWARDER_TEST_SHARED_MQTTSNTESTMESSAGEDATAGENERATOR_H_
