//
// Created by SomeDude on 03.03.2019.
//

#include "MqttSnFixedSizeRingBufferTests.h"
#include <test/shared/MqttSnMessageDataGenerator/MqttSnMessageDataGenerator.h>
#include <test/shared/ComparableMqttSnForwaderStructures/ComparableMqttSnMessageData.h>

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_AfterInit_HeadIsZero) {
  EXPECT_EQ(ringBuffer.head, 0);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_AfterInit_TailIsZero) {
  EXPECT_EQ(ringBuffer.tail, 0);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_AfterInit_MaxlenIsMAXLEN) {
  EXPECT_EQ(ringBuffer.maxlen, CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_AfterInit_ItemsAreZeroed) {
  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    MqttSnMessageData zeroItem = {0};
    EXPECT_EQ(memcmp(&ringBuffer.items[i], &zeroItem, sizeof(MqttSnMessageData)), 0);
  }
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_AfterInit_IsEmptyReturnsOne) {
  EXPECT_EQ(isEmpty(&ringBuffer), 1);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_AfterInit_IsFullReturnsZero) {
  EXPECT_EQ(isFull(&ringBuffer), 0);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutOne_IsEmptyReturnsZero) {
  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
  MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);

  EXPECT_EQ(isEmpty(&ringBuffer), 0);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutOne_IsFullReturnsZero) {
  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
  MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);

  EXPECT_EQ(isFull(&ringBuffer), 0);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMoreThanMAXLEN_IsEmptyReturnsZero) {
  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
  MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }
  ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), -1);

  EXPECT_EQ(isEmpty(&ringBuffer), 0);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMoreThanMAXLEN_IsFullReturnsOne) {
  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
  MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }
  ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), -1);

  EXPECT_EQ(isFull(&ringBuffer), 1);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMAXLENPopMAXLEN_IsEmptyReturnsOne) {
  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
  MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }

  MqttSnMessageData popMqttSnMessageData = {0};
  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);
  }

  EXPECT_EQ(isEmpty(&ringBuffer), 1);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMAXLENPopMAXLEN_IsFullReturnsZero) {
  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
  MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }

  MqttSnMessageData popMqttSnMessageData = {0};
  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);
  }

  EXPECT_EQ(isFull(&ringBuffer), 0);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutOneIn_PutReturnsZero) {
  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
  MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  EXPECT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutTwoIn_PutReturnsZeroRepeatedly) {
  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
  MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  EXPECT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutThreeIn_PutReturnsZeroRepeatedly) {
  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
  MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  EXPECT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMAXLENIn_PutReturnsZeroRepeatedly) {
  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
  MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN - 1; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }
  EXPECT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMoreThanMAXLENIn_PutReturnsMinusOne) {
  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
  MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }
  EXPECT_EQ(put(&ringBuffer, &mqttSnMessageData), -1);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMAXLENInPopOne_PopReturnsZero) {
  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
  MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }

  MqttSnMessageData popMqttSnMessageData = {0};
  EXPECT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMAXLENInPopTwo_PopReturnsZeroRepeatedly) {
  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
  MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }

  MqttSnMessageData popMqttSnMessageData = {0};
  ASSERT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);
  EXPECT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMAXLENInPopThree_PopReturnsZeroRepeatedly) {
  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
  MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }

  MqttSnMessageData popMqttSnMessageData = {0};
  ASSERT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);
  ASSERT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);
  EXPECT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMAXLENPopMAXLEN_PopReturnsZeroRepeatedly) {
  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
  MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }

  MqttSnMessageData popMqttSnMessageData = {0};
  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN - 1; i++) {
    ASSERT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);
  }
  EXPECT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMAXLENPopMAXLEN_AdditionalPopReturnsMinusOne) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
  MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }

  MqttSnMessageData popMqttSnMessageData = {0};
  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);
  }
  EXPECT_EQ(pop(&ringBuffer, &popMqttSnMessageData), -1);

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutOnePopOne_DataAreEqual) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  std::vector<MqttSnMessageData> mqttSnMessageDatas;
  for (uint32_t i = 0; i < 1; ++i) {
    MqttSnMessageData mqttSnMessageData = {0};
    MqttSnMessageDataGenerator::generateDeviceAddressFromUint32(&mqttSnMessageData.address, i);
    mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
    MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);
    mqttSnMessageDatas.push_back(mqttSnMessageData);
  }

  for (auto mqttSnMessageData : mqttSnMessageDatas) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }

  for (auto mqttSnMessageData : mqttSnMessageDatas) {
    MqttSnMessageData popMqttSnMessageData = {0};
    ASSERT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);
    ComparableMqttSnMessageData expected(mqttSnMessageData);
    ComparableMqttSnMessageData actual(popMqttSnMessageData);
    EXPECT_EQ(actual, expected);
  }

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutTwoPopTwo_DataAreEqual) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  std::vector<MqttSnMessageData> mqttSnMessageDatas;
  for (uint32_t i = 0; i < 2; ++i) {
    MqttSnMessageData mqttSnMessageData = {0};
    MqttSnMessageDataGenerator::generateDeviceAddressFromUint32(&mqttSnMessageData.address, i);
    mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
    MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);
    mqttSnMessageDatas.push_back(mqttSnMessageData);
  }

  for (auto mqttSnMessageData : mqttSnMessageDatas) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }

  for (auto mqttSnMessageData : mqttSnMessageDatas) {
    MqttSnMessageData popMqttSnMessageData = {0};
    ASSERT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);
    ComparableMqttSnMessageData expected(mqttSnMessageData);
    ComparableMqttSnMessageData actual(popMqttSnMessageData);
    EXPECT_EQ(actual, expected);
  }

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutThreePopThree_DataAreEqual) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  std::vector<MqttSnMessageData> mqttSnMessageDatas;
  for (uint32_t i = 0; i < 3; ++i) {
    MqttSnMessageData mqttSnMessageData = {0};
    MqttSnMessageDataGenerator::generateDeviceAddressFromUint32(&mqttSnMessageData.address, i);
    mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
    MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);
    mqttSnMessageDatas.push_back(mqttSnMessageData);
  }

  for (auto mqttSnMessageData : mqttSnMessageDatas) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }

  for (auto mqttSnMessageData : mqttSnMessageDatas) {
    MqttSnMessageData popMqttSnMessageData = {0};
    ASSERT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);
    ComparableMqttSnMessageData expected(mqttSnMessageData);
    ComparableMqttSnMessageData actual(popMqttSnMessageData);
    EXPECT_EQ(actual, expected);
  }

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMAXLENPopMAXLEN_DataAreEqual) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  std::vector<MqttSnMessageData> mqttSnMessageDatas;
  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; ++i) {
    MqttSnMessageData mqttSnMessageData = {0};
    MqttSnMessageDataGenerator::generateDeviceAddressFromUint32(&mqttSnMessageData.address, i);
    mqttSnMessageData.data_length = CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH;
    MqttSnMessageDataGenerator::generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);
    mqttSnMessageDatas.push_back(mqttSnMessageData);
  }

  for (auto mqttSnMessageData : mqttSnMessageDatas) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }

  for (auto mqttSnMessageData : mqttSnMessageDatas) {
    MqttSnMessageData popMqttSnMessageData = {0};
    ASSERT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);
    ComparableMqttSnMessageData expected(mqttSnMessageData);
    ComparableMqttSnMessageData actual(popMqttSnMessageData);
    EXPECT_EQ(actual, expected);
  }

}
