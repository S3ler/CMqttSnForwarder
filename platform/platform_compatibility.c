//
// Created by SomeDude on 12.05.2019.
//
#include "platform_compatibility.h"
// this here so cmake can determine link language and code analysis works

#if defined(MQTT_SN_ARDUINO) || defined(Arduino_h) || defined(WITH_PLATFORMIO)  // Arduino
int32_t PlatformCompatibilityGetTimestampMs(uint64_t *t) {
    *t = millis();
    return 0;
}
#else

int strcmp_P(const char *a, const char *b) {
    return strcmp(a, b);
}
int32_t PlatformCompatibilityGetTimestampNs(uint64_t *t) {
    time_t result = time(NULL);
    if (result == -1) {
        return -1;
    }
    *t = result;
    return 0;
}
int32_t PlatformCompatibilityGetTimestampMs(uint64_t *t) {
    struct timespec spec;
    if (clock_gettime(CLOCK_REALTIME, &spec) == -1) {
        return -1;
    }
    (*t)  = (uint64_t)spec.tv_sec * (uint64_t)1000;
    (*t) += (uint64_t)spec.tv_nsec / (uint64_t)1.0e6;
    /*
    time_t result = time(NULL);
    if (result == -1) {
        return -1;
    }
    *t = result;
     */
    return 0;
}
int32_t PlatformCompatibilityGetTimestampS(uint64_t *t) {
    time_t result = time(NULL);
    if (result == -1) {
        return -1;
    }
    *t = result;
    return 0;
}
#endif
