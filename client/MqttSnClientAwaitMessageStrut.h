//
// Created by SomeDude on 25.08.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTAWAITMESSAGESTRUT_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTAWAITMESSAGESTRUT_H

#include <stdint.h>
#include <parser/MqttSnMessageParser.h>
#include "MqttSnClientReceivedGwInfo.h"
#include "MqttSnClientReceivedAdvertise.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MqttSnClientAwaitContext_ {
    MQTT_SN_RETURN_CODE return_code;
    uint16_t            topic_id;
    int8_t              granted_qos;
    //uint16_t            msg_id;

    int32_t (*adv_cb)(MqttSnClientReceivedAdvertise *rec_advertise, void *context);
    int32_t (*gwinfo_cb)(MqttSnClientReceivedGwInfo *rec_gw_info, void *context);

    MqttSnClientReceivedGwInfo *   dst_gwinfo;
    MqttSnClientReceivedAdvertise *dst_advertise;
    void *                         context;
} MqttSnClientAwaitContext;

typedef enum MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ {
    MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR = -1, //TODO check this
    MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS = 0,
    MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_AWAIT,
    MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_TIMEOUT
} MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS;

typedef struct MqttSnClientAwaitMessage_ {
    MQTT_SN_MESSAGE_TYPE                msg_type;
    uint16_t                            msg_id;
    MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS status;
    MqttSnClientAwaitContext            await_context;
} MqttSnClientAwaitMessage;

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNCLIENTAWAITMESSAGESTRUT_H
