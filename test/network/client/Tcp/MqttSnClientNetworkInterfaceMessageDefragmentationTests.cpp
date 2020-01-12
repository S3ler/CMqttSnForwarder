//
// Created by SomeDude on 25.03.2019.
//

#include "MqttSnClientNetworkInterfaceMessageDefragmentationTests.h"

TEST_P(MqttSnClientNetworkInterfaceMessageDefragmentationTests, DefragmentationTests
) {
ASSERT_TRUE(clientNetworkGatewayLooper
.
pauseLoop()
);

uint64_t global_size = 0;
global_size = (((uint64_t) toTestMessageCount * (uint64_t) toTestMessageLength) - 1) / toTestPacketSize + 1;
std::vector<std::vector<std::vector < uint8_t>>>
vecs;
for (
const auto &mockClient
: mockClients) {
std::vector<ComparableClientMqttSnMessageData> mockClientMqttSnMessageDatas;
for (
uint16_t messageCount = 0;
messageCount<toTestMessageCount;
++messageCount) {
// we send message in RoundRobin for each client
if (useIdentifier) {
*mockClient->
getClientDeviceAddress() =
getDeviceAddressFromNetworkContext(mockClient->getIdentifier(), clientNetworkContext);
}
ComparableClientMqttSnMessageData data(toTestMessageLength,
                                       mockClient->getClientDeviceAddress(),
                                       mockClient->getIdentifier(),
                                       useIdentifier);
expectedMockClientSnMessageDatas.
push_back(data);
mockClientMqttSnMessageDatas.
push_back(data);
}
std::vector<uint8_t> v;
for (
auto data
: mockClientMqttSnMessageDatas) {
v.
insert(v
.
end(),
    std::make_move_iterator(data.data.begin()),
    std::make_move_iterator(data.data.end())
);
}
/*
     uint16_t n = toTestPacketSize;
  uint64_t size = (v.size() - 1) / n + 1;
  ASSERT_EQ(size, global_size);     // TODO test if size is ok
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
  std::vector<std::vector<uint8_t>> vecList;
  for (std::vector<uint8_t> v : vec) {
    vecList.push_back(v);
  }
  vecs.push_back(vecList);
 */
uint16_t n = toTestPacketSize;
uint64_t size = (v.size() - 1) / n + 1;
ASSERT_EQ(size, global_size
);
std::vector<std::vector<uint8_t>> vec;
for (
uint64_t k = 0;
k<size;
++k) {

auto start_itr = std::next(v.cbegin(), k * n);
auto end_itr = std::next(v.cbegin(), k * n + n);
vec[k].
resize(n);

if (
k *n
+ n > v.
size()
) {
end_itr = v.cend();
vec[k].
resize(v
.
size()
-
k *n
);
}
// copy elements from the input range to the sub-vector
std::copy(start_itr, end_itr, vec[k]
.
begin()
);
}
std::vector<std::vector<uint8_t>> vecList;
for (
std::vector<uint8_t> v
: vec) {
vecList.
push_back(v);
}
vecs.
push_back(vecList);
}

EXPECT_CALL(mockReceiveBuffer, put(&receiveBuffer, _)
)
.
Times(toTestMessageCount
* mockClients.
size()
)
.
WillRepeatedly(Invoke(
    [this]
        (MqttSnFixedSizeRingBuffer *receiveBuffer,
         MqttSnMessageData *receiveData) -> int {
      ComparableClientMqttSnMessageData data(*receiveData, useIdentifier);
      actualMockClientSnMessageDatas.push_back(data);
      return 0;
    }
)
);

ASSERT_TRUE(clientNetworkGatewayLooper
.
resumeLoop()
);

ASSERT_EQ(vecs
.
size(), mockClients
.
size()
) << "should never happen";

for (
uint64_t k = 0;
k<global_size;
++k) {
for (
uint64_t c = 0;
c<mockClients.
size();
++c) {
std::shared_ptr<MockClient> mockClient = mockClients[c];
device_address forwarderDeviceAddress = mockClient->getForwarderDeviceAddress();

std::vector<std::vector<uint8_t>> vec = vecs[c];
std::vector<uint8_t> subChunkData = vec[k];
ASSERT_LE(subChunkData
.
size(), UINT16_MAX
);
ASSERT_EQ(mockClient
->
send(&forwarderDeviceAddress,
    &subChunkData[0],
static_cast
<uint16_t>(subChunkData
.
size()
)), subChunkData.
size()
);
/*
ASSERT_LE(vecs[c][k].size(), UINT16_MAX);
ASSERT_EQ(mockClient->send(&forwarderDeviceAddress,
                     &vecs[c][k][0],
                     static_cast<uint16_t>(vecs[c][k].size())), vecs[c][k].size());
                           */

std::this_thread::sleep_for(std::chrono::milliseconds(100)
);
}
}

std::this_thread::sleep_for(std::chrono::milliseconds(1000)
);

EXPECT_THAT(actualMockClientSnMessageDatas, testing::UnorderedElementsAreArray(expectedMockClientSnMessageDatas)
);
}
