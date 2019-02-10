//
// Created by bele on 02.02.19.
//


#include <gtest/gtest.h>
#include <gmock/gmock-more-actions.h>
#include <gmock/gmock-cardinalities.h>
#include <gmock/gmock-matchers.h>
#include "MockClientNetwork/tcp/MockClientLinuxTcpNetworkImplementation.h"
#include "../forwarder/network/client/tcp/MqttSnClientTcpNetwork.h"
#include "../forwarder/MqttSnClientNetworkInterface.h"
#include "MockClientNetwork/MockClientNetworkReceiver.h"
#include "MockClientNetwork/GatewayNetworkLooper.h"
#include "MockClientNetwork/MockClientMqttSnMessageData.h"
#include <list>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Args;
using ::testing::SaveArg;
using ::testing::Invoke;
using ::testing::SetArgPointee;
using ::testing::Field;
using ::testing::DoAll;

class MqttSnForwarder_LinuxTcpClientNetwork_Tests : public ::testing::Test {

 public:
  uint16_t MqttSnForwarderPort = 9999;
  std::string MqttSnForwarderAddressAsString = "127.0.0.1";
  uint8_t MqttSnForwarderAddress[4] = {127, 0, 0, 1};
  device_address forwarderAddress;
  MqttSnClientNetworkInterface mqttSnClientNetworkInterface;
  MqttSnClientTcpNetwork clientNetworkContext;
  MqttSnFixedSizeRingBuffer receiveBuffer;
  MqttSnFixedSizeRingBuffer sendBuffer;
 public:
  MockClient mockClient;
  MockClientNetworkReceiver mockClientNetworkReceiver;
  MockClientLinuxTcpNetworkImplementation mockClientLinuxTcpNetworkImplementation;
  device_address mockClientNetworkAddress;

  virtual void SetUp() {
    MqttSnFixedSizeRingBufferInit(&receiveBuffer);
    MqttSnFixedSizeRingBufferInit(&sendBuffer);

    mqttSnClientNetworkInterface = {0};
    clientNetworkContext = {0};
    clientNetworkContext.port = 9999;

    mockClientNetworkAddress = {0};

    forwarderAddress.bytes[0] = MqttSnForwarderAddress[0];
    forwarderAddress.bytes[1] = MqttSnForwarderAddress[1];
    forwarderAddress.bytes[2] = MqttSnForwarderAddress[2];
    forwarderAddress.bytes[3] = MqttSnForwarderAddress[3];

    forwarderAddress.bytes[4] = static_cast<uint8_t>(MqttSnForwarderPort >> 8);
    forwarderAddress.bytes[5] = static_cast<uint8_t>(MqttSnForwarderPort >> 0);

    /*
    mockClient.setNetworkAddress(&mockClientNetworkAddress);
    mockClient.setForwarderAddress(&forwarderAddress);
    mockClient.setMockClientNetworkInterface(&mockClientLinuxTcpNetworkImplementation);
    mockClient.setMockClientNetworkReceiverInterface(&mockClientNetworkReceiver);
    */
  }

  virtual void TearDown() {
    mockClient.stop_loop();

    close(clientNetworkContext.max_clients);
    for (uint32_t i = 0; i < clientNetworkContext.max_clients; i++) {
      close(clientNetworkContext.client_socket[i]);
    }
    clientNetworkContext = {0};
    mqttSnClientNetworkInterface = {0};
    forwarderAddress = {0};

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  MqttSnForwarder_LinuxTcpClientNetwork_Tests() : Test() {

  }

  virtual ~MqttSnForwarder_LinuxTcpClientNetwork_Tests() {

  }
};

ACTION_P(check_receive_any_data, expected) {
  uint8_t *data = arg0;
  uint16_t data_length = arg1;
  std::vector<uint16_t> actualDataVector;
  actualDataVector.assign(data, data + data_length);
  EXPECT_THAT(expected, testing::ElementsAreArray(actualDataVector));
  //
}
ACTION_P(IsDeviceAddressEqual, expected) {
  EXPECT_THAT(arg0, ElementsAreArray(expected));
}
ACTION_P(IsMqttSnMessageDataEqual, expected) {
  EXPECT_THAT(arg0.address, IsDeviceAddressEqual(expected.address));
  EXPECT_EQ(expected.data_length, arg0.data_length);
  EXPECT_THAT(arg0.data, testing::ElementsAreArray(expected.data));
}

// abhandln für jeden client in der expected listet
// common elements
// elements only in A
// elments only in B
//
std::string getDeviceAddressAsString(device_address address) {
  std::string result;
  for (uint16_t i = 0; i < sizeof(device_address); i++) {
    result.append(std::to_string(address.bytes[i]));
    if (i != sizeof(device_address) - 1) {
      result.append(", ");
    }
  }
  return result;
}

std::string getDataAsString(uint8_t* data, uint16_t data_length) {
  std::string result;
  for (uint16_t i = 0; i < data_length; i++) {
    result.append(std::to_string(data[i]));
    if (i != data_length - 1) {
      result.append(", ");
    }
  }
  return result;
}

// at least one time, a second time is ignored
ACTION_P(ContainsUnorderedListAllElements, expected) {
  // expected == list of MessageData
  // arg0 == list of MessageData
  for_each(expected.begin(), expected.end(), [&arg0](const MqttSnMessageData &mqttSnMessageData) {
    // in next line: [&mqttSnMessageData]
    auto it =
        std::find_if(arg0.begin(), arg0.end(), [&mqttSnMessageData](const MqttSnMessageData &arg0MqttSnMessageData) {
          return (memcmp(arg0MqttSnMessageData.address.bytes, mqttSnMessageData.address.bytes, sizeof(device_address)) == 0);
        });
    if (it != arg0.end()) {
      MqttSnMessageData i = *it;
      EXPECT_THAT(i, IsMqttSnMessageDataEqual(mqttSnMessageData));
    } else {
      // no expected message
      std::string clientAddress = getDeviceAddressAsString(mqttSnMessageData.address);
      std::string gtestMessage("Expected Message Missing: ");
      gtestMessage.append("From: ");
      gtestMessage.append(clientAddress);
      gtestMessage.append("Length: ");
      gtestMessage.append(std::to_string(mqttSnMessageData.data_length));
      gtestMessage.append(getDataAsString(mqttSnMessageData.data, mqttSnMessageData.data_length));
      EXPECT_TRUE(false) << gtestMessage;
    }

  });
}
/*
void checkContainsUnorderedListAllElements(std::list<MqttSnMessageData> expected, std::list<MqttSnMessageData> arg0){
  for_each(expected.begin(), expected.end(), [&arg0](const MqttSnMessageData &mqttSnMessageData) {
    // in next line: [&mqttSnMessageData]
    auto it =
        std::find_if(arg0.begin(), arg0.end(), [&mqttSnMessageData](const MqttSnMessageData &arg0MqttSnMessageData) {
          return (memcmp(arg0MqttSnMessageData.address.bytes, mqttSnMessageData.address.bytes, sizeof(device_address))
              == 0);
        });
    if (it != arg0.end()) {
      EXPECT_THAT(*it, IsMqttSnMessageDataEqual(mqttSnMessageData));
    } else {
      // no expected message
      std::string clientAddress = getDeviceAddressAsString(mqttSnMessageData.address);
      std::string gtestMessage("Expected Message Missing: ");
      gtestMessage.append("From: ");
      gtestMessage.append(clientAddress);
      gtestMessage.append("Length: ");
      gtestMessage.append(std::to_string(mqttSnMessageData.data_length));
      gtestMessage.append(getDataAsString((uint8_t *)&mqttSnMessageData.data, mqttSnMessageData.data_length));
      EXPECT_TRUE(false) << gtestMessage;
    }

  });
}
*/
/*
TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientLinuxTcpInit_Success_ReturnZero) {
  EXPECT_EQ(ClientLinuxTcpInit(&mqttSnClientNetworkInterface, &clientNetworkContext), 0);
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientLinuxTcpConnect_Success_ReturnZero) {
  // create a new ClientLinux on same port => fails
  ASSERT_EQ(ClientLinuxTcpInit(&mqttSnClientNetworkInterface, &clientNetworkContext), 0);

  EXPECT_EQ(ClientLinuxTcpConnect(&mqttSnClientNetworkInterface, &clientNetworkContext), 0);

}


*/
/*
// Init Tests
TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkInit_Success_ReturnsZero){
  FAIL() << "not implemented";
}
TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkInit_Fails_ReturnsMinusOne){
  FAIL() << "not implemented";
}

// Connect Tests
TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkConnect_Success_ReturnsZero){
  FAIL() << "not implemented";
}
TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkConnect_Fails_ReturnsMinusOne){
  FAIL() << "not implemented";
}

// Receive Tests
// one client
TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_MinimumBytesSendFromOneClient_MinimumBytesMessageReceivedFromOneClient){
  FAIL() << "not implemented";
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_FiveBytesSendFromOneClient_FiveBytesMessageReceivedFromOneClient){
  FAIL() << "not implemented";
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_MaximumBytesSendFromOneClient_MaximumBytesMessageReceivedFromOneClient){
  FAIL() << "not implemented";
}
// two clients
TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_MinimumBytesSendFromTwoClients_MinimumBytesMessageReceivedFromTwoClients){
  FAIL() << "not implemented";
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_FiveBytesSendFromTwoClients_FiveBytesMessageReceivedFromTwoClients){
  FAIL() << "not implemented";
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_MaximumBytesSendFromTwoClients_MaximumBytesMessageReceivedFromTwoClients){
  FAIL() << "not implemented";
}
// five clients
TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_MinimumBytesSendFromFiveClients_MinimumBytesMessageReceivedFromFiveClients){
  FAIL() << "not implemented";
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_FiveBytesSendFromFiveClients_FiveBytesMessageReceivedFromFiveClients){
  FAIL() << "not implemented";
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_MaximumBytesSendFromFiveClients_MaximumBytesMessageReceivedFromFiveClients){
  FAIL() << "not implemented";
}
// maximum client
TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_MinimumBytesSendFromMaximumClients_MinimumBytesMessageReceivedFromMaximumClients){
  FAIL() << "not implemented";
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_FiveBytesSendFromMaximumClients_FiveBytesMessageReceivedFromMaximumClients){
  FAIL() << "not implemented";
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_MaximumBytesSendFromMaximumClients_MaximumBytesMessageReceivedFromMaximumClients){
  FAIL() << "not implemented";
}

// Receive Tests
// one client
TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_MinimumBytesSendToOneClient_MinimumBytesMessageReceivedByOneClient){
  FAIL() << "not implemented";
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_FiveBytesSendToOneClient_FiveBytesMessageReceivedByOneClient){
  FAIL() << "not implemented";
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_MaximumBytesSendToOneClient_MaximumBytesMessageReceivedByOneClient){
  FAIL() << "not implemented";
}
// two clients
TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_MinimumBytesSendToTwoClients_MinimumBytesMessageReceivedByTwoClients){
  FAIL() << "not implemented";
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_FiveBytesSendToTwoClients_FiveBytesMessageReceivedByTwoClients){
  FAIL() << "not implemented";
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_MaximumBytesSendToTwoClients_MaximumBytesMessageReceivedByTwoClients){
  FAIL() << "not implemented";
}
// five clients
TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_MinimumBytesSendToFiveClients_MinimumBytesMessageReceivedByFiveClients){
  FAIL() << "not implemented";
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_FiveBytesSendToFiveClients_FiveBytesMessageReceivedByFiveClients){
  FAIL() << "not implemented";
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_MaximumBytesSendToFiveClients_MaximumBytesMessageReceivedByFiveClients){
  FAIL() << "not implemented";
}
// maximum client
TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_MinimumBytesSendToMaximumClients_MinimumBytesMessageReceivedByMaximumClients){
  FAIL() << "not implemented";
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientNetworkReceive_FiveBytesSendToMaximumClients_FiveBytesMessageReceivedByMaximumClients){
  FAIL() << "not implemented";
}
*/

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests,
       ClientNetworkReceive_MaximumBytesSendToMaximumClients_MaximumBytesMessageReceivedByMaximumClients) {

  ASSERT_EQ(ClientLinuxTcpInit(&mqttSnClientNetworkInterface, &clientNetworkContext), 0);

  ASSERT_EQ(ClientLinuxTcpConnect(&mqttSnClientNetworkInterface, &clientNetworkContext), 0);

  GatewayNetworkLooper gatewayNetworkLooper;
  ASSERT_TRUE(gatewayNetworkLooper.startNetworkLoop(ClientLinuxTcpReceive,
                                                    &mqttSnClientNetworkInterface,
                                                    &receiveBuffer,
                                                    1000,
                                                    &clientNetworkContext));

  //int toTestClientCounter = CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_CLIENTS;
  int toTestClientCounter = 1;

  MockClient mockClients[toTestClientCounter];
  MockClientNetworkReceiver mockClientNetworkReceivers[toTestClientCounter];
  MockClientLinuxTcpNetworkImplementation mockClientLinuxTcpNetworkImplementations[toTestClientCounter];
  device_address mockClientNetworkAddresses[toTestClientCounter];
  for (uint16_t i = 0; i < toTestClientCounter; i++) {
    mockClientNetworkAddresses[i] = {0};
    mockClients[i].setNetworkAddress(&mockClientNetworkAddresses[i]);
    mockClients[i].setForwarderAddress(&forwarderAddress);
    mockClients[i].setMockClientNetworkInterface(&mockClientLinuxTcpNetworkImplementations[i]);
    mockClients[i].setMockClientNetworkReceiverInterface(&mockClientNetworkReceivers[i]);
    ASSERT_TRUE(mockClients[i].start_loop());
  }

  // TODO tcp specific
  for (uint16_t i = 0; i < toTestClientCounter; i++) {
    ASSERT_TRUE(mockClientLinuxTcpNetworkImplementations[i].isNetworkConnected());
  }
  // TODO tcp specific

  uint16_t maximumByteDataLength = MAX_MESSAGE_LENGTH;
  uint8_t maximumByteData[maximumByteDataLength] = {0};
  // generate some data
  for (uint16_t i = 0; i < maximumByteDataLength; i++) {
    maximumByteData[i] = static_cast<uint8_t>(i % UINT8_MAX);
  }

  std::vector<uint16_t> expectedDataVector;
  expectedDataVector.assign(maximumByteData, maximumByteData + maximumByteDataLength);

  MqttSnMessageData toSendMessageData = {0};
  toSendMessageData.address = forwarderAddress;
  toSendMessageData.data_length = maximumByteDataLength;
  memcpy(toSendMessageData.data, maximumByteData, toSendMessageData.data_length);

  // setup finished
  // start test

  //std::list<MqttSnMessageData> expectedMqttSnMessageDataList;
  std::vector<MockClientMqttSnMessageData> expectedMockClientSnMessageDatas;
  for (uint16_t i = 0; i < toTestClientCounter; i++) {
    mockClients[i].send(maximumByteData, maximumByteDataLength);
    MqttSnMessageData mqttSnMessageData = toSendMessageData;
    mqttSnMessageData.address = (*mockClients[i].getNetworkAddress());
    // TODO check
    // TODO set network address
    MockClientMqttSnMessageData mockClientMqttSnMessageData(mqttSnMessageData);
    expectedMockClientSnMessageDatas.push_back(mockClientMqttSnMessageData);
    //expectedMqttSnMessageDataList.push_back(mqttSnMessageData);
  }
  // give some time for messages to arrive
  std::this_thread::sleep_for(std::chrono::milliseconds(toTestClientCounter * 10));

  //std::list<MqttSnMessageData> actualMqttSnMessageDataList;
  std::vector<MockClientMqttSnMessageData> actualMockClientSnMessageDatas;
  MqttSnMessageData messageData;
  while (pop(&receiveBuffer, &messageData)) {
    MockClientMqttSnMessageData mockClientMqttSnMessageData(messageData);
    actualMockClientSnMessageDatas.push_back(mockClientMqttSnMessageData);
    //actualMqttSnMessageDataList.push_back(messageData);
  }

  EXPECT_THAT(actualMockClientSnMessageDatas,testing::UnorderedElementsAreArray(expectedMockClientSnMessageDatas));
  // checkContainsUnorderedListAllElements(expectedMqttSnMessageDataList, actualMqttSnMessageDataList);
  /*
  MqttSnMessageData expectedMqttSnMessageDataArray[expectedMqttSnMessageDataList.size()];
  std::copy(expectedMqttSnMessageDataList.begin(), expectedMqttSnMessageDataList.end(), expectedMqttSnMessageDataArray);
  MqttSnMessageData actualMqttSnMessageDataArray[actualMqttSnMessageDataList.size()];
  std::copy(actualMqttSnMessageDataList.begin(), actualMqttSnMessageDataList.end(), actualMqttSnMessageDataArray);
  MqttSnMessageData* b = expectedMqttSnMessageDataArray;
  EXPECT_THAT(actualMqttSnMessageDataArray, testing::ElementsAreArray(b, expectedMqttSnMessageDataList.size()));
  */
  //EXPECT_THAT(actualMqttSnMessageDataList, ContainsUnorderedListAllElements(expectedMqttSnMessageDataList));
  // TODO alternativ könnte man dein MQTTSnFixedSizeRingBuffer mocken und ein expected put machen - ist sogar besser


  //EXPECT_THAT(actualMqttSnMessageDataList, testing::ElementsAreArray(expectedMqttSnMessageDataList));
  /*for (uint16_t i = 0; i < toTestClientCounter; i++) {
    EXPECT_CALL(mockClientNetworkReceivers[i],
                receive_any_message(_, _)).WillOnce(check_receive_any_data(expectedDataVector));
  }*/



  /*for (uint16_t i = 0; i < toTestClientCounter; i++) {
    // set expected
    mockClients[i].send(maximumByteDataLength, maximumByteDataLength);
  }*/

  // teardown
  for (uint16_t i = 0; i < toTestClientCounter; i++) {
    mockClients[i].stop_loop();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  gatewayNetworkLooper.stopNetworkLoop();
  ClientLinuxTcpDisconnect(&mqttSnClientNetworkInterface, &clientNetworkContext);
  std::this_thread::sleep_for(std::chrono::milliseconds(toTestClientCounter * 10));
}

#ifdef CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_H

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests,
       ClientNetworkReceive_OneClientConnect_OneClientConnectionEstablished) {
  ASSERT_EQ(ClientLinuxTcpInit(&mqttSnClientNetworkInterface, &clientNetworkContext), 0);

  ASSERT_EQ(ClientLinuxTcpConnect(&mqttSnClientNetworkInterface, &clientNetworkContext), 0);

  GatewayNetworkLooper gatewayNetworkLooper;
  ASSERT_TRUE(gatewayNetworkLooper.startNetworkLoop(ClientLinuxTcpReceive,
                                                    &mqttSnClientNetworkInterface,
                                                    &receiveBuffer,
                                                    1000,
                                                    &clientNetworkContext));

  int toTestClientCounter = 1;

  MockClient mockClients[toTestClientCounter];
  MockClientNetworkReceiver mockClientNetworkReceivers[toTestClientCounter];
  MockClientLinuxTcpNetworkImplementation mockClientLinuxTcpNetworkImplementations[toTestClientCounter];
  device_address mockClientNetworkAddresses[toTestClientCounter];
  for (uint16_t i = 0; i < toTestClientCounter; i++) {
    mockClientNetworkAddresses[i] = {0};
    mockClients[i].setNetworkAddress(&mockClientNetworkAddresses[i]);
    mockClients[i].setForwarderAddress(&forwarderAddress);
    mockClients[i].setMockClientNetworkInterface(&mockClientLinuxTcpNetworkImplementations[i]);
    mockClients[i].setMockClientNetworkReceiverInterface(&mockClientNetworkReceivers[i]);
    ASSERT_TRUE(mockClients[i].start_loop());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  for (uint16_t i = 0; i < toTestClientCounter; i++) {
    EXPECT_TRUE(mockClientLinuxTcpNetworkImplementations[i].isNetworkConnected());
  }

  gatewayNetworkLooper.stopNetworkLoop();
  for (uint16_t i = 0; i < toTestClientCounter; i++) {
    mockClients[i].stop_loop();
  }
  ClientLinuxTcpDisconnect(&mqttSnClientNetworkInterface, &clientNetworkContext);
  std::this_thread::sleep_for(std::chrono::milliseconds(toTestClientCounter * 100));
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests,
       ClientNetworkReceive_TwoClientsConnect_TwoClientConnectionsEstablished) {
  ASSERT_EQ(ClientLinuxTcpInit(&mqttSnClientNetworkInterface, &clientNetworkContext), 0);

  ASSERT_EQ(ClientLinuxTcpConnect(&mqttSnClientNetworkInterface, &clientNetworkContext), 0);

  GatewayNetworkLooper gatewayNetworkLooper;
  ASSERT_TRUE(gatewayNetworkLooper.startNetworkLoop(ClientLinuxTcpReceive,
                                                    &mqttSnClientNetworkInterface,
                                                    &receiveBuffer,
                                                    1000,
                                                    &clientNetworkContext));

  int toTestClientCounter = 2;

  MockClient mockClients[toTestClientCounter];
  MockClientNetworkReceiver mockClientNetworkReceivers[toTestClientCounter];
  MockClientLinuxTcpNetworkImplementation mockClientLinuxTcpNetworkImplementations[toTestClientCounter];
  device_address mockClientNetworkAddresses[toTestClientCounter];
  for (uint16_t i = 0; i < toTestClientCounter; i++) {
    mockClientNetworkAddresses[i] = {0};
    mockClients[i].setNetworkAddress(&mockClientNetworkAddresses[i]);
    mockClients[i].setForwarderAddress(&forwarderAddress);
    mockClients[i].setMockClientNetworkInterface(&mockClientLinuxTcpNetworkImplementations[i]);
    mockClients[i].setMockClientNetworkReceiverInterface(&mockClientNetworkReceivers[i]);
    ASSERT_TRUE(mockClients[i].start_loop());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  for (uint16_t i = 0; i < toTestClientCounter; i++) {
    EXPECT_TRUE(mockClientLinuxTcpNetworkImplementations[i].isNetworkConnected());
  }

  gatewayNetworkLooper.stopNetworkLoop();
  for (uint16_t i = 0; i < toTestClientCounter; i++) {
    mockClients[i].stop_loop();
  }

  ClientLinuxTcpDisconnect(&mqttSnClientNetworkInterface, &clientNetworkContext);
  std::this_thread::sleep_for(std::chrono::milliseconds(toTestClientCounter * 100));
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests,
       ClientNetworkReceive_FiveClientsConnect_FiveClientConnectionsEstablished) {
  ASSERT_EQ(ClientLinuxTcpInit(&mqttSnClientNetworkInterface, &clientNetworkContext), 0);

  ASSERT_EQ(ClientLinuxTcpConnect(&mqttSnClientNetworkInterface, &clientNetworkContext), 0);

  GatewayNetworkLooper gatewayNetworkLooper;
  ASSERT_TRUE(gatewayNetworkLooper.startNetworkLoop(ClientLinuxTcpReceive,
                                                    &mqttSnClientNetworkInterface,
                                                    &receiveBuffer,
                                                    1000,
                                                    &clientNetworkContext));

  int toTestClientCounter = 5;

  MockClient mockClients[toTestClientCounter];
  MockClientNetworkReceiver mockClientNetworkReceivers[toTestClientCounter];
  MockClientLinuxTcpNetworkImplementation mockClientLinuxTcpNetworkImplementations[toTestClientCounter];
  device_address mockClientNetworkAddresses[toTestClientCounter];
  for (uint16_t i = 0; i < toTestClientCounter; i++) {
    mockClientNetworkAddresses[i] = {0};
    mockClients[i].setNetworkAddress(&mockClientNetworkAddresses[i]);
    mockClients[i].setForwarderAddress(&forwarderAddress);
    mockClients[i].setMockClientNetworkInterface(&mockClientLinuxTcpNetworkImplementations[i]);
    mockClients[i].setMockClientNetworkReceiverInterface(&mockClientNetworkReceivers[i]);
    ASSERT_TRUE(mockClients[i].start_loop());
  }

  for (uint16_t i = 0; i < toTestClientCounter; i++) {
    EXPECT_TRUE(mockClientLinuxTcpNetworkImplementations[i].isNetworkConnected());
  }

  for (uint16_t i = 0; i < toTestClientCounter; i++) {
    mockClients[i].stop_loop();
  }

  gatewayNetworkLooper.stopNetworkLoop();
  ClientLinuxTcpDisconnect(&mqttSnClientNetworkInterface, &clientNetworkContext);
  std::this_thread::sleep_for(std::chrono::milliseconds(toTestClientCounter * 10));
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests,
       ClientNetworkReceive_MaximumClientsConnect_MaximumClientConnectionsEstablished) {
  ASSERT_EQ(ClientLinuxTcpInit(&mqttSnClientNetworkInterface, &clientNetworkContext), 0);

  ASSERT_EQ(ClientLinuxTcpConnect(&mqttSnClientNetworkInterface, &clientNetworkContext), 0);

  GatewayNetworkLooper gatewayNetworkLooper;
  ASSERT_TRUE(gatewayNetworkLooper.startNetworkLoop(ClientLinuxTcpReceive,
                                                    &mqttSnClientNetworkInterface,
                                                    &receiveBuffer,
                                                    1000,
                                                    &clientNetworkContext));

  int toTestClientCounter = CMQTTSNFORWARDER_MQTTSNCLIENTTCPNETWORK_MAX_CLIENTS;

  MockClient mockClients[toTestClientCounter];
  MockClientNetworkReceiver mockClientNetworkReceivers[toTestClientCounter];
  MockClientLinuxTcpNetworkImplementation mockClientLinuxTcpNetworkImplementations[toTestClientCounter];
  device_address mockClientNetworkAddresses[toTestClientCounter];
  for (uint16_t i = 0; i < toTestClientCounter; i++) {
    mockClientNetworkAddresses[i] = {0};
    mockClients[i].setNetworkAddress(&mockClientNetworkAddresses[i]);
    mockClients[i].setForwarderAddress(&forwarderAddress);
    mockClients[i].setMockClientNetworkInterface(&mockClientLinuxTcpNetworkImplementations[i]);
    mockClients[i].setMockClientNetworkReceiverInterface(&mockClientNetworkReceivers[i]);
    ASSERT_TRUE(mockClients[i].start_loop());
  }

  for (uint16_t i = 0; i < toTestClientCounter; i++) {
    EXPECT_TRUE(mockClientLinuxTcpNetworkImplementations[i].isNetworkConnected());
  }

  for (uint16_t i = 0; i < toTestClientCounter; i++) {
    mockClients[i].stop_loop();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  gatewayNetworkLooper.stopNetworkLoop();
  ClientLinuxTcpDisconnect(&mqttSnClientNetworkInterface, &clientNetworkContext);
  std::this_thread::sleep_for(std::chrono::milliseconds(toTestClientCounter * 10));
}

TEST_F(MqttSnForwarder_LinuxTcpClientNetwork_Tests, ClientLinuxTcpConnect_Fails_ReturnMinusOne) {

  uint16_t AlternativeMqttSnForwarderPort = MqttSnForwarderPort;
  MqttSnClientNetworkInterface AlternativeMqttSnClientNetworkInterface = {0};
  MqttSnClientTcpNetwork AlternativeClientNetworkContext = {0};
  AlternativeClientNetworkContext.port = AlternativeMqttSnForwarderPort;

  ASSERT_EQ(ClientLinuxTcpInit(&AlternativeMqttSnClientNetworkInterface, &AlternativeClientNetworkContext), 0);

  ASSERT_EQ(ClientLinuxTcpConnect(&AlternativeMqttSnClientNetworkInterface, &AlternativeClientNetworkContext), 0);

  ASSERT_EQ(ClientLinuxTcpInit(&mqttSnClientNetworkInterface, &clientNetworkContext), 0);

  EXPECT_EQ(ClientLinuxTcpConnect(&mqttSnClientNetworkInterface, &clientNetworkContext), -1);
}

#endif