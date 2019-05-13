//
// Created by SomeDude on 12.05.2019.
//
#include "platform_compatibility.h"
// this here so cmake can determine link language and code analysis works

int strcmp_P(const char *a, const char *b) {
  return strcmp(a, b);
}
