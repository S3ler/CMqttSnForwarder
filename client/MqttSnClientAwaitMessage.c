//
// Created by SomeDude on 27.06.2019.
//

#include "MqttSnClientMethod.h"

MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS get_await_status_timeout_ms_easy(MqttSnClient *client, int32_t await_fd, uint64_t timeout_ms) {
    MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc;
    uint64_t                            start_ts_s = 0;

    if (get_timestamp_s(&start_ts_s) < 0) {
        return MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR;
    }
    while ((await_rc = get_await_status(client, await_fd)) == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_AWAIT) {
        uint64_t current_time_s = 0;
        if (get_timestamp_s(&current_time_s) < 0) {
            return MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR;
        }
        if (MqttSnClientLoop(client) < 0) {
            await_rc = MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR;
            break;
        }
        if (timeout_ms > 0) {
            if (client->status == MQTT_SN_CLIENT_CONNECTION_STATUS_AWAKE) {
                start_ts_s = client->awake_ping_request_send_time_s;
            }
            uint64_t elapsed_time_s = current_time_s - start_ts_s;
            if (elapsed_time_s > (uint64_t)timeout_ms / (uint64_t)1000) {
                break;
            }
        } else if (timeout_ms == 0) {
            break;
        }
    }
    if (await_rc != MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) {
        if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_AWAIT) {
            return MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_TIMEOUT;
        }
        return await_rc;
    }
    return await_rc;
}
MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS get_await_status_timeout_ms(MqttSnClient *client, int32_t await_fd, uint64_t start_ts_ms,
                                                                uint64_t timeout_ms) {
    MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS await_rc;
    while ((await_rc = get_await_status(client, await_fd)) == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_AWAIT) {
        uint64_t current_time = 0;
        if (get_timestamp_s(&current_time) < 0) {
            return MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR;
        }
        if (MqttSnClientLoop(client) < 0) {
            await_rc = MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR;
            break;
        }
        if (timeout_ms > 0) {
            uint64_t elapsed_time = current_time - start_ts_ms;
            if (elapsed_time > (uint64_t)timeout_ms / 1000) {
                break;
            }
        } else if (timeout_ms == 0) {
            break;
        }
    }
    if (await_rc != MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS) {
        if (await_rc == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_AWAIT) {
            return MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_TIMEOUT;
        }
        return await_rc;
    }
    return await_rc;
}

MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS get_await_status(MqttSnClient *client, int32_t await_fd) {
    if (await_fd < 0 || await_fd > MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT - 1) {  // maybe use assert instead
        return MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_ERROR;
    }
    return client->await_msg[await_fd].status;
}
MqttSnClientAwaitContext *get_await_context(MqttSnClient *client, int32_t await_fd) {
    if (await_fd < 0 || await_fd > MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT - 1) {  // maybe use assert instead
        return NULL;
    }
    return &client->await_msg[await_fd].await_context;
}
int32_t is_await_msg(MqttSnClient *client, MQTT_SN_MESSAGE_TYPE msg_type, uint16_t msg_id) {
    for (uint16_t i = 0; i < MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT; i++) {
        MqttSnClientAwaitMessage await_message = client->await_msg[i];
        if (await_message.msg_type == ANY_MESSAGE_TYPE) {
            continue;
        }
        if (client->await_msg[i].status == MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_AWAIT && client->await_msg[i].msg_type == msg_type &&
            client->await_msg[i].msg_id == msg_id) {
            return i;
        }
    }
    return -1;
}
int32_t set_await_msg_status(MqttSnClient *client, int32_t await_fd, MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS status) {
    if (await_fd < 0 || await_fd > MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT - 1) {  // maybe use assert instead
        return -1;
    }
    client->await_msg[await_fd].status = status;
    return 0;
}
int32_t set_await_msg_return_code(MqttSnClient *client, int32_t await_fd, MQTT_SN_RETURN_CODE return_code) {
    if (await_fd < 0 || await_fd > MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT - 1) {  // maybe use assert instead
        return -1;
    }
    client->await_msg[await_fd].await_context.return_code = return_code;
    return 0;
}
int32_t set_await_msg_granted_qos(MqttSnClient *client, int32_t await_fd, int8_t granted_qos) {
    if (await_fd < 0 || await_fd > MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT - 1) {  // maybe use assert instead
        return -1;
    }
    client->await_msg[await_fd].await_context.granted_qos = granted_qos;
    return 0;
}
int32_t set_await_msg_topic_id(MqttSnClient *client, int32_t await_fd, uint16_t topic_id) {
    if (await_fd < 0 || await_fd > MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT - 1) {  // maybe use assert instead
        return -1;
    }
    client->await_msg[await_fd].await_context.topic_id = topic_id;
    return 0;
}
int32_t free_await_fd(MqttSnClient *client, int32_t await_fd) {
    if (await_fd < 0 || await_fd > MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT - 1) {  // maybe use assert instead
        return -1;
    }
    client->await_msg[await_fd].msg_type = ANY_MESSAGE_TYPE;
    client->await_msg[await_fd].msg_id   = 0;
    client->await_msg[await_fd].status   = MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_SUCCESS;
    memset(&client->await_msg[await_fd].await_context, 0, sizeof(MqttSnClientAwaitContext));
    return 0;
}
int32_t acquire_await_fd(MqttSnClient *client, MQTT_SN_MESSAGE_TYPE msg_type, uint16_t msg_id) {
    for (uint16_t i = 0; i < MQTT_SN_CLIENT_MAX_MESSAGES_IN_FLIGHT; i++) {
        if (client->await_msg[i].msg_type == ANY_MESSAGE_TYPE) {
            client->await_msg[i].msg_type = msg_type;
            client->await_msg[i].msg_id   = msg_id;
            client->await_msg[i].status   = MQTT_SN_CLIENT_AWAIT_MESSAGE_STATUS_AWAIT;
            return i;
        }
    }
    return -1;
}
