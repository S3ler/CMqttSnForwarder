//
// Created by bele on 02.02.19.
//

#include <gtest/gtest.h>
#include <gmock/gmock-more-actions.h>
#include <gmock/gmock-cardinalities.h>
#include <gmock/gmock-matchers.h>
#include "../forwarder/MqttSnForwarder.h"
#include "../forwarder/network/gateway/tcp/MqttSnGatewayTcpNetwork.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Args;
using ::testing::SaveArg;
using ::testing::Invoke;
using ::testing::SetArgPointee;
using ::testing::Field;
using ::testing::DoAll;

class MqttSnForwarder_Tests : public ::testing::Test {
 public:
  virtual void SetUp() {

  }

  virtual void TearDown() {

  }

  MqttSnForwarder_Tests() : Test() {

  }

  virtual ~MqttSnForwarder_Tests() {

  }
};

TEST_F(MqttSnForwarder_Tests, asdf) {

}