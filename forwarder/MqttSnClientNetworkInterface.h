//
// Created by bele on 25.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACE_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "MqttSnFixedSizeRingBuffer.h"
#include "MqttSnForwarderLogging.h"
/**
 * MqttSnClientNetworkInterface provides an interface for to glue different kind of network implementations to the
 * MqttSnForwarder.
 */
typedef struct MqttSnClientNetworkInterface {

  int status;

  int (*client_network_receive)(struct MqttSnClientNetworkInterface *,
                                MqttSnFixedSizeRingBuffer *,
                                int,
                                void *context);

  int (*client_network_send)(struct MqttSnClientNetworkInterface *,
                             MqttSnFixedSizeRingBuffer *,
                             int,
                             void *context);

  int (*client_network_init)(struct MqttSnClientNetworkInterface *, void *context);

  int (*client_network_connect)(struct MqttSnClientNetworkInterface *, void *context);

  void (*client_network_disconnect)(struct MqttSnClientNetworkInterface *, void *context);

  device_address *client_network_address;

  device_address *mqtt_sn_gateway_address;

#ifdef WITH_LOGGING
  MqttSnLogger *logger;
#endif

} MqttSnClientNetworkInterface;

/**
 * Initializes the MqttSnClientNetworkInterface with the given client_network_address, the client network context
 * and the client_network_init method. Returns -1 in case of an error, 0 otherwise.
 *
 * @param client_network_address to be used inside the network. Is saved in the MqttSnClientNetworkInterface.
 * The client_network_address can be used by the network as address configuration or be ignored.
 * The semantic of the client_network_address's bytes is up to the network context implementor.
 * @param context passed to the MqttSnClientNetworkInterface and the client_network_init function pointer.
 * @param client_network_init function pointer to the network's initialization function.
 * The client_network_init function is called within this function.
 * @return -1 in case of an error, 0 otherwise.
 */
int ClientNetworkInit(MqttSnClientNetworkInterface *n,
                      device_address *mqtt_sn_gateway_device_address,
                      device_address *client_network_address,
                      void *context,
                      int (*client_network_init)(MqttSnClientNetworkInterface *, void *));

/**
 * De-initialize local data structures, close open connections and free resources here.
 * Is called after either client or gateway network returned -1 from any other method.
 *
 * @param context is the given client network context during ClientNetworkInit().
 */
void ClientNetworkDisconnect(MqttSnClientNetworkInterface *, void *context);

/**
 * Connect to the network. E.g. in TCP open a socket for incoming connections, in UDP you can open a socket for incoming
 * UDP datagrams. Depend on your network you can perform joining network procedures here.
 * Returns -1 in case of an error, 0 otherwise.
 *
 * @param context is the given client network context during ClientNetworkInit().
 * @return -1 in case of an error, 0 otherwise.
 */
int ClientNetworkConnect(MqttSnClientNetworkInterface *, void *context);

int ClientNetworkSend(MqttSnClientNetworkInterface *n,
                      MqttSnFixedSizeRingBuffer *sendBuffer,
                      int timeout_ms,
                      void *context);

int ClientNetworkReceive(MqttSnClientNetworkInterface *n,
                      MqttSnFixedSizeRingBuffer *receiveBuffer,
                      int timeout_ms,
                      void *context);
#ifdef __cplusplus
}
#endif

#endif //CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACE_H
