//
// Created by bele on 26.02.19.
//

#include "ClientNetworkMessageDataGenerator.h"

uint8_t *defaultMessageDataGenerator(uint8_t *arr, uint16_t len) {
  memset(arr, 0, len * sizeof(uint8_t));
  for (uint16_t i = 0; i < len; i++) {
    arr[i] = static_cast<uint8_t>(i % UINT8_MAX);
  }
  return arr;
}
