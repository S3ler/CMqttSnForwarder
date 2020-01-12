//
// Created by SomeDude on 08.07.2019.
//

#include "MqttSnGatewayForwarder.h"
#include <string.h>
#include <parser/MqttSnForwarderEncapsulationMessage.h>
#include <parser/logging/MqttSnMessageParserLogging.h>

void MqttSnGatewayForwarderInit(MqttSnGatewayForwarder *mqtt_sn_gateway_forwarder) {
  memset(mqtt_sn_gateway_forwarder, 0, sizeof(MqttSnGatewayForwarder));
  mqtt_sn_gateway_forwarder->forwarder_max_len = MQTT_SN_GATEWAY_MAX_FORWARDER_COUNT;
}

uint16_t gateway_add_forwarder_encapsulation_frames(MqttSnMessageData *msg,
                                                    int32_t gen_bytes,
                                                    MqttSnGatewayForwarder *forwarders,
                                                    const MqttSnLogger *logger) {
  msg->to = forwarders->wireless_node_id;
  if (forwarders->forwarder_len == 0) {
    return gen_bytes;
  }
  uint8_t gen_msg_buf[gen_bytes];
  memcpy(gen_msg_buf, msg->data, gen_bytes);
  int32_t fw_gen_rc = generate_multiple_forwarder_encapsulation_headers_byte(msg->data,
                                                                             sizeof(msg->data),
                                                                             forwarders->forwarder_radiuses,
                                                                             forwarders->forwarder_addresses,
                                                                             forwarders->forwarder_len,
                                                                             &forwarders->wireless_node_id,
                                                                             gen_msg_buf,
                                                                             gen_bytes);
  if (fw_gen_rc < 0) {
#if WITH_LOGGING
    log_msg_gen_error(logger, ENCAPSULATED_MESSAGE, __FILE__, __func__, __LINE__);
#endif
    return -1;
  }
  msg->to = forwarders->forwarder_addresses[0];
  return fw_gen_rc;
}
