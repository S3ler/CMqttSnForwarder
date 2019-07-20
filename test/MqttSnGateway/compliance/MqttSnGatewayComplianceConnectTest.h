//
// Created by SomeDude on 19.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_COMPLIANCE_MQTTSNGATEWAYCOMPLIANCECONNECTTEST_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_COMPLIANCE_MQTTSNGATEWAYCOMPLIANCECONNECTTEST_H_

#include <gtest/gtest.h>
#include <memory>
#include <gmock/gmock.h>
#include <list>
#include <test/MqttBroker/MqttBrokerTestContainerInterface.hpp>
#include <test/MqttSnMessageTester/MqttSnMessageTester.hpp>
#include <test/MqttSnGateway/MqttSnGatewayTestContainerInterface.h>
#include <test/MqttBroker/MqttBrokerMosquittoDockerTestContainer.h>
#include <test/MqttSnGateway/MqttSnGatewayArsmbTestContainer.h>
#include <test/MqttSnGateway/MqttSnGatewayTestType.h>
#include <test/MqttSnGateway/MqttSnGatewayTestContainerFactory.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Args;
using ::testing::SaveArg;
using ::testing::Invoke;
using ::testing::SetArgPointee;
using ::testing::Field;
using ::testing::DoAll;

using std::shared_ptr;
using std::to_string;

class MqttSnGatewayComplianceConnectTest : public ::testing::TestWithParam<MqttSnGatewayTestType> {

 public:
  shared_ptr<MqttBrokerTestContainerInterface> mqttBroker;

  shared_ptr<MqttSnGatewayTestContainerInterface> mqttSnGateway;

  shared_ptr<MqttSnMessageTester> mqttSnMessageTester;
  MqttSnTestMessageSender mqtt_sn_sender;
  MqttSnTestMessageReceiverMock mqtt_sn_receiver;

  /*
  std::list<std::string> predefined_topics;
  std::list<std::string> receiver_topics;
  predefined_topics.push_back(std::string("50 /unsubscribed/client/topic/name"));
  predefined_topics.push_back(std::string("20 /another/predefined/topic"));
  receiver_topics.push_back(std::string("/unsubscribed/client/topic/name"));
  */
  virtual void TearDown() {
    mqttSnMessageTester->stop();
    while (mqttSnMessageTester->is_running()) {}
    mqtt_sn_sender.setTester(nullptr);
    mqttSnMessageTester = nullptr;

    mqttSnGateway->stop();
    while (mqttSnGateway->isRunning()) {}
    mqttSnGateway = nullptr;

    mqttBroker->stop_broker();
    while (mqttBroker->isRunning()) {}
    mqttBroker = nullptr;
  }

  virtual void SetUp() {

    mqttBroker = std::make_shared<MqttBrokerMosquittoDockerTestContainer>();
    mqttBroker->start_broker();

    mqttSnGateway = MqttSnGatewayTestContainerFactory::getMqttSnGateway(GetParam());
    ASSERT_NE(mqttSnGateway, nullptr);
    ASSERT_EQ(mqttSnGateway->initialize(), EXIT_SUCCESS);
    ASSERT_EQ(mqttSnGateway->start(), EXIT_SUCCESS);

    mqttSnMessageTester = std::make_shared<MqttSnMessageTester>();
    mqtt_sn_sender.setTester(mqttSnMessageTester.get());
    mqttSnMessageTester->SetReceiver(&mqtt_sn_receiver);

    static int32_t i = 0;
    string tester_cmd = "-db --log_identifier MqttSnTester -p 1000" + to_string(i) + " -gp 2000" + to_string(i);
    ASSERT_TRUE(mqttSnMessageTester->SetConfiguration("MqttSnMessageTester", tester_cmd));
    ASSERT_TRUE(mqttSnMessageTester->InitializeGatewayNetwork());

    ASSERT_TRUE(mqttSnMessageTester->start());
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  }

 public:
  virtual ~MqttSnGatewayComplianceConnectTest() {

  }

};

MqttSnGatewayTestType mqttSnGatewayTestTypes[] = {MqttSnGatewayTestType::ARSMB, MqttSnGatewayTestType::PAHO};
struct PrintToStringMqttSnGatewayComplianceConnectTestValueName {
  template<class ParamType>
  std::string operator()(const ::testing::TestParamInfo<ParamType> &info) const {
    std::string result = getTextFromEnum(info.param);
    return result;
  }
};
INSTANTIATE_TEST_SUITE_P(MqttSnGatewayComplianceConnectCompareTest,
                         MqttSnGatewayComplianceConnectTest,
                         testing::ValuesIn(mqttSnGatewayTestTypes),
                         PrintToStringMqttSnGatewayComplianceConnectTestValueName());

ACTION_P(check_disconnect, expected) {
  ASSERT_EQ(expected.length, arg0->length);
  ASSERT_EQ(expected.type, arg0->type);
}

ACTION_P(check_connack, expected) {
  ASSERT_EQ(expected.length, arg0->length);
  ASSERT_EQ(expected.type, arg0->type);
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

#endif //CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_COMPLIANCE_MQTTSNGATEWAYCOMPLIANCECONNECTTEST_H_
