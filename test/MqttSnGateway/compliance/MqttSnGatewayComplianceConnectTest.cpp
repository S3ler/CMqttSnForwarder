//
// Created by bele on 14.04.17.
//

#include "MqttSnGatewayComplianceConnectTest.h"

TEST_P(MqttSnGatewayComplianceConnectTest, Connect_withoutwill_duration60_nocleansession_return_accepted) {

  test_connack expected_connack(TEST_ACCEPTED);

  EXPECT_CALL(mqtt_sn_receiver, receive_connack(_)).WillOnce(check_connack(expected_connack));
  mqtt_sn_sender.send_connect("Mqtt Sn Testclient", 60, false, false);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

TEST_P(MqttSnGatewayComplianceConnectTest, Connect_withminimumclientidsize_return_accepted) {

  test_connack expected_connack(TEST_ACCEPTED);

  EXPECT_CALL(mqtt_sn_receiver, receive_connack(_)).WillOnce(check_connack(expected_connack));
  mqtt_sn_sender.send_connect("M", 60, false, false);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceConnectTest, Connect_withmaximumclientidsize_return_accepted) {

  test_connack expected_connack(TEST_ACCEPTED);

  EXPECT_CALL(mqtt_sn_receiver, receive_connack(_)).WillOnce(check_connack(expected_connack));
  mqtt_sn_sender.send_connect("MqttSNTestclient23chars", 60, false, false);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceConnectTest, Connect_withemptyclientid_return_disconnect) {

  test_disconnect expected_disconnect;

  EXPECT_CALL(mqtt_sn_receiver, receive_disconnect(_)).WillOnce(check_disconnect(expected_disconnect));
  mqtt_sn_sender.send_connect("", 60, false, false);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceConnectTest, Connect_withtoolongclientid_return_disconnect) {

  test_disconnect expected_disconnect;

  EXPECT_CALL(mqtt_sn_receiver, receive_disconnect(_)).WillOnce(check_disconnect(expected_disconnect));
  mqtt_sn_sender.send_connect("MqttSNTestClientLongerThan23Characters", 60, false, false);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceConnectTest, Connect_withoutwill_durationMin_nocleansession_return_accepted) {

  test_connack expected_connack(TEST_ACCEPTED);

  EXPECT_CALL(mqtt_sn_receiver, receive_connack(_)).WillOnce(check_connack(expected_connack));
  mqtt_sn_sender.send_connect("Mqtt SN Testclient", 0, false, false);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceConnectTest, Connect_withoutwill_durationMax_nocleansession_return_accepted) {

  test_connack expected_connack(TEST_ACCEPTED);

  EXPECT_CALL(mqtt_sn_receiver, receive_connack(_)).WillOnce(check_connack(expected_connack));
  mqtt_sn_sender.send_connect("Mqtt SN Testclient", UINT16_MAX, false, false);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceConnectTest, Connect_withoutwill_duration60_cleansession_return_accepted) {

  test_connack expected_connack(TEST_ACCEPTED);

  EXPECT_CALL(mqtt_sn_receiver, receive_connack(_)).WillOnce(check_connack(expected_connack));
  mqtt_sn_sender.send_connect("Mqtt SN Testclient", 60, true, false);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceConnectTest, Connect_withwill_return_accepted) {

  test_connack expected_connack(TEST_ACCEPTED);
  test_willtopicreq expected_willtopicreq;
  test_willmsgreq expected_willmsgreq;

  const char *willtopic = "/some/will/topic";
  uint8_t qos = 0;
  bool retain = false;
  const char *willmsg = "some will message";

  EXPECT_CALL(mqtt_sn_receiver, receive_connack(_)).WillOnce(check_connack(expected_connack));

  EXPECT_CALL(mqtt_sn_receiver, receive_willmsgreq(_))
      .WillOnce(DoAll(check_willmsg_req(expected_willmsgreq),
                      InvokeUnrelatedFunction(&mqtt_sn_sender, &MqttSnTestMessageSender::send_willmsg,
                                              (const uint8_t *) willmsg, strlen(willmsg) + 1)));

  EXPECT_CALL(mqtt_sn_receiver, receive_willtopicreq(_))
      .WillOnce(DoAll(check_willtopicreq(expected_willtopicreq),
                      InvokeUnrelatedFunction(&mqtt_sn_sender, &MqttSnTestMessageSender::send_willtopic,
                                              willtopic, qos, retain)));

  mqtt_sn_sender.send_connect("Mqtt SN Testclient", 60, true, true);

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceConnectTest, Connect_withwill_minimumWillTopicLength_return_accepted) {

  test_connack expected_connack(TEST_ACCEPTED);
  test_willtopicreq expected_willtopicreq;
  test_willmsgreq expected_willmsgreq;

  const char *willtopic = "M";
  uint8_t qos = 0;
  bool retain = false;
  const char *willmsg = "some will message";

  EXPECT_CALL(mqtt_sn_receiver, receive_connack(_)).WillOnce(check_connack(expected_connack));

  EXPECT_CALL(mqtt_sn_receiver, receive_willmsgreq(_))
      .WillOnce(DoAll(check_willmsg_req(expected_willmsgreq),
                      InvokeUnrelatedFunction(&mqtt_sn_sender, &MqttSnTestMessageSender::send_willmsg,
                                              (const uint8_t *) willmsg, strlen(willmsg) + 1)));

  EXPECT_CALL(mqtt_sn_receiver, receive_willtopicreq(_))
      .WillOnce(DoAll(check_willtopicreq(expected_willtopicreq),
                      InvokeUnrelatedFunction(&mqtt_sn_sender, &MqttSnTestMessageSender::send_willtopic,
                                              willtopic, qos, retain)));

  mqtt_sn_sender.send_connect("Mqtt SN Testclient", 60, true, true);

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceConnectTest, Connect_withwill_maximumWillTopicLength_return_accepted) {

  test_connack expected_connack(TEST_ACCEPTED);
  test_willtopicreq expected_willtopicreq;
  test_willmsgreq expected_willmsgreq;

  const char *willtopic =
      "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea taki";
  uint8_t qos = 0;
  bool retain = false;
  const char *willmsg = "some will message";

  EXPECT_CALL(mqtt_sn_receiver, receive_connack(_)).WillOnce(check_connack(expected_connack));

  EXPECT_CALL(mqtt_sn_receiver, receive_willmsgreq(_))
      .WillOnce(DoAll(check_willmsg_req(expected_willmsgreq),
                      InvokeUnrelatedFunction(&mqtt_sn_sender, &MqttSnTestMessageSender::send_willmsg,
                                              (const uint8_t *) willmsg, strlen(willmsg) + 1)));

  EXPECT_CALL(mqtt_sn_receiver, receive_willtopicreq(_))
      .WillOnce(DoAll(check_willtopicreq(expected_willtopicreq),
                      InvokeUnrelatedFunction(&mqtt_sn_sender, &MqttSnTestMessageSender::send_willtopic,
                                              willtopic, qos, retain)));

  mqtt_sn_sender.send_connect("Mqtt SN Testclient", 60, true, true);

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceConnectTest, Connect_withwill_tooshortWillTopic_disconnect) {

  test_willtopicreq expected_willtopicreq;
  test_disconnect expected_disconnect;

  const char *willtopic = "";
  uint8_t qos = 0;
  bool retain = false;

  EXPECT_CALL(mqtt_sn_receiver, receive_disconnect(_)).WillOnce(check_disconnect(expected_disconnect));

  EXPECT_CALL(mqtt_sn_receiver, receive_willtopicreq(_))
      .WillOnce(DoAll(check_willtopicreq(expected_willtopicreq),
                      InvokeUnrelatedFunction(&mqtt_sn_sender, &MqttSnTestMessageSender::send_willtopic,
                                              willtopic, qos, retain)));

  mqtt_sn_sender.send_connect("Mqtt SN Testclient", 60, true, true);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceConnectTest, Connect_withwill_tooLongWillTopic_disconnect) {

  test_willtopicreq expected_willtopicreq;
  test_disconnect expected_disconnect;

  const char *willtopic =
      "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takim"; // 252 characters
  uint8_t qos = 0;
  bool retain = false;

  EXPECT_CALL(mqtt_sn_receiver, receive_disconnect(_)).WillOnce(check_disconnect(expected_disconnect));

  EXPECT_CALL(mqtt_sn_receiver, receive_willtopicreq(_))
      .WillOnce(DoAll(check_willtopicreq(expected_willtopicreq),
                      InvokeUnrelatedFunction(&mqtt_sn_sender, &MqttSnTestMessageSender::send_willtopic,
                                              willtopic, qos, retain)));

  mqtt_sn_sender.send_connect("Mqtt SN Testclient", 60, true, true);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceConnectTest, Connect_withwill_emptyWillmessageLength_return_accepted) {

  test_connack expected_connack(TEST_ACCEPTED);
  test_willtopicreq expected_willtopicreq;
  test_willmsgreq expected_willmsgreq;

  const char *willtopic = "/some/will/topic";
  uint8_t qos = 0;
  bool retain = false;
  const char *willmsg = "";

  EXPECT_CALL(mqtt_sn_receiver, receive_connack(_)).WillOnce(check_connack(expected_connack));

  EXPECT_CALL(mqtt_sn_receiver, receive_willmsgreq(_))
      .WillOnce(DoAll(check_willmsg_req(expected_willmsgreq),
                      InvokeUnrelatedFunction(&mqtt_sn_sender, &MqttSnTestMessageSender::send_willmsg,
                                              (const uint8_t *) willmsg, strlen(willmsg) + 1)));

  EXPECT_CALL(mqtt_sn_receiver, receive_willtopicreq(_))
      .WillOnce(DoAll(check_willtopicreq(expected_willtopicreq),
                      InvokeUnrelatedFunction(&mqtt_sn_sender, &MqttSnTestMessageSender::send_willtopic,
                                              willtopic, qos, retain)));

  mqtt_sn_sender.send_connect("Mqtt SN Testclient", 60, true, true);

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceConnectTest, Connect_withwill_minimumWillmessageLength_return_accepted) {

  test_connack expected_connack(TEST_ACCEPTED);
  test_willtopicreq expected_willtopicreq;
  test_willmsgreq expected_willmsgreq;

  const char *willtopic = "/some/will/topic";
  uint8_t qos = 0;
  bool retain = false;
  const char *willmsg = "M";

  EXPECT_CALL(mqtt_sn_receiver, receive_connack(_)).WillOnce(check_connack(expected_connack));

  EXPECT_CALL(mqtt_sn_receiver, receive_willmsgreq(_))
      .WillOnce(DoAll(check_willmsg_req(expected_willmsgreq),
                      InvokeUnrelatedFunction(&mqtt_sn_sender, &MqttSnTestMessageSender::send_willmsg,
                                              (const uint8_t *) willmsg, strlen(willmsg) + 1)));

  EXPECT_CALL(mqtt_sn_receiver, receive_willtopicreq(_))
      .WillOnce(DoAll(check_willtopicreq(expected_willtopicreq),
                      InvokeUnrelatedFunction(&mqtt_sn_sender, &MqttSnTestMessageSender::send_willtopic,
                                              willtopic, qos, retain)));

  mqtt_sn_sender.send_connect("Mqtt SN Testclient", 60, true, true);

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceConnectTest, Connect_withwill_maximumWillmessageLength_return_accepted) {

  test_connack expected_connack(TEST_ACCEPTED);
  test_willtopicreq expected_willtopicreq;
  test_willmsgreq expected_willmsgreq;

  const char *willtopic("/some/will/topic");
  uint8_t qos = 0;
  bool retain = false;
  const char *willmsg =
      "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takim"; // 253 = 252 characters + 1 (null-terminator)

  EXPECT_CALL(mqtt_sn_receiver, receive_connack(_)).WillOnce(check_connack(expected_connack));

  EXPECT_CALL(mqtt_sn_receiver, receive_willmsgreq(_))
      .WillOnce(DoAll(check_willmsg_req(expected_willmsgreq),
                      InvokeUnrelatedFunction(&mqtt_sn_sender, &MqttSnTestMessageSender::send_willmsg,
                                              (const uint8_t *) willmsg, strlen(willmsg) + 1)));

  EXPECT_CALL(mqtt_sn_receiver, receive_willtopicreq(_))
      .WillOnce(DoAll(check_willtopicreq(expected_willtopicreq),
                      InvokeUnrelatedFunction(&mqtt_sn_sender, &MqttSnTestMessageSender::send_willtopic,
                                              willtopic, qos, retain)));

  mqtt_sn_sender.send_connect("Mqtt SN Testclient", 60, true, true);

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  std::cout << std::endl;
}

TEST_P(MqttSnGatewayComplianceConnectTest, DISABLED_Connect_withwill_tooLongWillmessageLength_disconnect) {
  // this test cannot be performed, the will message is no char string with null terminator so 255 and any message is a valid will message equal to a will message with the maximum will message size.

  test_disconnect expected_disconnect;
  test_willtopicreq expected_willtopicreq;
  test_willmsgreq expected_willmsgreq;

  const char *willtopic("/some/will/topic");
  uint8_t qos = 0;
  bool retain = false;
  const char *willmsg =
      "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takima"; // 254 = 253 characters + 1 (null-terminator)

  EXPECT_CALL(mqtt_sn_receiver, receive_disconnect(_)).WillOnce(check_disconnect(expected_disconnect));

  EXPECT_CALL(mqtt_sn_receiver, receive_willmsgreq(_))
      .WillOnce(DoAll(check_willmsg_req(expected_willmsgreq),
                      InvokeUnrelatedFunction(&mqtt_sn_sender, &MqttSnTestMessageSender::send_willmsg,
                                              (const uint8_t *) willmsg, strlen(willmsg) + 1)));

  EXPECT_CALL(mqtt_sn_receiver, receive_willtopicreq(_))
      .WillOnce(DoAll(check_willtopicreq(expected_willtopicreq),
                      InvokeUnrelatedFunction(&mqtt_sn_sender, &MqttSnTestMessageSender::send_willtopic,
                                              willtopic, qos, retain)));

  mqtt_sn_sender.send_connect("Mqtt SN Testclient", 60, true, true);

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  std::cout << std::endl;
}
