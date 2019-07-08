//
// Created by bele on 06.07.19.
//

#include <platform/device_address.h>
#include "MqttSnParserEncapsulationMessageParseTests.h"
#include <parser/MqttSnForwarderEncapsulationMessage.h>
#include <string.h>

void MqttSnParserEncapsulationMessageParseTests::SetUp() {
  Test::SetUp();
}
void MqttSnParserEncapsulationMessageParseTests::TearDown() {
  Test::TearDown();
}

using std::array;
using std::vector;
using ::testing::ElementsAreArray;

TEST_F(MqttSnParserEncapsulationMessageParseTests, gen_parse_test) {
  const uint16_t mqtt_sn_message_length = 255 - 9 - 1;

  //const uint16_t mqtt_sn_message_length = 255-9;
  array<uint8_t, mqtt_sn_message_length> mqtt_sn_message;

  memset(&mqtt_sn_message[0], 0, mqtt_sn_message.size());
  for (uint16_t i = 0; i < mqtt_sn_message_length; i++) {
    mqtt_sn_message[i] = mqtt_sn_message_length - i;
  }

  const uint16_t forwarders_len = 3;
  array<device_address, forwarders_len> forwarder_addresses;
  for (uint16_t i = 0; i < forwarders_len; i++) {
    memset(&forwarder_addresses[i], i, sizeof(device_address));
  }
  device_address wireless_node_id = {0};
  memset(wireless_node_id.bytes, UINT8_MAX, sizeof(wireless_node_id.bytes));

  const uint16_t radiuses_len = forwarders_len;
  array<uint8_t, radiuses_len> radiuses;
  //memset(&radiuses[0], 0, sizeof(radiuses));

  const uint16_t expected_mqtt_sn_message_length = mqtt_sn_message_length;
  array<uint8_t, expected_mqtt_sn_message_length> expected_mqtt_sn_message;
  memcpy(&expected_mqtt_sn_message[0], &mqtt_sn_message[0], sizeof(mqtt_sn_message));

  const uint16_t dst_len = UINT16_MAX;
  array<uint8_t, dst_len> dst;

  //generate_multiple_forwarder_encapsulation_headers_byte

  int32_t gen_rc = generate_multiple_forwarder_encapsulation_headers_byte(&dst[0],
                                                                          dst_len,
                                                                          &radiuses[0],
                                                                          &forwarder_addresses[0],
                                                                          forwarders_len,
                                                                          &wireless_node_id,
                                                                          &mqtt_sn_message[0],
                                                                          mqtt_sn_message_length);
  ASSERT_GT(gen_rc, 0);

  const uint16_t actual_mqtt_sn_message_length = expected_mqtt_sn_message_length;
  array<uint8_t, actual_mqtt_sn_message_length> actual_mqtt_sn_message;
  memset(&actual_mqtt_sn_message[0], 0, actual_mqtt_sn_message_length);



  uint16_t actual_forwarders_len = 0;
  const uint16_t actual_forwarders_max_len = forwarders_len + 1; // TODO
  array<device_address, actual_forwarders_max_len> actual_forwarder_addresses;
  memset(&actual_forwarder_addresses[0], 0, sizeof(actual_forwarder_addresses));

  const uint16_t actual_radiuses_max_len = actual_forwarders_max_len;
  array<uint8_t, actual_radiuses_max_len> actual_radiuses;
  //memset(actual_radiuses, 0, actual_radiuses_len);

  device_address actual_wireless_node_id = {0};

  int32_t parse_rc = parse_multiple_forwarder_encapsulation_headers_byte(&actual_radiuses[0],
                                                                         &actual_forwarder_addresses[0],
                                                                         &actual_forwarders_len,
                                                                         actual_forwarders_max_len,
                                                                         &forwarder_addresses[0],
                                                                         &actual_wireless_node_id,
                                                                         &dst[0],
                                                                         gen_rc);

  memcpy(&actual_mqtt_sn_message[0], &dst[0] + parse_rc, mqtt_sn_message_length);

  ASSERT_GT(parse_rc, 0);

  for (uint16_t i = 0; i < forwarders_len; i++) {

    device_address actual_fw_addr = actual_forwarder_addresses[i];
    device_address expected_fw_addr = forwarder_addresses[i];

    vector<uint8_t>
        actual_fw_addr_array(&actual_fw_addr.bytes[0], &actual_fw_addr.bytes[0] + sizeof(actual_fw_addr.bytes));
    vector<uint8_t>
        expected_fw_addr_array(expected_fw_addr.bytes, expected_fw_addr.bytes + sizeof(expected_fw_addr.bytes));

    EXPECT_THAT(actual_fw_addr_array, testing::ElementsAreArray(expected_fw_addr_array));
  }

  vector<uint8_t>
      wireless_node_id_vector(&wireless_node_id.bytes[0], &wireless_node_id.bytes[0] + sizeof(wireless_node_id.bytes));
  vector<uint8_t>
      actual_wireless_node_id_vector
      (&actual_wireless_node_id.bytes[0], &actual_wireless_node_id.bytes[0] + sizeof(actual_wireless_node_id.bytes));
  EXPECT_THAT(actual_wireless_node_id_vector, testing::ElementsAreArray(wireless_node_id_vector));

  EXPECT_THAT(actual_mqtt_sn_message, testing::ElementsAreArray(expected_mqtt_sn_message));
}