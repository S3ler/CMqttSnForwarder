Featurelist:
// TODO logging für restiliche mqtt-sn- message
// TODO error parsing


  // TODO: printf("                        [-c config_file]\n");
  // TODO  printf(" --json : produce json valid log message
  // TODO: printf(" -c : specify the forwarder config file.\n");

plugin for logging
code coverage reports
test für forwarder core fixen
mehr tests schreiben
interface dokumenteiren
restliche logfunctionen für verbose

// TODO: Real defragmentationtest without network (fake-network implementations) for interface conformation + interface conformationtestsiii

[![Build Status](http://arsmb.de/api/badges/S3ler/CMqttSnForwarder/status.svg)](http://arsmb.de/S3ler/CMqttSnForwarder)


# C MQTT-SN Forwarder
What does not work yet:
automatically choice of MQTT-SN Gateway (we need a SearchGW and parsing prodecure)
loading from configurationfile
// TODO tests with multiple message (at the moment there is only one message send)
// find out maximum rating for networks
// der trick ist: obwohl in C implementiert kann man C++ google test framework nutzen, man muss nur zur vergleichbarkeit
// alles es eine Frage der Netzwerkabstraction

### Supported Transmission Protocols
The Mqtt-SN Forwarder forwards between two different networks.
For example we can use LoRa on the client side and Tcp to the Mqtt-SN Gateway.
In the following tables is listet which protocols are implemented for client and gateway side.

### Networks

### Client Networks
The gateway networks is the network side where the MQTT-SN Clients are reachable.

#### Supported Client Networks
The table shows for each client network the following information: the network protocol name, implementation status, test status and if examples are available. 

| Protocol     | Tcp  |
| :----------  | :---:|
| Implemented  |   y  |
| Tested       |   n  |
| Example      |   n  |

#### Client Network Tests
The gateway networks is the network side where the MQTT-SN Clients are reachable.

### Gateway Network
The gateway networks is the network side where the MQTT-SN Gateway is reachable.

#### Supported Gateway Networks
The table shows for each gateway network the following information: the network protocol name, implementation status, test status and if examples are available. 

| Protocol     | Tcp  |
| :----------  | :---:|
| Implemented  |   y  |
| Tested       |   n  |
| Example      |   n  |

#### Client Network Tests
The ClientNetwork API has the following method, these are to be tested:
* ClientNetworkInit to initialize the network data structure or hardware components, e.g. initializing data structure for a TCP sockets.
* ClientNetworkConnect to connect to the network, e.g. start listening to a TCP port.
* ClientNetworkDisconnect to disconnect from the network, e.g. stop listening to the TCP port and close all TCP client to the MQTT-SN Gateway.
* ClientNetworkReceive accepts new incoming client TCP connections and receives data from the MQTT-SN clients and puts them into a message buffer.
* ClientNetworkSend to send data from a message buffer to the MQTT-SN clients.

#### Init Tests
| Name     | Description  |
| :----------  | :---|
| ClientNetworkInit_Success_ReturnsZero                |   Tests the API for returning 0 on success. Tests if it is possible to initialize the network components. |
| ClientNetworkInit_Fails_ReturnsMinusOne              |   Tests the API for returning -1 on failure. |

###### Disconnect Tests (requires Connect Tests to pass)
Disconnect test are unique for each network, hardware and used datastructure.
We propose to implement these in a way which makes it possible to check afterwards if it disconnected.
Because this depends on many factors no general tests can be implemented.

###### Connect Tests (requires Init Tests to pass)
| Name     | Description  |
| :----------  | :---|
| ClientNetworkConnect_Success_ReturnsZero             |   Tests the API for returning 0 on success. Tests if it is possible to connect to the network.  |
| ClientNetworkConnect_Fails_ReturnsMinusOne           |   Tests the API for returning -1 on failure. |

###### Receive Tests (requires Connect Tests to pass)
| Name     | Description  |
| :----------  | :---|
| ClientNetworkReceive_MinimumBytesSendFromOneClient_MinimumBytesMessageReceivedFromOneClient      | Tests if a minimum size message (message length 1 byte, 1 byte data) from one client is received.  |
| ClientNetworkReceive_FiveBytesSendFromOneClient_FiveBytesMessageReceivedFromOneClient         | Tests if a five size message (message length 5 byte, 5 byte data) from one client is received. |
| ClientNetworkReceive_MaximumBytesSendFromOneClient_MaximumBytesMessageReceivedFromOneClient |  Tests if a maximum size message (message length CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte, CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte data) from one client is received. |
| ClientNetworkReceive_MinimumBytesSendFromTwoClients_MinimumBytesMessageReceivedFromTwoClients      | Tests if a minimum size message (message length 1 byte, 1 byte data) from two clients is received.  |
| ClientNetworkReceive_FiveBytesSendFromTwoClients_FiveBytesMessageReceivedFromTwoClients         | Tests if a five size message (message length 5 byte, 5 byte data) from two clients is received. |
| ClientNetworkReceive_MaximumBytesSendFromTwoClients_MaximumBytesMessageReceivedFromTwoClients |  Tests if a maximum size message (message length CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte, CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte data) from two clients is received. |
| ClientNetworkReceive_MinimumBytesSendFromFiveClients_MinimumBytesMessageReceivedFromFiveClients      | Tests if a minimum size message (message length 1 byte, 1 byte data) from five clients is received.  |
| ClientNetworkReceive_FiveBytesSendFromFiveClients_FiveBytesMessageReceivedFromFiveClients         | Tests if a five size message (message length 5 byte, 5 byte data) from five clients is received. |
| ClientNetworkReceive_MaximumBytesSendFromFiveClients_MaximumBytesMessageReceivedFromFiveClients |  Tests if a maximum size message (message length CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte, CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte data) from five clients is received. |
| ClientNetworkReceive_MinimumBytesSendFromMaximumClients_MinimumBytesMessageReceivedFromMaximumClients      | Tests if a minimum size message (message length 1 byte, 1 byte data) from maximum clients is received.  |
| ClientNetworkReceive_FiveBytesSendFromMaximumClients_FiveBytesMessageReceivedFromMaximumClients         | Tests if a five size message (message length 5 byte, 5 byte data) from maximum clients is received. |
| ClientNetworkReceive_MaximumBytesSendFromMaximumClients_MaximumBytesMessageReceivedFromMaximumClients |  Tests if a maximum size message (message length CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte, CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte data) from maximum clients is received. |

###### Send Tests (requires Connect Tests to pass)
| Name     | Description  |
| :----------  | :---|
| ClientNetworkReceive_MinimumBytesSendToOneClient_MinimumBytesMessageReceivedByOneClient      | Tests if a minimum size message (message length 1 byte, 1 byte data) is received by one client.  |
| ClientNetworkReceive_FiveBytesSendToOneClient_FiveBytesMessageReceivedByOneClient         | Tests if a five size message (message length 5 byte, 5 byte data) is received by one client.  |
| ClientNetworkReceive_MaximumBytesSendToOneClient_MaximumBytesMessageReceivedByOneClient |  Tests if a maximum size message (message length CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte, CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte data) is received by one client.  |
| ClientNetworkReceive_MinimumBytesSendToTwoClients_MinimumBytesMessageReceivedByTwoClients      | Tests if a minimum size message (message length 1 byte, 1 byte data) is received by two clients.  |
| ClientNetworkReceive_FiveBytesSendToTwoClients_FiveBytesMessageReceivedByTwoClients         | Tests if a five size message (message length 5 byte, 5 byte data) is received by two clients.  |
| ClientNetworkReceive_MaximumBytesSendToTwoClients_MaximumBytesMessageReceivedByTwoClients |  Tests if a maximum size message (message length CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte, CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte data) is received by two clients.  |
| ClientNetworkReceive_MinimumBytesSendToFiveClients_MinimumBytesMessageReceivedByFiveClients      | Tests if a minimum size message (message length 1 byte, 1 byte data) is received by five clients.  |
| ClientNetworkReceive_FiveBytesSendToFiveClients_FiveBytesMessageReceivedByFiveClients         | Tests if a five size message (message length 5 byte, 5 byte data) is received by five clients.  |
| ClientNetworkReceive_MaximumBytesSendToFiveClients_MaximumBytesMessageReceivedByFiveClients |  Tests if a maximum size message (message length CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte, CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte data) is received by five clients.  |
| ClientNetworkReceive_MinimumBytesSendToMaximumClients_MinimumBytesMessageReceivedByMaximumClients      | Tests if a minimum size message (message length 1 byte, 1 byte data) is received by maximum clients.  |
| ClientNetworkReceive_FiveBytesSendToMaximumClients_FiveBytesMessageReceivedByMaximumClients         | Tests if a five size message (message length 5 byte, 5 byte data) is received by maximum clients.  |
| ClientNetworkReceive_MaximumBytesSendToMaximumClients_MaximumBytesMessageReceivedByMaximumClients |  Tests if a maximum size message (message length CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte, CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte data) is received by maximum clients.  |

###### TCP specific Send Tests (requires Connect Tests to pass)
These tests must pass as a requirement for other TCP network Send Tests and Receive Tests.
| Name     | Description  |
| :----------  | :---|
| ClientNetworkReceive_OneClientConnect_OneClientConnectionEstablished | Tests if one client can connect to the MQTT-SN Forwarder |
| ClientNetworkReceive_TwoClientsConnect_TwoClientConnectionsEstablished | Tests if two clients can connect to the MQTT-SN Forwarder |
| ClientNetworkReceive_FiveClientsConnect_FiveClientConnectionsEstablished | Tests if five clients can connect to the MQTT-SN Forwarder |
| ClientNetworkReceive_MaximumClientsConnect_MaximumClientConnectionsEstablished | Tests if five clients can connect to the MQTT-SN Forwarder |


##############################################################################

#### Gateway Network Tests
The GatewayNetwork performs the following action, these are to be tested:
* GatewayNetworkInit to initialize the network data structure or hardware components, e.g. initializing data structure for a TCP Socket
* GatewayNetworkConnect to connect to the network, e.g. connect to a MQTT-SN Gateway's TCP socket.
* GatewayNetworkDisconnect to disconnect from the network, e.g. close the TCP socket to the MQTT-SN Gateway.
* GatewayNetworkReceive to receive data from the MQTT-SN Gateway and puts them into a message buffer.
* GatewayNetworkSend to send data from the message buffer to the MQTT-SN Gateway.

###### Init Tests
| Name     | Description  |
| :----------  | :---|
| GatewayNetworkInit_Success_ReturnsZero                |   Tests the API for returning 0 on success. Tests if it is possible to initialize the network. |
| GatewayNetworkInit_Fails_ReturnsMinusOne              |   Tests the API for returning -1 on failure. |

###### Disconnect Tests (requires Connect Tests to pass)
Disconnect test are unique for each network, hardware and used datastructure.
We propose to implement these in a way which makes it possible to check afterwards if it disconnected.
Because this depends on many factors no general tests can be implemented.

###### Connect Tests (requieres Init Tests to pass)
| Name     | Description  |
| :----------  | :---|
| GatewayNetworkConnect_Success_ReturnsZero             |   Tests the API for returning 0 on success. Tests if it is possible to connect to the network.  |
| GatewayNetworkConnect_Fails_ReturnsMinusOne           |   Tests the API for returning -1 on failure. |

###### Receive Tests (requires Connect Tests to pass)
| Name     | Description  |
| :----------  | :---|
| GatewayNetworkReceive_MinimumBytesSend_MinimumBytesMessageReceived      | Tests if a minimum size messages (message length 1 byte, 1 byte data) is received.  |
| GatewayNetworkReceive_FiveBytesSend_FiveBytesMessageReceived         | Tests if a five size messages (message length 5 byte, 5 byte data) is received. |
| GatewayNetworkReceive_MaximumBytesSend_MaximumBytesMessageReceived |  Tests if a maximum size messages (message length CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte, CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte data) is received. |

###### Send Tests (requires Connect Tests to pass)
| Name     | Description  |
| :----------  | :---|
| GatewayNetworkSend_ZeroBytesToSend_NoMessageReceived_ReturnsZero     |   Tests the API for returning 0 on active network connection. Tests if empty messages are ignored.  |
| GatewayNetworkSend_MinimumBytesToSend_MinimumMessageReceived_ReturnsZero  |   Tests the API for returning 0 on active network connection. Tests if a minimum size messages (message length 1 byte, 1 byte data) is send and received.  |
| GatewayNetworkSend_FiveBytesToSend_FiveByteMessageReceived_ReturnsZero     |   Tests the API for returning 0 on active network connection. Tests if a five size messages (message length 5 byte, 5 byte data) is send and received.  |
| GatewayNetworkSend_MaximumBytesSend_MaximumByteMessageReceived_ReturnsZero   |   Tests the API for returning 0 on active network connection. Tests if a maximum size messages (message length CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte, CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte data) is send and received.  |

###### Receive Tests (requires Connect Tests to pass)
| Name     | Description  |
| :----------  | :---|
| GatewayNetworkReceive_MinimumBytesSend_MinimumBytesMessageReceived      | Tests if a minimum size messages (message length 1 byte, 1 byte data) is received.  |
| GatewayNetworkReceive_FiveBytesSend_FiveBytesMessageReceived         | Tests if a five size messages (message length 5 byte, 5 byte data) is received. |
| GatewayNetworkReceive_MaximumBytesSend_MaximumBytesMessageReceived |  Tests if a maximum size messages (message length CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte, CMQTTSNFORWARDER_MAXIMUM_MESSAGE_LENGTH byte data) is received. |

###### Protocol Specific Tests

###### Tcp only disconnect tests
| Name                                                  | Description  |
| :---------------------------------------------------  | :------------|
| GatewayLinuxTcpDisconnect_Gratefully_SocketIsZero     |   Tests if the socket filedescriptor is zero (0) after a grateful disconnect.  |
| GatewayLinuxTcpDisconnect_Ungratefully_SocketIsZero   |   Tests if the socket filedescriptor is zero (0) after and ungrateful disconnect.  |

## Getting Started

## Getting Started - Writing new tests


#[[
project(gmock-global-download NONE)

#include(ExternalProject)
ExternalProject_Add(gmock-global
  GIT_REPOSITORY    https://github.com/apriorit/gmock-global.git
  GIT_TAG           master
  SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/gmock-global-src"
  BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/gmock-global-build"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)
]]

What does not work:
searching for gatewaay - no dynamic connection - whitelisting
looking for advertisment
configurations files
monitoring



    add_executable(GatewayTests
            test/network/gateway/MockGatewayNetwork/MockGateway.cpp
            test/network/gateway/MockGatewayNetwork/MockGateway.h
            test/network/gateway/MockGatewayNetwork/MockGatewayNetworkInterface.h
            test/network/gateway/MockGatewayNetwork/MockGatewayNetworkReceiver.h
            test/network/gateway/MockGatewayNetwork/MockGatewayNetworkInterface.h

            test/network/gateway/MockForwarderGatewayNetworkLooper/MockForwarderGatewayNetworkLooper.cpp
            test/network/gateway/MockForwarderGatewayNetworkLooper/MockForwarderGatewayNetworkLooper.h

            test/network/gateway/TestConfigurations/MockGatewayConfiguration.h
            test/network/gateway/TestConfigurations/MqttSnForwarderGatewayNetworkTestConfiguration.h
            test/network/gateway/TestConfigurations/MqttSnGatewayNetworkMessageDataGenerator.cpp
            test/network/gateway/TestConfigurations/MqttSnGatewayNetworkMessageDataGenerator.h
            test/network/gateway/TestConfigurations/MqttSnGatewayNetworkValueParameter.h

            test/network/gateway/TestConfigurations/GetParameterMqttSnGatewayNetworkTestTypeParameter.cpp
            test/network/gateway/TestConfigurations/GetParameterMqttSnGatewayNetworkTestTypeParameter.h

            test/network/gateway/Tcp/MockGatewayLinuxTcpNetworkImplementation.cpp
            test/network/gateway/Tcp/MockGatewayLinuxTcpNetworkImplementation.h

            test/network/gateway/Tests/MqttSnGatewayNetworkInterfaceSendReceiveTests.h
            test/network/gateway/Tests/MqttSnGatewayNetworkInterfaceSendReceiveTests.cpp
            test/network/gateway/Tcp/LinuxTcpMqttSnGatewayNetworkInterfaceTests.cpp
            test/network/gateway/Tests/MqttSnGatewayNetworkInterfaceInitializationTests.cpp
            test/network/gateway/Tests/MqttSnGatewayNetworkInterfaceInitializationTests.h
            test/network/gateway/Tests/MqttSnGatewayNetworkInterfaceConnectTests.cpp
            test/network/gateway/Tests/MqttSnGatewayNetworkInterfaceConnectTests.h
            test/network/gateway/Tcp/LinuxTcpMqttSnGatewayNetworkDisconnectTests.cpp
            test/network/gateway/Tcp/LinuxTcpMqttSnGatewayNetworkDisconnectTests.h
            test/network/gateway/Tcp/TextFixture.h
            test/network/gateway/Tcp/TextFixture.cpp)

    target_link_libraries(GatewayTests
            gtest gtest_main gmock gmock_main pthread
            lib-mqtt-sn-forwarder
            lib-mqtt-sn-gateway-network)

--------


    # create a testing mock libraries for gateway network and client network
    add_library(lib-mqtt-sn-client-mock-network
            forwarder/global_defines.h
            test/network/client/MockClientNetwork/MockClient/MockClient.cpp
            test/network/client/MockClientNetwork/MockClient/MockClient.h
            test/network/client/MockClientNetwork/MockClient/MockClientNetworkInterface.h
            test/network/client/MockClientNetwork/MockClient/MockClientNetworkReceiverInterface.h
            test/network/client/MockClientNetwork/MockClient/MockClientNetworkReceiver.h
            test/network/client/MockClientNetwork/MockGateway/ClientNetworkGatewayLooper.cpp
            test/network/client/MockClientNetwork/MockGateway/ClientNetworkGatewayLooper.h
            test/network/client/MockClientNetwork/MockClient/ComparableClientMqttSnMessageData.h
            test/network/client/TestConfigurations/MqttSnClientNetworkTestValueParameter.h
            test/network/client/TestConfigurations/MqttSnGatewayClientNetworkTestConfiguration.h
            test/network/client/TestConfigurations/MockClientConfiguration.h
            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.cpp
            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.h
            test/network/client/TestConfigurations/ClientNetworkMessageDataGenerator.cpp
            test/network/client/TestConfigurations/ClientNetworkMessageDataGenerator.h
            test/network/client/TestConfigurations/CartesianProductTestCaseGenerator.cpp
            test/network/client/TestConfigurations/CartesianProductTestCaseGenerator.h
            test/network/client/Tcp/MockClientLinuxTcpNetworkImplementation.cpp
            test/network/client/Tcp/MockClientLinuxTcpNetworkImplementation.h
            test/network/client/Tcp/TcpMqttSnClientNetworkInterfaceTests.cpp)

    target_link_libraries(lib-mqtt-sn-client-mock-network
            gtest gtest_main gmock gmock_main pthread
            lib-mqtt-sn-gateway-network lib-mqtt-sn-forwarder lib-mqtt-sn-client-network)

    add_executable(MqttSnForwarder_LinuxTcpClientNetwork_Tests
            forwarder/global_defines.h
            test/network/client/MockClientNetwork/MockClient/MockClient.cpp
            test/network/client/MockClientNetwork/MockClient/MockClient.h
            test/network/client/MockClientNetwork/MockClient/MockClientNetworkInterface.h
            test/network/client/MockClientNetwork/MockClient/MockClientNetworkReceiverInterface.h
            test/network/client/MockClientNetwork/MockClient/MockClientNetworkReceiver.h
            test/network/client/MockClientNetwork/MockGateway/ClientNetworkGatewayLooper.cpp
            test/network/client/MockClientNetwork/MockGateway/ClientNetworkGatewayLooper.h
            test/network/client/MockClientNetwork/MockClient/ComparableClientMqttSnMessageData.h
            test/network/client/TestConfigurations/MqttSnClientNetworkTestValueParameter.h
            test/network/client/TestConfigurations/MqttSnGatewayClientNetworkTestConfiguration.h
            test/network/client/TestConfigurations/MockClientConfiguration.h
            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.cpp
            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.h
            test/network/client/TestConfigurations/ClientNetworkMessageDataGenerator.cpp
            test/network/client/TestConfigurations/ClientNetworkMessageDataGenerator.h
            test/network/client/TestConfigurations/CartesianProductTestCaseGenerator.cpp
            test/network/client/TestConfigurations/CartesianProductTestCaseGenerator.h

            test/network/client/Tcp/MockClientLinuxTcpNetworkImplementation.cpp
            test/network/client/Tcp/MockClientLinuxTcpNetworkImplementation.h
            test/network/client/Tcp/TcpMqttSnClientNetworkInterfaceTests.cpp)

    target_link_libraries(MqttSnForwarder_LinuxTcpClientNetwork_Tests
            lib-mqtt-sn-client-mock-network
            gtest gtest_main gmock gmock_main pthread
            lib-mqtt-sn-gateway-network lib-mqtt-sn-forwarder lib-mqtt-sn-client-network)


////////////////////////////////////


int log_gateway_connack_message(const MqttSnLogger *logger,
                                int level,
                                const device_address *address,
                                const uint8_t *data,
                                uint16_t data_len) {
  ParsedMqttSnHeader h = {0};
  if (parse_connack(&h, data, data_len)) {
    // TODO decide what to do
    return (log_current_time(logger) ||
        log_str(logger, "Invalid Message.") ||
        log_flush(logger) != 0);
  }
  MqttSnMessageConnack *c = (MqttSnMessageConnack *) h.payload;
  uint8_t return_code = c->returnCode;

  const char *connack_to = "CONNACK to ";
  const char *open_braked = " ( ";
  const char *close_braked = ").";
  // 1554750679: CONNACK to 127.0.0.1 (rc0).

  return (
      log_str(logger, connack_to) ||
          log_device_address(logger, address) ||
          log_str(logger, open_braked) ||
          log_return_code(logger, return_code) ||
          log_str(logger, close_braked) ||
          log_flush(logger) != 0);
}


int log_gateway_publish_message(const MqttSnLogger *logger,
                                int level,
                                const device_address *address,
                                const uint8_t *data,
                                uint16_t data_len) {
  ParsedMqttSnHeader h = {0};
  if (parse_publish(&h, data, data_len)) {
    return 0;
  }

  const char *publish_to = "gateway PUBLISH to ";
  const char *open_braked = " ( ";
  const char *comma = ", ";
  const char *dots = "... (";
  const char *bytes_close_braked = " bytes)).";

  MqttSnMessagePublish *p = (MqttSnMessagePublish *) h.payload;
  uint16_t msg_id = ntohs(p->msgId);
  uint16_t topic_id = ntohs(((MqttSnMessagePublish *) h.payload)->topicId);
  uint8_t flags = ((MqttSnMessagePublish *) h.payload)->flags;
  uint16_t msg_bytes = h.length;

  return (
      log_str(logger, publish_to) ||
          log_device_address(logger, address) ||
          log_str(logger, open_braked) ||
          log_message_id(logger, msg_id) ||
          log_str(logger, comma) ||
          log_topic_id(logger, topic_id) ||
          log_str(logger, comma) ||
          log_mqtt_sn_flags(logger, flags) ||
          log_str(logger, comma) ||
          log_str(logger, dots) ||
          log_uint16(logger, msg_bytes) ||
          log_str(logger, bytes_close_braked) ||
          log_flush(logger) != 0);
}


int log_gateway_disconnect_message(const MqttSnLogger *logger,
                                   int level,
                                   const device_address *address,
                                   const uint8_t *data,
                                   uint16_t data_len) {
  ParsedMqttSnHeader h = {0};
  if (parse_disconnect(&h, data, data_len)) {
    // TODO decide what to do
    return (log_current_time(logger) ||
        log_str(logger, "Invalid Message.") ||
        log_flush(logger) != 0);
  }

  const char *disconnect_from = "gateway DISCONNECT from ";
  const char *open_braked = " (";
  const char *close_braked = " ).";

  if (h.length == 2) {
    return (log_current_time(logger) ||
        log_str(logger, disconnect_from) ||
        log_device_address(logger, address) ||
        log_flush(logger) != 0);
  }
  if (h.length == 4) {
    MqttSnMessageDisconnect *p = (MqttSnMessageDisconnect *) h.payload;
    uint16_t duration = ntohs(p->duration);

    return (
        log_str(logger, disconnect_from) ||
            log_device_address(logger, address) ||
            log_str(logger, open_braked) ||
            log_duration(logger, duration) ||
            log_str(logger, close_braked) ||
            log_flush(logger) != 0);
  }
  return 0;
}