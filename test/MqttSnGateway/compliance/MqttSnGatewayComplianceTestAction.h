//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_COMPLIANCE_MQTTSNGATEWAYCOMPLIANCETESTACTION_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_COMPLIANCE_MQTTSNGATEWAYCOMPLIANCETESTACTION_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <arpa/inet.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Args;
using ::testing::SaveArg;
using ::testing::Invoke;
using ::testing::SetArgPointee;
using ::testing::Field;
using ::testing::DoAll;

ACTION_P(check_disconnect, expected) {
  ASSERT_EQ(expected.length, arg0->length);
  ASSERT_EQ(expected.type, arg0->type);
}

ACTION_P(check_connack, expected) {
  ASSERT_EQ(expected.length, arg0->length);
  ASSERT_EQ(expected.type, arg0->type);
  ASSERT_EQ(expected.return_code, arg0->return_code);
}

ACTION_P(check_puback, expected) {
  ASSERT_EQ(expected.length, arg0->length);
  ASSERT_EQ(expected.type, arg0->type);
  ASSERT_EQ(expected.topic_id, ntohs(arg0->topic_id));
  ASSERT_EQ(expected.msg_id, ntohs(arg0->msg_id));
  ASSERT_EQ(expected.return_code, arg0->return_code);
}

ACTION_P(check_willtopicreq, expected) {
  ASSERT_EQ(expected.length, arg0->length);
  ASSERT_EQ(expected.type, arg0->type);
}

ACTION_P(check_willmsg_req, expected) {
  ASSERT_EQ(expected.length, arg0->length);
  ASSERT_EQ(expected.type, arg0->type);
}
ACTION_P(check_regack, expected) {
  ASSERT_EQ(expected.length, arg0->length);
  ASSERT_EQ(expected.type, arg0->type);
  ASSERT_EQ(expected.msg_id, arg0->msg_id);
  ASSERT_EQ(expected.topic_id, ntohs(arg0->topic_id));
  ASSERT_EQ(expected.return_code, arg0->return_code);
}

ACTION_P(check_pubrel, expected) {
  ASSERT_EQ(expected.length, arg0->length);
  ASSERT_EQ(expected.type, arg0->type);
  ASSERT_EQ(expected.msg_id, arg0->msg_id);
}

ACTION_P(check_suback, expected) {
  ASSERT_EQ(expected.length, arg0->length);
  ASSERT_EQ(expected.type, arg0->type);
  ASSERT_EQ(expected.flags, arg0->flags);
  ASSERT_EQ(expected.msg_id, arg0->msg_id);
  ASSERT_EQ(expected.topic_id, arg0->topic_id);
  ASSERT_EQ(expected.return_code, arg0->return_code);
}

ACTION_P(check_publish, expected) {
  ASSERT_EQ(expected.length, arg0->length);
  ASSERT_EQ(expected.type, arg0->type);
  ASSERT_EQ(expected.flags, arg0->flags);
  ASSERT_EQ(expected.topic_id, arg0->topic_id);
  ASSERT_EQ(expected.message_id, arg0->message_id);
  ASSERT_THAT(std::vector<uint8_t>(arg0->data, arg0->data + (arg0->length - 7)),
              ::testing::ElementsAreArray(expected.data, expected.length - 7));
}
ACTION_P2(InvokeUnrelatedFunction, classPointer, pointerToMemberFunc) {
  (classPointer->*pointerToMemberFunc)();
}

ACTION_P3(InvokeUnrelatedFunction, classPointer, pointerToMemberFunc, first) {
  (classPointer->*pointerToMemberFunc)(first);
}

ACTION_P4(InvokeUnrelatedFunction, classPointer, pointerToMemberFunc, first, second) {
  (classPointer->*pointerToMemberFunc)(first, second);
}

ACTION_P5(InvokeUnrelatedFunction, classPointer, pointerToMemberFunc, first, second, third) {
  (classPointer->*pointerToMemberFunc)(first, second, third);
}

ACTION_P6(InvokeUnrelatedFunction, classPointer, pointerToMemberFunc, first, second, third, fourth) {
  (classPointer->*pointerToMemberFunc)(first, second, third, fourth);
}

#endif //CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_COMPLIANCE_MQTTSNGATEWAYCOMPLIANCETESTACTION_H_
