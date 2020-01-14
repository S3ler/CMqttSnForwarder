## Getting Started Linux
### Dependencies
sudo apt-get install cmake
sudo apt-get install libssl-dev
get docker: https://docs.docker.com/install/linux/docker-ce/ubuntu/
docker needs to be runable without sudo
docker pull eclipse-mosquitto:1.6.7

### Build and Run
mkdir cmake-build-debug
cd cmake-build-debug
cmake ../
cmake --build $PWD --target 10kconsecutivePublishQoS1Paylaod10Byte -- -j 2

## Getting Started Arduino ESP32
### Prepare
install arduino cli
apt install python-pip
pip install pyserial
board_manager:
  additional_urls:
    - http://arduino.esp8266.com/stable/package_esp8266com_index.json
    - http://dl.espressif.com/dl/package_esp32_index.json
[ ! -d ~/Arduino/libraries ] && mkdir -p ~/Arduino/libraries
cp arduinolibraries ~/Arduino/libraries
ln -s ~/Arduino/libraries/CMqttSnForwarder ~/CMqttSnForwarder
ln -s ~/Arduino/libraries/OLED ~/OLED

Ideen:
Automatisch docker container erstellen aus den executables
über python api überwachen
oder
nur executable ausführen

Frage: wie kann ich executables überwachen für lasttests?

Testumgebung:
Gateway:
Variablen:
| Unabhängige Variable | Abhängige Variable | Fixe Variablen |
| :------------------: | :-----------------:| :-------------:|
| Implemented  |   y  |
| Tested       |   n  |
| Example      |   n  |

Procedures:

Gateway Discovery Procedures:

Advertisement:
Gateway:
Advertisement Procedure
Advertisement Monitoring Procedure
Advertisement Pinging-Monitoring Procedure
Forwarder:
Advertisement Monitoring Procedure
Client:
Advertisement Monitoring Procedure

Search Gateway - Gateway Info:
Gateway:
SearchGW-GWinfo replying
GWInfo Monitoring Procedure
Forwarder:
GWInfo Monitoring Procedure
Client
GWInfo Monitoring Procedure

Connect Procedure:
Gateway: (variables: bool:[with clean session], bool:[with will], bool:[prior will], bool:[any prior subscriptions], bool:[any prior registrations])
Client connect with clean session without will without subscription without registrations
Client connect without clean session without will without subscription without registrations

Client: (variable: )
connect with clean session
connect without clean session
connect with will
connect without will



Featurelist:
* search gw for mqtt-sn-gateway     // FEATURE implement searching for gateway
* fix tests
* use network interface by address
* filter out non-mqtt-sn-gateway address on gateway network interface (shouldnt be necessary
* fix plugin
* plugin multicast
* parsing creates error messages which are printable
arduino
* fix arduino esp8266 interfaces
* tcp
* multicast in tcp  for
* default configuration (on error)
* enable es32 compatibility

 morgen: screen output, logger in forwarder refactoren

 Parser: message logging, message generating, check message parsing
 Forwarder: Same Broadcast Address and Port in Gateway and Client Network
 ClientPublish: Read lines from file and publish (including some delay)
 ClientSub: Load config from file and Read Linkes from file for subscribe
//  enable GW MQTT-SN message header check?

// https://stackoverflow.com/questions/6688909/c-program-to-get-the-values-of-ip-header-fields-from-linux-ip-stack/6689064#6689064

// : how to fix the problem that we cannot find out own IP-Address of bind interface when using IN_ADDR_ANY
// this leads to the problem that we cannot filter out own send message to multicast
// and then we forward our own multicast messages in the client network to the gateway and vice versa

// TODO mention: easily adeptable for other platforms like RTOS
//  encapsulatedMessage->crtl = 0;
//   receive and send broadcasting
// for more information see Encapsulated message in MQTT-SN spec


-cL redis://localhost:6379 -gL redis://localhost:6379 -gnp ./plugins/libhiredis_plugin.so -cnp ./plugins/libhiredis_plugin.so --debug
--debug -gP tcp -cP tcp -cA 127.0.0.1 -gA 127.0.0.1


plugin for logging
code coverage reports
test für forwarder core fixen
mehr tests schreiben
interface dokumenteiren
restliche logfunctionen für verbose


[![Build Status](http://arsmb.de/api/badges/S3ler/CMqttSnForwarder/status.svg)](http://arsmb.de/S3ler/CMqttSnForwarder)


# C MQTT-SN Forwarder
What does not work yet:
automatically choice of MQTT-SN Gateway (we need a SearchGW and parsing prodecure)
loading from configurationfile
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

