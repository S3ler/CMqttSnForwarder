//
// Created by SomeDude on 23.03.2019.
//
#include "MqttSnGatewayNetworkInterfaceMessageDefragmentationTests.h"

// TODO extra tests machen, vor allem für größere defragmentierungen
// TODO extra tests machen, die alle kombinationen durchtestet

// send by gateway => received by MqttSnForwarder
TEST_P(MqttSnGatewayNetworkInterfaceMessageDefragmentationTests, DefragmentationTests) {
  ASSERT_TRUE(gatewayNetworkForwarderLooper.pauseLoop());

  for (uint16_t messageCounter = 0; messageCounter < toTestMessageCount; ++messageCounter) {
    if (useIdentifier) {
      *mockGateway->getGatewayDeviceAddress() = getDeviceAddressFromNetworkContext(mockGateway->getIdentifier(),
                                                                                   gatewayNetworkContext);
    }
    ComparableGatewayMqttSnMessageData data(toTestMessageLength,
                                            mockGateway->getGatewayDeviceAddress(),
                                            mockGateway->getIdentifier(),
                                            useIdentifier);
    expectedMockGatewayMqttSnMessageDatas.push_back(data);
  }

  std::vector<uint8_t> v;
  for (auto data : expectedMockGatewayMqttSnMessageDatas) {
    v.insert(v.end(),
             std::make_move_iterator(data.data.begin()),
             std::make_move_iterator(data.data.end())
    );

  }

  uint16_t n = packetSize;
  device_address forwarderDeviceAddress = mockGateway->getForwarderDeviceAddress();
  uint64_t size = (v.size() - 1) / n + 1;
  std::vector<uint8_t> vec[size];
  for (uint64_t k = 0; k < size; ++k) {

    auto start_itr = std::next(v.cbegin(), k * n);
    auto end_itr = std::next(v.cbegin(), k * n + n);
    vec[k].resize(n);

    if (k * n + n > v.size()) {
      end_itr = v.cend();
      vec[k].resize(v.size() - k * n);
    }
    // copy elements from the input range to the sub-vector
    std::copy(start_itr, end_itr, vec[k].begin());
  }

  EXPECT_CALL(mockReceiveBuffer, put(&receiveBuffer, _))
      .Times(toTestMessageCount)
      .WillRepeatedly(Invoke(
          [this]
              (MqttSnFixedSizeRingBuffer *receiveBuffer,
               MqttSnMessageData *receiveData) -> int {
            ComparableGatewayMqttSnMessageData messageData(*receiveData, useIdentifier);
            actualMockGatewaySnMessageDatas.push_back(messageData);
            return 0;
          }
      ));

  for (uint64_t k = 0; k < size; ++k) {
    std::vector<uint8_t> subChunkData = vec[k];
    ASSERT_TRUE(gatewayNetworkForwarderLooper.pauseLoop());
    ASSERT_LE(subChunkData.size(), UINT16_MAX);
    ASSERT_EQ(mockGateway->send(&forwarderDeviceAddress,
                                &subChunkData[0],
                                static_cast<uint16_t>(subChunkData.size())), subChunkData.size());
    ASSERT_TRUE(gatewayNetworkForwarderLooper.resumeLoop());
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  EXPECT_THAT(actualMockGatewaySnMessageDatas,
              testing::UnorderedElementsAreArray(expectedMockGatewayMqttSnMessageDatas));
}
