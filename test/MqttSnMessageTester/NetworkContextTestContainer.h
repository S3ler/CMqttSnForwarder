//
// Created by SomeDude on 18.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNMESSAGETESTER_NETWORKCONTEXTTESTCONTAINER_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNMESSAGETESTER_NETWORKCONTEXTTESTCONTAINER_H_

#include <stdint.h>
#include <vector>
#include <memory>
#include <config/network/client/client_network_config.h>
#include <network/linux/client/ip/tcp/MqttSnClientTcpNetwork.h>
#include <network/linux/client/ip/udp/MqttSnClientUdpNetwork.h>
#include <forwarder/config/forwarder_config.h>
#include <forwarder/MqttSnForwarder.h>
#include <network/linux/gateway/plugin/gateway_network_plugin_interface.h>
#include <network/linux/gateway/plugin/MqttSnGatewayPluginNetwork.h>
#include <network/linux/client/plugin/MqttSnClientPluginNetwork.h>
#include <network/linux/gateway/ip/tcp/MqttSnGatewayTcpNetwork.h>

using std::shared_ptr;

class NetworkContextTestContainer {
 private:
  MqttSnForwarder forwarder;
  void *gatewayNetworkContext = NULL;
  void *clientNetworkContext = NULL;

 private:
  device_address mqttSnGatewayNetworkAddress;

  device_address clientNetworkAddress;
  device_address clientNetworkBroadcastAddress;

  device_address gatewayNetworkAddress;
  device_address gatewayNetworkBroadcastAddress;
#ifdef WITH_LINUX_PLUGIN_NETWORK_TODO
  shared_ptr<gateway_plugin_device_address> pluginMqttSnGatewayNetworkAddress = nullptr;
  shared_ptr<gateway_plugin_device_address> pluginForwarderGatewayNetworkAddress = nullptr;
  shared_ptr<gateway_plugin_device_address> pluginForwarderGatewayNetworkBroadcastAddress = nullptr;
  shared_ptr<gateway_plugin_config> plugin_cfg = nullptr;
  shared_ptr<MqttSnGatewayPluginContext> gatewayPluginContext = nullptr;
  shared_ptr<MqttSnClientPluginContext> clientPluginContext = nullptr;
#endif
#ifdef WITH_LINUX_TCP_CLIENT_NETWORK_TODO
  shared_ptr<MqttSnGatewayTcpNetwork> tcpGatewayNetworkContext = nullptr;
  shared_ptr<MqttSnClientTcpNetwork> tcpClientNetworkContext = nullptr;
#endif
#ifdef WITH_LINUX_UDP_CLIENT_NETWORK
  shared_ptr<MqttSnGatewayUdpNetwork> udpGatewayNetworkContext;
  shared_ptr<MqttSnClientUdpNetwork> udpClientNetworkContext;
#endif

 public:
  void *InitializeClientNetwork(MqttSnClientNetworkInterface *clientNetworkInterface,
                                const client_network_config *cncfg,
                                const mqtt_sn_gateway_config *optional_msgcfg);
  void *InitializeGatewayNetwork(MqttSnGatewayNetworkInterface *gatewayNetworkInterface,
                                 const gateway_network_config *gncfg,
                                 const mqtt_sn_gateway_config *msgcfg,
                                 const client_find_mqtt_sn_gateway_config *cfmsgcfg);

 private:
#ifdef WITH_LINUX_UDP_CLIENT_NETWORK
  int32_t init_client_udp(const client_network_config *cncfg,
                          const mqtt_sn_gateway_config *optional_msgcfg,
                          MqttSnClientNetworkInterface *clientNetworkInterface,
                          const MqttSnLogger *logger);
  int32_t init_gateway_udp(const gateway_network_config *gncfg,
                           const mqtt_sn_gateway_config *msgcfg,
                           const client_find_mqtt_sn_gateway_config *cfmsgcfg,
                           MqttSnGatewayNetworkInterface *gatewayNetworkInterface,
                           const MqttSnLogger *logger);
#endif

};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNMESSAGETESTER_NETWORKCONTEXTTESTCONTAINER_H_
