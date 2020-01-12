//
// Created by SomeDude on 19.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_COMPLIANCE_MQTTSNGATEWAYCOMPLIANCECONNECTTEST_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_COMPLIANCE_MQTTSNGATEWAYCOMPLIANCECONNECTTEST_H_

#include <gtest/gtest.h>
#include <memory>
#include <gmock/gmock.h>
#include <list>
#include <test/MqttBroker/MqttBrokerTestContainerInterface.h>
#include <test/MqttSnMessageTester/MqttSnMessageTester.hpp>
#include <test/MqttSnGateway/MqttSnGatewayTestContainerInterface.h>
#include <test/MqttBroker/MqttBrokerMosquittoDockerTestContainer.h>
#include <test/MqttSnGateway/MqttSnGatewayArsmbTestContainer.h>
#include <test/MqttSnGateway/MqttSnGatewayTestType.h>
#include <test/MqttSnGateway/MqttSnGatewayTestContainerFactory.h>
#include <test/MqttBroker/MqttBrokerTestType.h>
#include <test/MqttBroker/MqttBrokerTestContainerFactory.h>

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

class MqttSnGatewayComplianceConnectTest
    : public ::testing::TestWithParam<std::tuple<MqttBrokerTestType, MqttSnGatewayTestType>> {

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
    while (mqttSnMessageTester->isRunning()) {}
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
    mqttBroker = MqttBrokerTestContainerFactory::getMqttBroker(std::get<0>(GetParam()));
    ASSERT_NE(mqttBroker.get(), nullptr);
    mqttBroker->start_broker();

    mqttSnGateway = MqttSnGatewayTestContainerFactory::getMqttSnGateway(std::get<1>(GetParam()));
    ASSERT_NE(mqttSnGateway, nullptr);
    //ASSERT_EQ(mqttSnGateway->initialize(), EXIT_SUCCESS);
    ASSERT_EQ(mqttSnGateway->start(), true);

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

};

struct PrintToStringMqttSnGatewayComplianceConnectTestValueName {
  template<class ParamType>
  std::string operator()(const ::testing::TestParamInfo<ParamType> &info) const {
    std::string result = std::string(getTextFromEnum(std::get<0>(info.param))) + "_"
        + std::string(getTextFromEnum(std::get<1>(info.param)));
    return result;
  }
};
INSTANTIATE_TEST_SUITE_P(MqttSnGatewayComplianceConnectCompareTest,
                         MqttSnGatewayComplianceConnectTest,
                         testing::Combine(
                             testing::Values(MqttBrokerTestType::MOSQUITTO_DOCKER),
                             testing::Values(MqttSnGatewayTestType::ARSMB, MqttSnGatewayTestType::PAHO)),
                         PrintToStringMqttSnGatewayComplianceConnectTestValueName());

#endif //CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_COMPLIANCE_MQTTSNGATEWAYCOMPLIANCECONNECTTEST_H_
