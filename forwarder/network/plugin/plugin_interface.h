//
// Created by SomeDude on 07.04.2019.
//

#ifndef CMQTTSNFORWARDER_PLUGIN_INTERFACE_H
#define CMQTTSNFORWARDER_PLUGIN_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../MqttSnGatewayNetworkInterface.h"

typedef struct plugin_device_address_ {
  const uint8_t length;
  const uint8_t *bytes;
} plugin_device_address;

typedef struct plugin_config_ {
  const char *plugin_path;
  const char *protocol;
  uint16_t forwarder_maximum_message_length;
  uint8_t forwarder_device_address_length;
  const plugin_device_address *mqtt_sn_gateway_network_address;
  const plugin_device_address *forwarder_gateway_network_address;
} plugin_config;

typedef struct plugin_message_ {
  plugin_device_address address;
  uint8_t *data;
  uint16_t data_length;
} plugin_message;

// TODO rewrite doku
/**
 * initialize the network - use the protocol to check if the protocol matches you
 * match the protocol
 * initialize resources
 * the protocol is set by the -gP or -gL command line argument and passed to the plugin_config.
 * initialize the plugin_context here
 * @return -1 on error or when the protocol does not match else 0
 */
int plugin_network_init(const plugin_config *cfg, void *plugin_context);

/**
 *
 * @return the short name of the protocol supported by the plugin e.g. udp or tcp. It is used to match the command line protocol argument with the plugin
 */
const char *plugin_get_short_network_protocol_name();

/**
 *
 * @return the maximum count of bytes the plugin can send and receive.
 */
uint16_t plugin_get_maximum_message_length();

/**
 * free resources
 * there will be no later call
 * free the plugin_context information here
 */
void plugin_network_disconnect(const plugin_config *cfg, void *plugin_context);

/**
 * Connect to the network - use the mqtt_gateway_network_address and the forwarder_network_address inside the plugin_config.
 * The mqtt_gateway_network_address is set by the -h and the -p or the -L command line arguments and passed to the plugin_config.
 * The forwarder_network_address is set by the -gA and the -gp or the -gL command line arguments and passed to the plugin_config.
 * @param cfg
 * @param plugin_context
 * @return -1 on error
 */
int plugin_network_connect(const plugin_config *cfg, void *plugin_context);

/**
 * receives data from the network to the target
 * the maximum bytes you can put into the rec_message buffer is equal to plugin_get_maximum_message_length();
 * the timeout is given in milliseconds. A timeout of 0 or smaller can be ignored.
 * @param n
 * @param receiveBuffer
 * @param timeout_ms
 * @param context
 * @return -1 on error, 0 if a message was received, 1 if no message was received
 */
int plugin_network_receive(plugin_message *rec_message,
                           int timeout_ms,
                           const plugin_config *cfg,
                           void *plugin_context);

/**
 * Is used for two scenarios: Sends data over the gateway network to the mqt-sn gateway.
 * Send a plugin_message to the destination. The destination address is take from the plugin_message address.
 * The data as byte array and the data_length is taken from the plugin_message.
 * the timeout is given in milliseconds. A timeout of 0 or smaller can be ignored.
 * note: it may happen that message are not send to the gateway, e.g. a search-gw message.
 * @param send_message to be send
 * @param cfg plugin configuration
 * @param plugin_context plugin context for plugin specific information to be passed between the plugin functions
 * @return -1 on error, else the number of send bytes, if the number of send bytes does not match plugin_message.data_length the packet will be resend later.
 */
int plugin_network_send(const plugin_message *send_message,
                        int timeout_ms,
                        const plugin_config *cfg,
                        void *plugin_context);

#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_PLUGIN_INTERFACE_H
