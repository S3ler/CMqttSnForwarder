//
// Created by SomeDude on 24.05.2019.
//

#include "MqttSnForwarderParser.h"
#include <string.h>
#include <logging/MqttSnLoggingInterface.h>
#include <parser/logging/MqttSnForwarderLoggingMessages.h>

int32_t remove_mqtt_sn_forwarder_encapsulation_frame(MqttSnMessageData *gatewayMessageData,
                                                     MqttSnMessageData *clientMessageData,
                                                     const device_address *client_network_broadcast_address,
                                                     const MqttSnLogger *logger) {
  uint8_t radius = 0;
  int32_t p_rc = parse_forwarder_encapsulation_byte(&radius,
                                                    &clientMessageData->to,
                                                    clientMessageData->data,
                                                    &clientMessageData->data_length,
                                                    sizeof(clientMessageData->data),
                                                    gatewayMessageData->data,
                                                    gatewayMessageData->data_length);
  if (p_rc < 0) {
#ifdef WITH_DEBUG_LOGGING
    log_gateway_mqtt_sn_message_malformed(logger,
                                          &gatewayMessageData->from,
                                          gatewayMessageData->data,
                                          gatewayMessageData->data_length,
                                          gatewayMessageData->signal_strength);
#endif
    return -1;
  }
  clientMessageData->from = gatewayMessageData->from;
  if (client_network_broadcast_address && radius == MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_CRTL_BROADCAST_RADIUS) {
    clientMessageData->to = *client_network_broadcast_address;
  }
#if defined(WITH_DEBUG_LOGGING)
  //TODO
#endif
  return 0;
}
int32_t add_mqtt_sn_forwarder_encapsulation_frame(MqttSnMessageData *clientMessageData,
                                                  MqttSnMessageData *gatewayMessageData,
                                                  const device_address *mqtt_sn_gateway_network_address,
                                                  const device_address *client_network_broadcast_address,
                                                  const MqttSnLogger *logger) {
  uint8_t broadcast = 0;
  if (client_network_broadcast_address
      && memcmp(&gatewayMessageData->to, client_network_broadcast_address, sizeof(device_address)) == 0) {
    broadcast = MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_CRTL_BROADCAST_RADIUS;
    // TODO mention later in doku: Radius: broadcast radius (only relevant in direction GW to forwarder). We ignore the section: "(only relevant in direction GW to forwarder)" as in the translation from one network to another it is not clear what the broadcast address ist.
    // example different UDP broadcast address or in SN network own defined BC address. So we uses these to indicator for this in GW direction to indicate that the message was broadcasted.
  }
  int rc = generate_forwarder_encapsulation_byte(gatewayMessageData->data,
                                                 sizeof(gatewayMessageData->data),
                                                 broadcast,
                                                 &clientMessageData->from,
                                                 clientMessageData->data,
                                                 clientMessageData->data_length);
  if (rc < 0) {
    return -1;
  }
  gatewayMessageData->data_length = rc;
  gatewayMessageData->to = *mqtt_sn_gateway_network_address;
  gatewayMessageData->from = clientMessageData->from;

  return 0;
}
