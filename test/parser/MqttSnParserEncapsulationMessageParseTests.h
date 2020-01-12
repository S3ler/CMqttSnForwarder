//
// Created by bele on 06.07.19.
//

#ifndef CMQTTSNFORWARDER_TEST_PARSER_MQTTSNPARSERENCAPSULATIONMESSAGEPARSETESTS_H_
#define CMQTTSNFORWARDER_TEST_PARSER_MQTTSNPARSERENCAPSULATIONMESSAGEPARSETESTS_H_

#include <gtest/gtest.h>
#include "gmock/gmock.h"

class MqttSnParserEncapsulationMessageParseTests : public ::testing::Test {
 protected:
  void SetUp() override;
  void TearDown() override;
};

#endif //CMQTTSNFORWARDER_TEST_PARSER_MQTTSNPARSERENCAPSULATIONMESSAGEPARSETESTS_H_
