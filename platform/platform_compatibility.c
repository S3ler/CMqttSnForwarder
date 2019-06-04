//
// Created by SomeDude on 12.05.2019.
//
#include "platform_compatibility.h"
// this here so cmake can determine link language and code analysis works

int strcmp_P(const char *a, const char *b) {
  return strcmp(a, b);
}
int32_t PlatformCompatibilityGetTimestamp(uint64_t *t) {
  time_t result = time(NULL);
  if (result == -1) {
    return -1;
  }
  *t = result;
  return 0;
}
