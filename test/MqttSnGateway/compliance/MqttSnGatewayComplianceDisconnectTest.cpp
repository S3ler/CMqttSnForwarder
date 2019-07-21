//
// Created by SomeDude on 21.07.2019.
//

#include "MqttSnGatewayComplianceDisconnectTest.h"
#include "MqttSnGatewayComplianceTestAction.h"

TEST_P(MqttSnGatewayComplianceDisconnectTest, PredefinedTopic_QoS_M1_Publish) {
  // expected - incoming publish
  const char *topic = "/unsubscribed/client/topic/name";
  const char *data = "some qos m1 data";

  std::string data_string(data);
  std::vector<uint8_t> data_vector;
  std::copy( data_string.begin(), data_string.end(), std::back_inserter(data_vector));
  data_vector.push_back('\0');

  EXPECT_CALL(mqtt_receiver, receive(AllOf(Field(&MqttPublish::data, data_vector),
                                           Field(&MqttPublish::topic, topic))));

  // when -  send publish with qos -1
  mqtt_sn_sender.send_publish(false, (int8_t) -1, false, false, (uint16_t) 50, 0, (const uint8_t *) data,
                              (uint8_t) (strlen(data) + 1));

  // wait until all message are exchanged
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceDisconnectTest, PredefinedTopic_QoS_0_Publish) {

  test_connack expected_connack(TEST_ACCEPTED);
  EXPECT_CALL(mqtt_sn_receiver, receive_connack(_)).WillOnce(check_connack(expected_connack));

  // expected - incoming publish
  const char *topic = "/unsubscribed/client/topic/name";
  const char *data = "some qos 0 data";

  std::string data_string(data);
  std::vector<uint8_t> data_vector;
  std::copy( data_string.begin(), data_string.end(), std::back_inserter(data_vector));
  data_vector.push_back('\0');

  EXPECT_CALL(mqtt_receiver, receive(AllOf(Field(&MqttPublish::data, data_vector),
                                           Field(&MqttPublish::topic, topic))));


  mqtt_sn_sender.send_connect("Mqtt SN Testclient", UINT16_MAX, false, false);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  // when -  send publish with qos 0
  mqtt_sn_sender.send_publish(false, (int8_t) 0, false, false, (uint16_t) 50, 0, (const uint8_t *) data,
                              (uint8_t) (strlen(data) + 1));

  // wait until all message are exchanged
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceDisconnectTest, PredefinedTopic_QoS_1_Publish) {

  uint16_t msg_id = 10;
  uint16_t topic_id = 50;
  test_puback expected_puback(topic_id, msg_id, TEST_ACCEPTED);
  test_connack expected_connack(TEST_ACCEPTED);

  EXPECT_CALL(mqtt_sn_receiver, receive_connack(_)).WillOnce(check_connack(expected_connack));
  EXPECT_CALL(mqtt_sn_receiver, receive_puback(_)).WillOnce(check_puback(expected_puback));

  // expected - incoming publish
  const char *topic = "/unsubscribed/client/topic/name";
  const char *data = "some qos 0 data";

  std::string data_string(data);
  std::vector<uint8_t> data_vector;
  std::copy( data_string.begin(), data_string.end(), std::back_inserter(data_vector));
  data_vector.push_back('\0');

  EXPECT_CALL(mqtt_receiver, receive(AllOf(Field(&MqttPublish::data, data_vector),
                                           Field(&MqttPublish::topic, topic))));

  mqtt_sn_sender.send_connect("Mqtt SN Testclient", UINT16_MAX, false, false);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  // when -  send publish with qos 0
  mqtt_sn_sender.send_publish(false, (int8_t) 1, false, false, topic_id, msg_id, (const uint8_t *) data,
                              (uint8_t) (strlen(data) + 1));

  // wait until all message are exchanged
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceDisconnectTest, DISABLED_PredefinedTopic_QoS_2_Publish) {

  // TODO implement me
  ASSERT_TRUE(false);
  std::cout << std::endl;
}