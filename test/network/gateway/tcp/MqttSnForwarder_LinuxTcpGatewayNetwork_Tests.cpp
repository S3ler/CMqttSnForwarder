//
// Created by bele on 02.02.19.
//

#include <gtest/gtest.h>
#include <gmock/gmock-more-actions.h>
#include <gmock/gmock-cardinalities.h>
#include <gmock/gmock-matchers.h>
#include "../../../../forwarder/network/gateway/tcp/MqttSnGatewayTcpNetwork.h"
#include "../../../../forwarder/MqttSnForwarder.h"
#include "../MockGatewayNetwork/MockGateway.h"
#include "../MockGatewayNetwork/MockGatewayNetworkReceiver.h"
#include "../MockGatewayNetwork/tcp/MockGatewayLinuxTcpNetworkImplementation.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Args;
using ::testing::SaveArg;
using ::testing::Invoke;
using ::testing::SetArgPointee;
using ::testing::Field;
using ::testing::DoAll;

class MqttSnForwarder_LinuxTcpGatewayNetwork_Tests : public ::testing::Test {
 public:
  uint16_t MqttSnGatewayPort = 8888;
  std::string MqttSnGatewayAddress = "127.0.0.1";
  MqttSnGatewayNetworkInterface mqttSnGatewayNetworkInterface;
  MqttSnGatewayTcpNetwork gatewayNetworkContext;
 public:
  MockGateway mockGateway;
  MockGatewayNetworkReceiver mockGatewayNetworkReceiver;
  MockGatewayLinuxTcpNetworkImplementation mockGatewayLinuxTcpNetworkImplementation;
  uint16_t mockGatewayPort = MqttSnGatewayPort;
  device_address mockGatewayDeviceAddress;

  virtual void SetUp() {
    initGatewaySocketContext();

    mockGatewayDeviceAddress = {0};

    mockGatewayDeviceAddress.bytes[0] = 127;
    mockGatewayDeviceAddress.bytes[1] = 0;
    mockGatewayDeviceAddress.bytes[2] = 0;
    mockGatewayDeviceAddress.bytes[3] = 1;

    mockGatewayDeviceAddress.bytes[4] = static_cast<uint8_t>(mockGatewayPort >> 8);
    mockGatewayDeviceAddress.bytes[5] = static_cast<uint8_t>(mockGatewayPort >> 0);
    mockGateway.setNetworkAddress(&mockGatewayDeviceAddress);
    mockGateway.setMockGatewayNetworkReceiverInterface(&mockGatewayNetworkReceiver);
    mockGateway.setMockGatewayNetworkInterface(&mockGatewayLinuxTcpNetworkImplementation);
    ASSERT_TRUE(mockGateway.start_loop());
  }

  void initGatewaySocketContext() {
    gatewayNetworkContext = {0};
    gatewayNetworkContext.my_socket = 0;
    gatewayNetworkContext.ip = const_cast<char *>(MqttSnGatewayAddress.c_str());
    gatewayNetworkContext.port = MqttSnGatewayPort;
  }

  virtual void TearDown() {
    close(gatewayNetworkContext.my_socket);

    mockGateway.stop_loop();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

  }

  MqttSnForwarder_LinuxTcpGatewayNetwork_Tests() : Test() {

  }

  virtual ~MqttSnForwarder_LinuxTcpGatewayNetwork_Tests() {

  }

};

ACTION_P(check_receive_any_data, expected) {
  uint8_t *data = arg0;
  uint16_t data_length = arg1;
  std::vector<uint16_t> actualDataVector;
  actualDataVector.assign(data, data + data_length);
  EXPECT_THAT(expected, testing::UnorderedElementsAreArray(actualDataVector));
}
/*

TEST_F(MqttSnForwarder_LinuxTcpGatewayNetwork_Tests, GatewayNetworkInit_Success_ReturnsZero) {
  EXPECT_EQ(GatewayLinuxTcpInit(&mqttSnGatewayNetworkInterface, &gatewayNetworkContext), 0);
}

TEST_F(MqttSnForwarder_LinuxTcpGatewayNetwork_Tests, GatewayNetworkConnect_Success_ReturnsZero) {
  EXPECT_EQ(GatewayLinuxTcpConnect(&mqttSnGatewayNetworkInterface, &gatewayNetworkContext), 0);
}

TEST_F(MqttSnForwarder_LinuxTcpGatewayNetwork_Tests, GatewayNetworkConnect_Fails_ReturnsMinusOne) {
  mockGateway.stop_loop();
  // stop client and close socket
  EXPECT_EQ(GatewayLinuxTcpConnect(&mqttSnGatewayNetworkInterface, &gatewayNetworkContext), -1);
}

TEST_F(MqttSnForwarder_LinuxTcpGatewayNetwork_Tests, GatewayLinuxTcpDisconnect_Gratefully_SocketIsZero) {
  GatewayLinuxTcpConnect(&mqttSnGatewayNetworkInterface, &gatewayNetworkContext);
  EXPECT_EQ(GatewayLinuxTcpConnect(&mqttSnGatewayNetworkInterface, &gatewayNetworkContext), 0);
}

TEST_F(MqttSnForwarder_LinuxTcpGatewayNetwork_Tests, GatewayLinuxTcpDisconnect_Ungratefully_SocketIsZero) {
  mockGateway.stop_loop();
  // GatewayLinuxTcpConnect(&mqttSnGatewayNetworkInterface, &gatewayNetworkContext);
  EXPECT_EQ(gatewayNetworkContext.my_socket, 0);
}

TEST_F(MqttSnForwarder_LinuxTcpGatewayNetwork_Tests, GatewayNetworkConnect_FiveBytesToSend_ReturnsZero) {
  ASSERT_EQ(GatewayLinuxTcpConnect(&mqttSnGatewayNetworkInterface, &gatewayNetworkContext), 0);

  uint16_t fiveByteDataLength = 5;
  uint8_t fiveByteData[5] = {1, 2, 3, 4, 5};

  std::vector<uint16_t> expectedDataVector;
  expectedDataVector.assign(fiveByteData, fiveByteData + fiveByteDataLength);

  MqttSnFixedSizeRingBuffer sendBuffer;
  MqttSnFixedSizeRingBufferInit(&sendBuffer);
  MqttSnMessageData toSendMessageData = {0};
  toSendMessageData.address = mockGatewayDeviceAddress;
  toSendMessageData.data_length = fiveByteDataLength;
  //toSend.data = fiveByteData;
  memcpy(toSendMessageData.data, fiveByteData, toSendMessageData.data_length);
  ASSERT_EQ(put(&sendBuffer, &toSendMessageData), 0);

  EXPECT_CALL(mockGatewayNetworkReceiver,
              receive_any_message(_, _)).WillOnce(check_receive_any_data(expectedDataVector));

  ASSERT_EQ(GatewayLinuxTcpSend(&mqttSnGatewayNetworkInterface, &sendBuffer, 1000, &gatewayNetworkContext), 0);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

TEST_F(MqttSnForwarder_LinuxTcpGatewayNetwork_Tests, GatewayNetworkConnect_MinimumBytesToSend_ReturnsZero) {
  ASSERT_EQ(GatewayLinuxTcpConnect(&mqttSnGatewayNetworkInterface, &gatewayNetworkContext), 0);

  uint16_t oneByteDataLength = 1;
  uint8_t oneByteData[1] = {1};

  std::vector<uint16_t> expectedDataVector;
  expectedDataVector.assign(oneByteData, oneByteData + oneByteDataLength);

  MqttSnFixedSizeRingBuffer sendBuffer;
  MqttSnFixedSizeRingBufferInit(&sendBuffer);
  MqttSnMessageData toSendMessageData = {0};
  toSendMessageData.address = mockGatewayDeviceAddress;
  toSendMessageData.data_length = oneByteDataLength;
  memcpy(toSendMessageData.data, oneByteData, toSendMessageData.data_length);
  ASSERT_EQ(put(&sendBuffer, &toSendMessageData), 0);

  EXPECT_CALL(mockGatewayNetworkReceiver,
              receive_any_message(_, _)).WillOnce(check_receive_any_data(expectedDataVector));

  ASSERT_EQ(GatewayLinuxTcpSend(&mqttSnGatewayNetworkInterface, &sendBuffer, 1000, &gatewayNetworkContext), 0);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

TEST_F(MqttSnForwarder_LinuxTcpGatewayNetwork_Tests, GatewayNetworkConnect_ZeroBytesToSend_ReturnsZero) {
  ASSERT_EQ(GatewayLinuxTcpConnect(&mqttSnGatewayNetworkInterface, &gatewayNetworkContext), 0);

  uint16_t zeroByteDataLength = 0;
  // uint8_t zeroByteData[0];

  MqttSnFixedSizeRingBuffer sendBuffer;
  MqttSnFixedSizeRingBufferInit(&sendBuffer);
  MqttSnMessageData toSendMessageData = {0};
  toSendMessageData.address = mockGatewayDeviceAddress;
  toSendMessageData.data_length = zeroByteDataLength;
  // memcpy(toSendMessageData.data, oneByteData, toSendMessageData.data_length);
  ASSERT_EQ(put(&sendBuffer, &toSendMessageData), 0);

  EXPECT_CALL(mockGatewayNetworkReceiver, receive_any_message(_, _)).Times(0);

  ASSERT_EQ(GatewayLinuxTcpSend(&mqttSnGatewayNetworkInterface, &sendBuffer, 1000, &gatewayNetworkContext), 0);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

TEST_F(MqttSnForwarder_LinuxTcpGatewayNetwork_Tests, GatewayNetworkConnect_MaximmumBytesSend_ReturnsZero) {
  ASSERT_EQ(GatewayLinuxTcpConnect(&mqttSnGatewayNetworkInterface, &gatewayNetworkContext), 0);

  uint16_t maximumByteDataLength = MockGatewayLinuxTcpNetworkImplementation_BUFFER_SIZE;
  uint8_t maximumByteData[MockGatewayLinuxTcpNetworkImplementation_BUFFER_SIZE] = {0};
  // generate some data
  for (uint16_t i = 0; i < maximumByteDataLength; i++) {
    maximumByteData[i] = i % UINT8_MAX;
  }

  std::vector<uint16_t> expectedDataVector;
  expectedDataVector.assign(maximumByteData, maximumByteData + maximumByteDataLength);

  MqttSnFixedSizeRingBuffer sendBuffer;
  MqttSnFixedSizeRingBufferInit(&sendBuffer);
  MqttSnMessageData toSendMessageData = {0};
  toSendMessageData.address = mockGatewayDeviceAddress;
  toSendMessageData.data_length = maximumByteDataLength;
  memcpy(toSendMessageData.data, maximumByteData, toSendMessageData.data_length);
  ASSERT_EQ(put(&sendBuffer, &toSendMessageData), 0);


  EXPECT_CALL(mockGatewayNetworkReceiver,
              receive_any_message(_, _)).WillOnce(check_receive_any_data(expectedDataVector));

  ASSERT_EQ(GatewayLinuxTcpSend(&mqttSnGatewayNetworkInterface, &sendBuffer, 1000, &gatewayNetworkContext), 0);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
*/
TEST_F(MqttSnForwarder_LinuxTcpGatewayNetwork_Tests, GatewayNetworkConnect_MaximmumBytesSend_ReturnsZero) {

}