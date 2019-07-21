//
// Created by SomeDude on 21.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_COMPLIANCE_MQTTSNGATEWAYCOMPLIANCEDISCONNECTTEST_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_COMPLIANCE_MQTTSNGATEWAYCOMPLIANCEDISCONNECTTEST_H_

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
#include <test/MqttClientMessageTester/MqttTestMessageReceiverMock.h>
#include <test/MqttClientMessageTester/MqttMessageTester.h>
#include <test/MqttClientMessageTester/MqttMessageTestType.h>
#include <test/MqttClientMessageTester/MqttMessageTesterFactory.h>

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

class MqttSnGatewayComplianceDisconnectTest : public ::testing::TestWithParam<std::tuple<MqttBrokerTestType,
                                                                                         MqttSnGatewayTestType>> {

 public:
  shared_ptr<MqttBrokerTestContainerInterface> mqttBroker;

  shared_ptr<MqttSnGatewayTestContainerInterface> mqttSnGateway;

  shared_ptr<MqttSnMessageTester> mqttSnMessageTester;
  MqttSnTestMessageSender mqtt_sn_sender;
  MqttSnTestMessageReceiverMock mqtt_sn_receiver;

  shared_ptr<MqttMessageTester> mqttMessageTester;
  MqttTestMessageReceiverMock mqtt_receiver;

  virtual void TearDown() {
    mqttSnMessageTester->stop();
    while (mqttSnMessageTester->isRunning()) {}
    mqtt_sn_sender.setTester(nullptr);
    mqttSnMessageTester = nullptr;

    mqttSnGateway->stop();
    while (mqttSnGateway->isRunning()) {}
    mqttSnGateway = nullptr;

    mqttMessageTester->stop();
    while (mqttMessageTester->isRunning()) {}
    mqttMessageTester = nullptr;

    mqttBroker->stop_broker();
    while (mqttBroker->isRunning()) {}
    mqttBroker = nullptr;
  }

  virtual void SetUp() {
    mqttBroker = MqttBrokerTestContainerFactory::getMqttBroker(std::get<0>(GetParam()));
    ASSERT_NE(mqttBroker.get(), nullptr);
    ASSERT_TRUE(mqttBroker->start_broker());

    mqttMessageTester = MqttMessageTesterFactory::getMqttMessageTester(MqttMessageTestType::PAHOC);
    ASSERT_NE(mqttMessageTester, nullptr);
    ASSERT_TRUE(mqttMessageTester->start());

    mqttSnGateway = MqttSnGatewayTestContainerFactory::getMqttSnGateway(std::get<1>(GetParam()));
    ASSERT_NE(mqttSnGateway, nullptr);
    ASSERT_EQ(mqttSnGateway->initialize(), EXIT_SUCCESS);
    ASSERT_EQ(mqttSnGateway->start(), EXIT_SUCCESS);

    mqttSnMessageTester = std::make_shared<MqttSnMessageTester>();
    mqtt_sn_sender.setTester(mqttSnMessageTester.get());
    mqttSnMessageTester->SetReceiver(&mqtt_sn_receiver);

    // TODO factory?
    static int32_t i = 0;
    string tester_cmd = "-db --log_identifier MqttSnTester -p 1000" + to_string(i) + " -gp 2000" + to_string(i);
    ASSERT_TRUE(mqttSnMessageTester->SetConfiguration("MqttSnMessageTester", tester_cmd));
    ASSERT_TRUE(mqttSnMessageTester->InitializeGatewayNetwork());

    ASSERT_TRUE(mqttSnMessageTester->start());

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  }

};

struct PrintToStringMqttSnGatewayComplianceDisconnectTestValueName {
  template<class ParamType>
  std::string operator()(const ::testing::TestParamInfo<ParamType> &info) const {
    std::string result = std::string(getTextFromEnum(std::get<0>(info.param))) + "_"
        + std::string(getTextFromEnum(std::get<1>(info.param)));
    return result;
  }
};

INSTANTIATE_TEST_SUITE_P(MqttSnGatewayComplianceDisconnectCompareTest,
                         MqttSnGatewayComplianceDisconnectTest,
                         testing::Combine(
                             testing::Values(MqttBrokerTestType::MOSQUITTO_DOCKER),
                             testing::Values(MqttSnGatewayTestType::ARSMB, MqttSnGatewayTestType::PAHO)),
                         PrintToStringMqttSnGatewayComplianceDisconnectTestValueName());

#endif //CMQTTSNFORWARDER_TEST_MQTTSNGATEWAY_COMPLIANCE_MQTTSNGATEWAYCOMPLIANCEDISCONNECTTEST_H_
