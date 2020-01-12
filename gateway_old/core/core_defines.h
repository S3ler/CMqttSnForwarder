#ifndef GATEWAY_CORE_DEFINES_H
#define GATEWAY_CORE_DEFINES_H
#include <stdint.h>

enum CORE_RESULT : int16_t {
    SUCCESS = 1,
    ZERO = 0,
    CLIENTNONEXISTENCE = -1,
    CLIENTSUBSCRIPTIONFULL = -2,
    TOPICIDNONEXISTENCE = -3,
    FULL = -4,
    MOREPUBLISHESAVAILABLE = -5,
    TOOMUCHDATA = -6,
};


enum CLIENT_STATUS : uint8_t {
    EMPTY = 0,
    ACTIVE = 1,
    ASLEEP = 2,
    AWAKE = 3,
    DISCONNECTED = 4,
    LOST = 5,
};


#endif //GATEWAY_DATABASE_DEFINES_H
