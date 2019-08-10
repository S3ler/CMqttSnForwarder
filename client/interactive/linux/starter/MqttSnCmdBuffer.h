//
// Created by SomeDude on 02.08.2019.
//

#ifndef CMQTTSNFORWARDER_CLIENT_INTERACTIVE_LINUX_STARTER_MQTTSNCMDBUFFER_H_
#define CMQTTSNFORWARDER_CLIENT_INTERACTIVE_LINUX_STARTER_MQTTSNCMDBUFFER_H_

#include <stdint.h>
#include <client/interactive/config/interactive_client_config.h>
#include <network/linux/gateway/ip/udp/MqttSnGatewayUdpNetwork.h>
#include <client/MqttSnClient.h>
#include <logging/linux/file/FileLogging.h>
#include <logging/linux/filestdout/FileStdoutLogging.h>
#include "mqtt_sn_client_interactive_action_type.h"
#include "mqtt_sn_client_interactive_action_result_type.h"
#include "mqtt_sn_client_interactive_cmd_result.h"

#ifdef __cplusplus
extern "C" {
#endif
// the maximum message length is 65536 => encoded as 255, 65536*4. Plus some additional space for commands
#ifndef MQTT_SN_COMMAND_BUFFER_MAXIMUM_LENGTH
#define MQTT_SN_COMMAND_BUFFER_MAXIMUM_LENGTH 1024 // (65536 * 4 + 1014)
#endif
#ifndef MQTT_SN_COMMAND_BUFFER_MAXIMUM_ARGV_LENGTH
#define MQTT_SN_COMMAND_BUFFER_MAXIMUM_ARGV_LENGTH 1024
#endif

typedef struct interactive_client_udp_buffer_ {
  device_address mqttSnGatewayNetworkAddress;
  device_address forwarderGatewayNetworkAddress;
  device_address forwarderGatewayNetworkBroadcastAddress;
  MqttSnGatewayUdpNetwork udpGatewayNetworkContext;
} interactive_client_udp_buffer;

void interactive_client_udp_buffer_init(interactive_client_udp_buffer *cfg);

int32_t interactive_client_udp_buffer_start(const MqttSnLogger *logger,
                                            const mqtt_sn_gateway_config *msngcfg,
                                            const gateway_network_config *gncfg,
                                            MqttSnGatewayNetworkInterface *gatewayNetwork,
                                            interactive_client_udp_buffer *gatewayNetworkContext,
                                            device_address *mqtt_sn_gateway_address);
typedef struct mqtt_sn_interactive_client_ {
  uint64_t action_number;
  MqttSnClient client;
  device_address mqtt_sn_gateway_address;
  interactive_client_udp_buffer udp_network_buffer;

  FileLoggingContext file_log_context;
  FileStdoutLoggingContext fileconsole_log_context;
} mqtt_sn_interactive_client;
typedef enum MQTT_SN_CLIENT_INTERACTIVE_STATUS_ {
  MQTT_SN_CLIENT_INTERACTIVE_STATUS_GATEWAY_NETWORK_DISCONNECTED,
  MQTT_SN_CLIENT_INTERACTIVE_STATUS_GATEWAY_NETWORK_CONNECTED,
  MQTT_SN_CLIENT_INTERACTIVE_STATUS_MQTT_SN_GATEWAY_DISCONNECTED,
  MQTT_SN_CLIENT_INTERACTIVE_STATUS_MQTT_SN_GATEWAY_CONNECTED
} MQTT_SN_CLIENT_INTERACTIVE_STATUS;

void mqtt_sn_interactive_client_init(mqtt_sn_interactive_client *interactive_client);


#define MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_NO_COMMAND -1
#define MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_INVALID_STATE 0
#define MQTT_SN_CLIENT_INTERACTIVE_COMMAND_RETURN_VALUE_VALID_STATE 1

typedef struct mqtt_sn_interactive_client_cmd_buffer_ {
  char argv_0[1];
  char buffer[MQTT_SN_COMMAND_BUFFER_MAXIMUM_LENGTH];
  int32_t argc;
  char *argv[MQTT_SN_COMMAND_BUFFER_MAXIMUM_ARGV_LENGTH];
  MQTT_SN_CLIENT_INTERACTIVE_STATUS status;
} mqtt_sn_interactive_client_cmd_buffer;

void mqtt_sn_cmd_buffer_init(mqtt_sn_interactive_client_cmd_buffer *mqtt_sn_cmd_buffer);
void mqtt_sn_cmd_buffer_reset(mqtt_sn_interactive_client_cmd_buffer *mqtt_sn_cmd_buffer);
void mqtt_sn_cmd_buffer_print_current_status(const MqttSnLogger *logger,
                                             const mqtt_sn_interactive_client_cmd_buffer *mqtt_sn_cmd_buffer);

int32_t mqtt_sn_cmd_buffer_parse_string(mqtt_sn_interactive_client_cmd_buffer *mqtt_sn_cmd_buffer);
uint8_t mqtt_sn_cmd_buffer_is_exit_command(const char *str);
uint8_t mqtt_sn_buffer_is_help(const char *str);

MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE mqtt_sn_cmd_buffer_parse_command(mqtt_sn_interactive_client_cmd_buffer *c);
int32_t mqtt_sn_cmd_buffer_check_status(mqtt_sn_interactive_client_cmd_buffer *c,
                                        MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk);
int32_t mqtt_sn_cmd_buffer_change_status(mqtt_sn_interactive_client_cmd_buffer *cmd_buffer,
                                         MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                         MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE result);
mqtt_sn_client_interactive_cmd_result mqtt_sn_cmd_buffer_execute_command(mqtt_sn_interactive_client *client,
                                                                         MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                         const interactive_client_config *cfg,
                                                                         MqttSnLogger *logger);

void mqtt_sn_cmd_buffer_print_usage_short(const MqttSnLogger *logger);
void mqtt_sn_cmd_buffer_print_usage_long(const MqttSnLogger *logger);

int32_t mqtt_sn_client_interactive_start_logger(mqtt_sn_interactive_client *client,
                                                const mqtt_sn_logger_config *cfg,
                                                MqttSnLogger *logger);

MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveReinitLogger(mqtt_sn_interactive_client *client,
                                                                                  MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                                  const interactive_client_config *cfg,
                                                                                  MqttSnLogger *logger);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveInit(mqtt_sn_interactive_client *client,
                                                                          MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                          const interactive_client_config *cfg,
                                                                          const MqttSnLogger *logger);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveDeinit(mqtt_sn_interactive_client *client,
                                                                            MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                            const interactive_client_config *cfg,
                                                                            const MqttSnLogger *logger);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveSearchGwAndConnect(mqtt_sn_interactive_client *client,
                                                                                        MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                                        const interactive_client_config *cfg,
                                                                                        const MqttSnLogger *logger);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveDirectConnect(mqtt_sn_interactive_client *client,
                                                                                   MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                                   const interactive_client_config *cfg,
                                                                                   const MqttSnLogger *logger);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveAdvertiseAndDirectConnect(
    mqtt_sn_interactive_client *client,
    MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
    const interactive_client_config *cfg,
    const MqttSnLogger *logger);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveDisconnect(mqtt_sn_interactive_client *client,
                                                                                MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                                const interactive_client_config *cfg,
                                                                                const MqttSnLogger *logger);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveSubscribe(mqtt_sn_interactive_client *client,
                                                                               MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                               const interactive_client_config *cfg,
                                                                               const MqttSnLogger *logger);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveUnsubscribe(mqtt_sn_interactive_client *client,
                                                                                 MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                                 const interactive_client_config *cfg,
                                                                                 const MqttSnLogger *logger);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveRegister(mqtt_sn_interactive_client *client,
                                                                              MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                              const interactive_client_config *cfg,
                                                                              const MqttSnLogger *logger);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractivePublish(mqtt_sn_interactive_client *client,
                                                                             MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                             const interactive_client_config *cfg,
                                                                             const MqttSnLogger *logger);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveSleep(mqtt_sn_interactive_client *client,
                                                                           MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                           const interactive_client_config *cfg,
                                                                           const MqttSnLogger *logger);
MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE MqttSnClientInteractiveWakeup(mqtt_sn_interactive_client *client,
                                                                            MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE tk,
                                                                            const interactive_client_config *cfg,
                                                                            const MqttSnLogger *logger);
#ifdef __cplusplus
}
#endif
#endif //CMQTTSNFORWARDER_CLIENT_INTERACTIVE_LINUX_STARTER_MQTTSNCMDBUFFER_H_
