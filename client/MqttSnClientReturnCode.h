//
// Created by SomeDude on 25.08.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTRETURNCODE_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTRETURNCODE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum MQTT_SN_CLIENT_RETURN_CODE_ {
    MQTT_SN_CLIENT_RETURN_ERROR   = -1,
    MQTT_SN_CLIENT_RETURN_SUCCESS = 0,
    MQTT_SN_CLIENT_RETURN_CONGESTION,
    MQTT_SN_CLIENT_RETURN_NOT_SUPPORTED,
    MQTT_SN_CLIENT_RETURN_TIMEOUT,
    MQTT_SN_CLIENT_RETURN_NETWORK_ERROR,
    MQTT_SN_CLIENT_RETURN_NOT_CONNECTED,
    MQTT_SN_CLIENT_RETURN_CONNECTION_LOST,
} MQTT_SN_CLIENT_RETURN_CODE;

#ifdef __cplusplus
}
#endif
#endif  //CMQTTSNFORWARDER_MQTTSNCLIENTRETURNCODE_H