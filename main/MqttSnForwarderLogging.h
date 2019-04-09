//
// Created by SomeDude on 08.04.2019.
//

#ifndef CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGING_H
#define CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGING_H

#include <global_defines.h>
#include "forwarder_config.h"
enum log_err_t {
  LOG_ERR_SUCCESS = 0,
  MOSQ_ERR_NOMEM = 1,
  MOSQ_ERR_INVAL = 2,
  MOSQ_ERR_NOT_SUPPORTED = 3
};

int log_flush();
int log_str(const char *str);

int log_int8(int8_t n);
int log_int16(int16_t n);
int log_int32(int32_t n);
int log_int64(int64_t n);

int log_uint8(uint8_t n);
int log_uint16(uint16_t n);
int log_uint32(uint32_t n);
int log_uint64(uint64_t n);
int log_uint64(uint64_t n);

int log_uintmax(uintmax_t n);
int log_key_int8_value(const char *key, int8_t v);

int get_timestamp(uint64_t *t);
int log_current_time();

int log_device_address(const device_address *address);

int log_gateway_id(uint8_t gwId);

int log_gateway_add(uint8_t gwAdd);

int log_message_id(uint32_t msgId);

int log_clean_flag(uint8_t clean);

int log_duration(uint32_t duration);

int log_protocol_id(uint8_t protocolId);

int log_will_flag(uint8_t will);

int log_return_code(uint8_t returnCode);

int log_topic_id(uint32_t topicId);

int log_topic_name(const char *topicName);

int log_topic_id_type(uint8_t topicIdType);

int log_retain_flag(uint8_t retainFlag);

int log_data_byte_counter(uint32_t dataByteCount);

// 1554750672: cmqttsnforwarder version 1.4.15 (build date 1553632477) starting
// { "t":1554750672, "name":"cmqttsnforwarder", "version":"1.4.15", "builddate":1553632477, "action":"starting"}
int log_forwarder_started(int level, const char *VERSION, int major, int minor, int revision);

// 1554750672: cmqttsnforwarder version 1.4.15 terminated
int log_forwarder_terminated(int level, const char *VERSION, uint32_t major, uint32_t minor, uintmax_t revision);

// 1554750672: Using default config.
// 1554750672: Opening ipv4 listen socket on port 8888 [34, 11].
// 1554750672: Opening ipv6 listen socket on port 8888 [34, 11].
// TODO print out full config
// LOG_LEVEL_DEFAULT
//int log_config(int level, forwarder_config *fcfg);



// 1554750679: client message from 127.0.0.1.24.11 to 127.0.0.1.24.11 ( length255, bytes(5, 3, 2 , 1)).
// LOG_LEVEL_DEBUG
int log_client_message(int level, const device_address *address, const uint8_t *data, uint16_t data_len);

// 1554750679: gateway message from 127.0.0.1.24.11 to 127.0.0.1.24.11 ( length255, bytes(5, 3, 2 , 1)).
// LOG_LEVEL_DEBUG
int log_gateway_message(int level, const device_address *address, const uint8_t *data, uint16_t data_len);

// logs any valid mqtt-sn message from the client network
// LOG_LEVEL_DEFAULT
int log_client_mqtt_sn_message(int level, const device_address *address, const uint8_t *data, uint16_t data_len);

// logs any valid mqtt-sn message from the gateway network
// LOG_LEVEL_DEFAULT
int log_gateway_mqtt_sn_message(int level, const device_address *address, const uint8_t *data, uint16_t data_len);

// 1554750679: ADVERTISE to 255.255.255.255.34.11 (gw1, d600).
// device_address
// (gw) gatewayId [0-255], (d)uration [0-65535]
// LOG_LEVEL_VERBOSE
int log_gateway_advertise(int level, device_address address);

// 1554750679: SEARCHGW to 255.255.255.255.34.11 (rad2).
// device_address
// (rad)ius [0-255]
// LOG_LEVEL_VERBOSE
int log_client_searchgw(int level, device_address address);

// 1554750679: GWINFO to 127.0.0.1.34.11 (gw1, gwAdd).
// device_address
// (gw) gatewayId [0-255], (gwAdd) gatewayAddress {device_address [1]}
// LOG_LEVEL_VERBOSE
int log_client_gwinfo_message(int level, device_address address);

// 1554750679: GWINFO from 255.255.255.255.34.11 (gw1).
// device_address
// (gw) gatewayId [0-255]
// LOG_LEVEL_VERBOSE
int log_gateway_gwinfo_message(int level, device_address address);

// 1554750679: CONNECT from 127.0.0.1 as SomeDude210607853 (m1, c1, d60, p1, w0).
// device_address, clientId {char [0 - 24]}
// (m)essageId [0 - 65535], (c)lean [1|0], (d)uration [0 - 65535], (p)rotocolId [1], (w)ill [1|0]
// LOG_LEVEL_DEFAULT
int log_client_connect_message(int level, device_address address);

// 1554750679: CONNECT to 127.0.0.1 (rc0).
// device_address
// (r)eturnCode [0 - 255]
// LOG_LEVEL_DEFAULT
int log_gateway_connak_message(int level, device_address address);

// 1554750679: DISCONNECT from 127.0.0.1.
// 1554750679: DISCONNECT from 127.0.0.1 (d600).
// device_address
// (d)uration [0 - 65535]
// LOG_LEVEL_DEFAULT
int log_client_disconnect_message(int level, device_address address);

// 1554750679: DISCONNECT to 127.0.0.1.
// device_address
// LOG_LEVEL_DEFAULT
int log_gateway_disconnect_message(int level, device_address address);

// 1554750679: REGISTER from 127.0.0.1 (m1, t0, 'some/topic/name').
// device_address
// LOG_LEVEL_VERBOSE
// (m)essageId [0 - 65535], (t)opicId [0-65535], topicName {char [0 - 65535-2-2-1-1]
int log_client_register_message(int level, device_address address);

// 1554750679: REGACK to 127.0.0.1 (m1, t1, r0).
// device_address
// (m)essageId [0 - 65535], (t)opicId [0-65535], (r)eturnCode [0 - 255]
// LOG_LEVEL_VERBOSE
int log_gateway_regack_message(int level, device_address address);

// 1554750679: SUBSCRIBE from 127.0.0.1 (m2, d0, q0, t2, t1).
// 1554750679: SUBSCRIBE from 127.0.0.1 (m2, d0, q0, t0, 'some/topic).
// device_address
// (m)essageId [0 - 65535], (d)up [1|0], (q)os [-1|0|1|2], (t)opicIdType [0|1|2], [ (t)opicId [0-65535] | topicName {char [0 - 65535-2-2-1-1] ]
// LOG_LEVEL_VERBOSE
int log_client_subscribe_message(int level, device_address address);

// 1554750679: SUBACK to 127.0.0.1 (m2, q0, t1, r0).
// device_address
// (m)essageId [0 - 65535], (q)os [-1|0|1|2], (t)opicId [0-65535], (r)eturnCode [0 - 255]
// LOG_LEVEL_VERBOSE
int log_gateway_suback_message(int level, device_address address);

// 1554750679: PUBLISH from 127.0.0.1 (m4, d0, q-1, r0, t0, t2, ... (20 bytes)).
// device_address
// (m)essageId [0 - 65535], (d)up [1|0], (q)os [-1|0|1|2], (r)etain [0|1] ,(t)opicIdType [0|1|2], (t)opicId [0-65535], dataByteCount {bytes [0 - 65535-2-2-1-1]}
// LOG_LEVEL_VERBOSE
int log_client_publish_message(int level, device_address address);

// 1554750679: PUBLISH to 127.0.0.1 (m4, d0, q-1, r0, t0, t2, ... (20 bytes)).
// device_address
// (m)essageId [0 - 65535], (d)up [1|0], (q)os [-1|0|1|2], (r)etain [0|1] ,(t)opicIdType [0|1|2], (t)opicId [0-65535], dataByteCount {bytes [0 - 65535-2-2-1-1]}
// LOG_LEVEL_VERBOSE
int log_gateway_publish_message(int level, device_address address);

// 1554750679: PUBACK from 127.0.0.1 (m4, t2, r0).
// device_address
// (m)essageId [0 - 65535], (t)opicId [0-65535], (r)eturnCode [0 - 255]
// LOG_LEVEL_VERBOSE
int log_client_puback_message(int level, device_address address);

// 1554750679: UNSUBSCRIBE from 127.0.0.1 (m2, t2, t1).
// 1554750679: UNSUBSCRIBE from 127.0.0.1 (m2, t0, 'some/topic).
// device_address
// (m)essageId [0 - 65535], (t)opicIdType [0|1|2], [ (t)opicId [0-65535] | topicName {char [0 - 65535-2-2-1-1] ]
// LOG_LEVEL_VERBOSE
int log_client_unsubscribe_message(int level, device_address address);

// 1554750679: UNSUBACK to 127.0.0.1 (m2).
// device_address
// (m)essageId [0 - 65535]
// LOG_LEVEL_VERBOSE
int log_gateway_unsuback_message(int level, device_address address);

#endif //CMQTTSNFORWARDER_MQTTSNFORWARDERLOGGING_H
