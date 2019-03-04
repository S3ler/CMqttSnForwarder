//
// Created by SomeDude on 03.03.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERTESTS_H
#define CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERTESTS_H

#include <gtest/gtest.h>
#include "../../forwarder/MqttSnFixedSizeRingBuffer.h"
#include "ComparableMqttSnMessageData.h"

class MqttSnFixedSizeRingBufferTests : public ::testing::Test {
 public:

  void generateData(uint8_t *data, uint16_t data_length) {
    for (uint16_t i = 0; i < data_length; ++i) {
      data[i] = static_cast<uint8_t>(i % UINT8_MAX);
    }
  }

  void generateDeviceAddress(device_address *address, uint32_t address_number) {
    int16_t shift = 3;
    for (uint8_t i = 0; i < sizeof(device_address); ++i, shift = (static_cast<int16_t>(3 - i))) {
      if (shift >= 0) {
        address->bytes[i] = static_cast<uint8_t>(address_number >> (shift * 8));
      } else {
        address->bytes[i] = 0;
      }
    }
  }

};

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_AfterInitHeadIsZero_TestIsZero) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);
  EXPECT_EQ(ringBuffer.head, 0);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_AfterInitTailIsZero_TestIsZero) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);
  EXPECT_EQ(ringBuffer.tail, 0);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_AfterInitMaxlenIsMAXLEN_TestIsZero) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);
  EXPECT_EQ(ringBuffer.maxlen, CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_AfterInit_ItemsAreZeroed) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);
  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    MqttSnMessageData zeroItem = {0};
    EXPECT_EQ(memcmp(&ringBuffer.items[i], &zeroItem, sizeof(MqttSnMessageData)), 0);
  }
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_AfterInitIsEmpty_ReturnsOne) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  EXPECT_EQ(isEmpty(&ringBuffer), 1);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutOneIsEmpty_ReturnsZero) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = MAX_MESSAGE_LENGTH;
  generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);

  EXPECT_EQ(isEmpty(&ringBuffer), 0);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMoreThanMAXLENIsEmpty_IsMinusOne) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = MAX_MESSAGE_LENGTH;
  generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }
  ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), -1);

  EXPECT_EQ(isEmpty(&ringBuffer), 0);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMAXLENPopMAXLENIsEmpty_IsOne) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = MAX_MESSAGE_LENGTH;
  generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }

  MqttSnMessageData popMqttSnMessageData = {0};
  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);
  }

  EXPECT_EQ(isEmpty(&ringBuffer), 1);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutOneIn_IsZero) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = MAX_MESSAGE_LENGTH;
  generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  EXPECT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutTwoIn_IsZero) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = MAX_MESSAGE_LENGTH;
  generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  EXPECT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutThreeIn_IsZero) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = MAX_MESSAGE_LENGTH;
  generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  EXPECT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMAXLEN_IsZero) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = MAX_MESSAGE_LENGTH;
  generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN - 1; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }
  EXPECT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMoreThanMAXLEN_IsMinusOne) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = MAX_MESSAGE_LENGTH;
  generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }
  EXPECT_EQ(put(&ringBuffer, &mqttSnMessageData), -1);
}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMAXLENPopOne_IsZero) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = MAX_MESSAGE_LENGTH;
  generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }

  MqttSnMessageData popMqttSnMessageData = {0};
  EXPECT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMAXLENInPopTwo_IsZero) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = MAX_MESSAGE_LENGTH;
  generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }

  MqttSnMessageData popMqttSnMessageData = {0};
  ASSERT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);
  EXPECT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMAXLENInPopThree_IsZero) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = MAX_MESSAGE_LENGTH;
  generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }

  MqttSnMessageData popMqttSnMessageData = {0};
  ASSERT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);
  ASSERT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);
  EXPECT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMAXLENPopMAXLEN_IsZero) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = MAX_MESSAGE_LENGTH;
  generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN; i++) {
    ASSERT_EQ(put(&ringBuffer, &mqttSnMessageData), 0);
  }

  MqttSnMessageData popMqttSnMessageData = {0};
  for (uint32_t i = 0; i < CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFER_MAXLEN - 1; i++) {
    ASSERT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);
  }
  EXPECT_EQ(pop(&ringBuffer, &popMqttSnMessageData), 0);

}

TEST_F(MqttSnFixedSizeRingBufferTests, MqttSnFixedSizeRingBufferTests_PutMAXLENPopMAXLEN_IsMinusOne) {
  MqttSnFixedSizeRingBuffer ringBuffer;
  MqttSnFixedSizeRingBufferInit(&ringBuffer);

  MqttSnMessageData mqttSnMessageData = {0};
  mqttSnMessageData.data_length = MAX_MESSAGE_LENGTH;
  generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);

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
    generateDeviceAddress(&mqttSnMessageData.address, i);
    mqttSnMessageData.data_length = MAX_MESSAGE_LENGTH;
    generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);
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
    generateDeviceAddress(&mqttSnMessageData.address, i);
    mqttSnMessageData.data_length = MAX_MESSAGE_LENGTH;
    generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);
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
    generateDeviceAddress(&mqttSnMessageData.address, i);
    mqttSnMessageData.data_length = MAX_MESSAGE_LENGTH;
    generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);
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
    generateDeviceAddress(&mqttSnMessageData.address, i);
    mqttSnMessageData.data_length = MAX_MESSAGE_LENGTH;
    generateData(mqttSnMessageData.data, mqttSnMessageData.data_length);
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

#endif //CMQTTSNFORWARDER_MQTTSNFIXEDSIZERINGBUFFERTESTS_H
