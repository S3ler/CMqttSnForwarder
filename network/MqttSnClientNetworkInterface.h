//
// Created by bele on 25.01.19.
//

#ifndef CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACE_H
#define CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACE_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <ringbuffer/MqttSnFixedSizeRingBuffer.h>
#ifdef WITH_LOGGING
#include <logging/MqttSnLoggingInterface.h>
#endif

typedef enum MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_ {
  MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_DEINITIALIZED = 0,
  MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_INITIALIZED = 1,
  MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_DISCONNECTED = 2,
  MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS_CONNECTED = 3
} MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS;

/**
 * MqttSnClientNetworkInterface provides an interface for to glue different kind of network implementations to the
 * MqttSnForwarder.
 */
typedef struct MqttSnClientNetworkInterface_ {
  MQTT_SN_CLIENT_NETWORK_INTERFACE_STATUS status;
  uint16_t max_data_length;

  device_address *client_network_address;
  device_address *mqtt_sn_gateway_address;
  device_address *client_network_broadcast_address;

  int32_t (*initialize)(struct MqttSnClientNetworkInterface_ *, void *context);
  int32_t (*deinitialize)(struct MqttSnClientNetworkInterface_ *, void *context);

  int32_t (*connect)(struct MqttSnClientNetworkInterface_ *, void *context);
  int32_t (*disconnect)(struct MqttSnClientNetworkInterface_ *, void *context);

  int32_t (*send)(struct MqttSnClientNetworkInterface_ *,
                  const device_address *,
                  const device_address *, const uint8_t *, uint16_t, uint16_t *,
                  uint8_t,
                  int32_t,
                  void *context);
  int32_t (*receive)(struct MqttSnClientNetworkInterface_ *,
                     device_address *,
                     device_address *, uint8_t *, uint16_t *, uint16_t,
                     uint8_t *,
                     int32_t,
                     void *context);

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
int ClientNetworkInitialize(MqttSnClientNetworkInterface *n,
                            uint16_t max_data_length,
                            device_address *mqtt_sn_gateway_address,
                            device_address *client_network_address,
                            device_address *client_network_broadcast_address,
                            void *context,
                            int (*client_network_init)(MqttSnClientNetworkInterface *, void *));

int32_t ClientNetworkDeinitialize(MqttSnClientNetworkInterface *n, void *context);

/**
 * Connect to the network. E.g. in TCP open a socket for incoming connections, in UDP you can open a socket for incoming
 * UDP datagrams. Depend on your network you can perform joining network procedures here.
 * Returns -1 in case of an error, 0 otherwise.
 *
 * @param context is the given client network context during ClientNetworkInit().
 * @return -1 in case of an error, 0 otherwise.
 */
int32_t ClientNetworkConnect(MqttSnClientNetworkInterface *, void *context);

/**
 * De-initialize local data structures, close open connections and free resources here.
 * Is called after either client or gateway network returned -1 from any other method.
 *
 * @param context is the given client network context during ClientNetworkInit().
 */
int32_t ClientNetworkDisconnect(MqttSnClientNetworkInterface *, void *context);

int32_t ClientNetworkSend(MqttSnClientNetworkInterface *n,
                          MqttSnFixedSizeRingBuffer *sendBuffer,
                          int timeout_ms,
                          void *context);

int32_t ClientNetworkReceive(MqttSnClientNetworkInterface *n,
                             MqttSnFixedSizeRingBuffer *receiveBuffer,
                             int timeout_ms,
                             void *context);

#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_MQTTSNCLIENTNETWORKINTERFACE_H
