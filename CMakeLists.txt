cmake_minimum_required(VERSION 3.10)
project(CMqttSnForwarder C CXX)

option(WITH_ARDUINO "Include WITH_ARDUINO support?" ON)
if (NOT WITH_TESTING AND NOT WITH_ARDUINO)
    project(CMqttSnForwarder C)
elseif (WITH_TESTING)
    project(CMqttSnForwarder C CXX)
elseif (WITH_ARDUINO)
    project(CMqttSnForwarder C CXX)
endif ()

set(VERSION a1)
set(MAJOR 0)
set(MINOR 1)
set(TWEAK 0)
string(TIMESTAMP CMAKE_BUILD_TIMESTAMP "%d-%m-%Y %H:%M:%S")
set(MANUAL_WEBSITE https://github.com/S3ler/CMqttSnForwarder)
add_definitions(-DVERSION=\"${VERSION}\"
        -DMAJOR=${MAJOR}
        -DMINOR=${MINOR}
        -DTWEAK=${TWEAK}
        -DCMAKE_BUILD_TIMESTAMP=\"${CMAKE_BUILD_TIMESTAMP}\"
        -DMANUAL_WEBSITE=\"${MANUAL_WEBSITE}\")

if (DEFINED CMAKE_CXX_COMPILER_ENV_VAR)
    set(CMAKE_CXX_STANDARD 11)
endif ()
if (DEFINED CMAKE_C_COMPILER_ENV_VAR AND NOT DEFINED CMAKE_CXX_COMPILER_ENV_VAR)
    set(CMAKE_C_STANDARD 99)
endif ()
if (NOT DEFINED CMAKE_C_STANDARD AND NOT DEFINED CMAKE_CXX_STANDARD)
    message(FATAL_ERROR "Neither C nor C++ project")
endif ()

set(CMQTTSNFORWADER_ERROR_COMPILE_FLAGS -Wall -Wextra -pedantic -Werror
        -Wno-unused-parameter -Wno-missing-field-initializers -Wno-unused-value -Wno-unused-but-set-variable)

set(MQTT_SN_PLATFORM_ABSTRACTION_LAYER_FILES
        platform/device_address.c
        platform/device_address.h
        platform/platform_compatibility.c
        platform/platform_compatibility.h
        platform/MqttSnMessageData.c
        platform/MqttSnMessageData.h)
add_library(lib-mqtt-sn-platform-abstraction-layer ${MQTT_SN_PLATFORM_ABSTRACTION_LAYER_FILES})
target_include_directories(lib-mqtt-sn-platform-abstraction-layer PRIVATE .)
target_compile_options(lib-mqtt-sn-platform-abstraction-layer PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})

set(MQTT_SN_FIXED_SIZE_RING_BUFFER_FILES
        ringbuffer/MqttSnFixedSizeRingBuffer.h
        ringbuffer/MqttSnFixedSizeRingBuffer.c)
add_library(lib-mqtt-sn-ring-buffer ${MQTT_SN_FIXED_SIZE_RING_BUFFER_FILES})
target_include_directories(lib-mqtt-sn-ring-buffer PRIVATE .)
target_compile_options(lib-mqtt-sn-ring-buffer PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})

option(WITH_LOGGING "Include LOGGING support?" ON)
if (WITH_LOGGING)
    add_definitions(-DWITH_LOGGING)
    message("Building with LOGGING support")
    option(WITH_DEBUG_LOGGING "Include DEBUG_LOGGING support?" ON)
    if (WITH_DEBUG_LOGGING)
        message("Building with DEBUG_LOGGING support")
        add_definitions(-DWITH_DEBUG_LOGGING)
    endif ()
    set(CORE_LOGGING_FILES
            platform/device_address.h
            logging/MqttSnLoggingInterface.h
            logging/MqttSnLoggingInterface.c)
    set(ARDUINO_LOGGING_FILES
            logging/linux/stdout/StdoutLogging.c
            logging/linux/stdout/StdoutLogging.h)
    set(STDOUT_LOGGING_FILES
            logging/linux/stdout/StdoutLogging.c
            logging/linux/stdout/StdoutLogging.h)
    set(STDERR_LOGGING_FILES
            logging/linux/stderr/StderrLogging.c
            logging/linux/stderr/StderrLogging.h)

    set(LOGGING_FILES ${CORE_LOGGING_FILES} ${MQTT_SN_LOGGING_FILES} ${STDOUT_LOGGING_FILES} ${STDERR_LOGGING_FILES})

    add_library(lib-mqtt-sn-logging ${LOGGING_FILES})
    target_include_directories(lib-mqtt-sn-logging PRIVATE .)
    target_compile_options(lib-mqtt-sn-logging PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
endif ()

set(MQTT_SN_MESSAGE_PARSER_FILES
        platform/device_address.h
        parser/MqttSnMessageParser.c parser/MqttSnMessageParser.h
        parser/MqttSnAdvertiseMessage.c parser/MqttSnAdvertiseMessage.h
        parser/MqttSnSearchGwMessage.c parser/MqttSnSearchGwMessage.h
        parser/MqttSnGwInfoMessage.c parser/MqttSnGwInfoMessage.h
        parser/MqttSnConnectMessage.c parser/MqttSnConnectMessage.h
        parser/MqttSnConnackMessage.c parser/MqttSnConnackMessage.h
        parser/MqttSnWillTopicReq.c parser/MqttSnWillTopicReq.h
        parser/MqttSnWillTopicMessage.c parser/MqttSnWillTopicMessage.h
        parser/MqttSnWillMsgReqMessage.c parser/MqttSnWillMsgReqMessage.h
        parser/MqttSnWillMsgMessage.c parser/MqttSnWillMsgMessage.h
        parser/MqttSnRegister.c parser/MqttSnRegister.h
        parser/MqttSnRegAckMessage.c parser/MqttSnRegAckMessage.h
        parser/MqttSnPublishMessage.c parser/MqttSnPublishMessage.h
        parser/MqttSnPubackMessage.c parser/MqttSnPubackMessage.h
        parser/MqttSnPubrecMessage.c parser/MqttSnPubrecMessage.h
        parser/MqttSnSubscribeMessage.c parser/MqttSnSubscribeMessage.h
        parser/MqttSnUnsubscribeMessage.c parser/MqttSnUnsubscribeMessage.h
        parser/MqttSnSubackMessage.c parser/MqttSnSubackMessage.h
        parser/MqttSnUnsubackMessage.c parser/MqttSnUnsubackMessage.h
        parser/MqttSnPingReqMessage.c parser/MqttSnPingReqMessage.h
        parser/MqttSnPingRespMessage.c parser/MqttSnPingRespMessage.h
        parser/MqttSnDisconnectMessage.c parser/MqttSnDisconnectMessage.h
        parser/MqttSnForwarderEncapsulationMessage.c parser/MqttSnForwarderEncapsulationMessage.h
        parser/MqttSnWillTopicUpdMessage.c parser/MqttSnWillTopicUpdMessage.h
        parser/MqttSnWillMsgUpdMessage.c parser/MqttSnWillMsgUpdMessage.h
        parser/MqttSnWillTopicRespMessage.c parser/MqttSnWillTopicRespMessage.h
        parser/MqttSnWillMsgRespMessage.c parser/MqttSnWillMsgRespMessage.h)
if (WITH_LOGGING)
    set(MQTT_SN_MESSAGE_PARSER_FILES ${MQTT_SN_MESSAGE_PARSER_FILES}
            parser/logging/MqttSnAdvertiseMessageLogging.c
            parser/logging/MqttSnAdvertiseMessageLogging.h
            parser/logging/MqttSnMessageParserLogging.c
            parser/logging/MqttSnMessageParserLogging.h
            parser/logging/MqttSnSearchGwMessageLogging.c
            parser/logging/MqttSnSearchGwMessageLogging.h
            parser/logging/MqttSnGwInfoMessageLogging.c
            parser/logging/MqttSnGwInfoMessageLogging.h
            parser/logging/common/MqttSnMessageLogging.c
            parser/logging/common/MqttSnMessageLogging.h)
endif ()
add_library(lib-mqtt-sn-message-parser ${MQTT_SN_MESSAGE_PARSER_FILES})
target_include_directories(lib-mqtt-sn-message-parser PRIVATE .)
target_compile_options(lib-mqtt-sn-message-parser PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(lib-mqtt-sn-message-parser lib-mqtt-sn-logging)
option(WITH_LINUX_PLUGIN_NETWORK "Include WITH_LINUX_PLUGIN_NETWORK support?" ON)
if (WITH_LINUX_PLUGIN_NETWORK)
    add_definitions(-DWITH_LINUX_PLUGIN_NETWORK)
    message("Building with WITH_LINUX_PLUGIN_NETWORK support")

    option(WITH_LINUX_PLUGIN_CLIENT_NETWORK "Include WITH_LINUX_PLUGIN_CLIENT_NETWORK support?" ON)
    option(WITH_LINUX_PLUGIN_GATEWAY_NETWORK "Include WITH_LINUX_PLUGIN_GATEWAY_NETWORK support?" ON)
    if (NOT WITH_LINUX_PLUGIN_CLIENT_NETWORK AND NOT WITH_LINUX_PLUGIN_GATEWAY_NETWORK)
        message(FATAL_ERROR "With WITH_LINUX_PLUGIN_NETWORK but neither with WITH_LINUX_PLUGIN_CLIENT_NETWORK nor with WITH_LINUX_PLUGIN_GATEWAY_NETWORK")
    endif ()

    if (WITH_LINUX_PLUGIN_CLIENT_NETWORK)
        add_definitions(-DWITH_LINUX_CLIENT_NETWORK_PLUGIN)
        message("Building with WITH_LINUX_PLUGIN_CLIENT_NETWORK support")
        set(LINUX_CLIENT_PLUGIN_NETWORK_FILES
                network/MqttSnClientNetworkInterface.c
                network/MqttSnClientNetworkInterface.h
                network/linux/client/plugin/client_network_plugin_interface.c
                network/linux/client/plugin/client_network_plugin_interface.h
                network/linux/client/plugin/MqttSnClientPluginNetwork.h
                network/linux/client/plugin/MqttSnClientPluginNetwork.c)
        if (WITH_LOGGING)
            set(LINUX_CLIENT_PLUGIN_NETWORK_FILES ${LINUX_CLIENT_PLUGIN_NETWORK_FILES}
                    network/shared/client/logging/MqttSnDebugMessageLogging.c
                    network/shared/client/logging/MqttSnDebugMessageLogging.h
                    network/linux/shared/plugin/logging/MqttSnPluginLogger.h
                    network/linux/shared/plugin/logging/MqttSnPluginLogger.c)
        endif ()
        add_library(lib-mqtt-sn-linux-plugin-client-network ${LINUX_CLIENT_PLUGIN_NETWORK_FILES})
        target_include_directories(lib-mqtt-sn-linux-plugin-client-network PRIVATE .)
        target_compile_options(lib-mqtt-sn-linux-plugin-client-network PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
        target_link_libraries(lib-mqtt-sn-linux-plugin-client-network dl)
    endif ()

    if (WITH_LINUX_PLUGIN_GATEWAY_NETWORK)
        add_definitions(-DWITH_LINUX_GATEWAY_NETWORK_PLUGIN)
        message("Building with WITH_LINUX_PLUGIN_GATEWAY_NETWORK support")
        set(LINUX_GATEWAY_PLUGIN_NETWORK_FILES
                network/MqttSnGatewayNetworkInterface.c
                network/MqttSnGatewayNetworkInterface.h
                network/linux/gateway/plugin/gateway_network_plugin_interface.c
                network/linux/gateway/plugin/gateway_network_plugin_interface.h
                network/linux/gateway/plugin/MqttSnGatewayPluginNetwork.h
                network/linux/gateway/plugin/MqttSnGatewayPluginNetwork.c)
        if (WITH_LOGGING)
            set(LINUX_GATEWAY_PLUGIN_NETWORK_FILES ${LINUX_GATEWAY_PLUGIN_NETWORK_FILES}
                    network/shared/gateway/logging/MqttSnDebugMessageLogging.c
                    network/shared/gateway/logging/MqttSnDebugMessageLogging.h
                    network/linux/shared/plugin/logging/MqttSnPluginLogger.h
                    network/linux/shared/plugin/logging/MqttSnPluginLogger.c)
        endif ()
        add_library(lib-mqtt-sn-linux-plugin-gateway-network ${LINUX_GATEWAY_PLUGIN_NETWORK_FILES})
        target_include_directories(lib-mqtt-sn-linux-plugin-gateway-network PRIVATE .)
        target_compile_options(lib-mqtt-sn-linux-plugin-gateway-network PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
        target_link_libraries(lib-mqtt-sn-linux-plugin-gateway-network dl)
    endif ()
endif ()

option(WITH_LINUX_UDP_NETWORK "Include WITH_LINUX_UDP_NETWORK support?" ON)
if (WITH_LINUX_UDP_NETWORK)
    add_definitions(-DWITH_LINUX_UDP_NETWORK)
    add_definitions(-DWITH_LINUX_UDP_GATEWAY_NETWORK)
    add_definitions(-DWITH_LINUX_UDP_CLIENT_NETWORK)
    message("Building with WITH_LINUX_UDP_NETWORK support")
    option(WITH_LINUX_UDP_CLIENT_NETWORK_BROADCAST "Include WITH_LINUX_UDP_CLIENT_NETWORK_BROADCAST support?" ON)
    option(WITH_LINUX_UDP_GATEWAY_NETWORK_BROADCAST "Include WITH_LINUX_UDP_GATEWAY_NETWORK_BROADCAST support?" ON)


    if (WITH_LINUX_UDP_CLIENT_NETWORK_BROADCAST OR WITH_LINUX_UDP_GATEWAY_NETWORK_BROADCAST)
        add_definitions(-DWITH_UDP_BROADCAST) #TODO replace by WITH_LINUX_UDP_NETWORK_BROADCAST
        add_definitions(-DWITH_LINUX_UDP_NETWORK_BROADCAST) #TODO replace by WITH_LINUX_UDP_NETWORK_BROADCAST
    endif ()
    if (WITH_LINUX_UDP_CLIENT_NETWORK_BROADCAST)
        add_definitions(-DWITH_CLIENT_NETWORK_BROADCAST)
        add_definitions(-DWITH_LINUX_UDP_CLIENT_NETWORK_BROADCAST)
        add_definitions(-DWITH_UDP_BROADCAST_CLIENT) # TODO replace by WITH_LINUX_UDP_CLIENT_NETWORK_BROADCAST
        message("Building with WITH_LINUX_UDP_CLIENT_NETWORK_BROADCAST support")
    endif ()
    if (WITH_LINUX_UDP_GATEWAY_NETWORK_BROADCAST)
        add_definitions(-DWITH_GATEWAY_NETWORK_BROADCAST)
        add_definitions(-DWITH_LINUX_UDP_GATEWAY_NETWORK_BROADCAST)
        add_definitions(-DWITH_UDP_BROADCAST_GATEWAY) #TODO replace by WITH_LINUX_UDP_GATEWAY_NETWORK_BROADCAST
        message("Building with WITH_LINUX_UDP_GATEWAY_NETWORK_BROADCAST support")
    endif ()

    set(LINUX_UDP_GATEWAY_NETWORK_FILES
            network/MqttSnGatewayNetworkInterface.c
            network/MqttSnGatewayNetworkInterface.h
            network/linux/gateway/ip/udp/MqttSnGatewayUdpNetwork.h
            network/linux/gateway/ip/udp/MqttSnGatewayUdpNetwork.c
            network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.h
            network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.c
            network/shared/ip/IpHelper.h
            network/shared/ip/IpHelper.c
            network/linux/shared/ip/MqttSnIpNetworkHelper.h
            network/linux/shared/ip/MqttSnIpNetworkHelper.c)
    if (WITH_LINUX_UDP_GATEWAY_NETWORK_BROADCAST)
        set(LINUX_UDP_GATEWAY_NETWORK_FILES ${LINUX_UDP_GATEWAY_NETWORK_FILES}
                network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.c
                network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.h)
    endif ()
    if (WITH_LOGGING)
        set(LINUX_UDP_GATEWAY_NETWORK_FILES ${LINUX_UDP_GATEWAY_NETWORK_FILES}
                network/shared/ip/IpHelperLogging.c
                network/shared/ip/IpHelperLogging.h
                network/linux/shared/ip/MqttSnIpNetworkHelperLogging.h
                network/linux/shared/ip/MqttSnIpNetworkHelperLogging.c
                network/shared/gateway/logging/MqttSnDebugMessageLogging.c
                network/shared/gateway/logging/MqttSnDebugMessageLogging.h)
    endif ()
    add_library(lib-mqtt-sn-linux-udp-gateway-network ${LINUX_UDP_GATEWAY_NETWORK_FILES})
    target_include_directories(lib-mqtt-sn-linux-udp-gateway-network PRIVATE .)
    target_compile_options(lib-mqtt-sn-linux-udp-gateway-network PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})

    set(LINUX_UDP_CLIENT_NETWORK_FILES
            network/MqttSnClientNetworkInterface.c
            network/MqttSnClientNetworkInterface.h
            network/linux/client/ip/udp/MqttSnClientUdpNetwork.c
            network/linux/client/ip/udp/MqttSnClientUdpNetwork.h
            network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.h
            network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.c
            network/shared/ip/IpHelper.h
            network/shared/ip/IpHelper.c
            network/linux/shared/ip/MqttSnIpNetworkHelper.h
            network/linux/shared/ip/MqttSnIpNetworkHelper.c)
    if (WITH_LINUX_UDP_CLIENT_NETWORK_BROADCAST)
        set(LINUX_UDP_CLIENT_NETWORK_FILES ${LINUX_UDP_CLIENT_NETWORK_FILES}
                network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.c
                network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.h)
    endif ()
    if (WITH_LOGGING)
        set(LINUX_UDP_CLIENT_NETWORK_FILES ${LINUX_UDP_CLIENT_NETWORK_FILES}
                network/shared/ip/IpHelperLogging.c
                network/shared/ip/IpHelperLogging.h
                network/linux/shared/ip/MqttSnIpNetworkHelperLogging.h
                network/linux/shared/ip/MqttSnIpNetworkHelperLogging.c
                network/shared/client/logging/MqttSnDebugMessageLogging.c
                network/shared/client/logging/MqttSnDebugMessageLogging.h)
    endif ()
    add_library(lib-mqtt-sn-linux-udp-client-network ${LINUX_UDP_CLIENT_NETWORK_FILES})
    target_include_directories(lib-mqtt-sn-linux-udp-client-network PRIVATE .)
    target_compile_options(lib-mqtt-sn-linux-udp-client-network PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})

endif ()

option(WITH_LINUX_TCP_NETWORK "Include WITH_LINUX_TCP_NETWORK support?" ON)
if (WITH_LINUX_TCP_NETWORK)
    add_definitions(-DWITH_LINUX_TCP_NETWORK)
    message("Building with WITH_LINUX_TCP_NETWORK support")

    option(WITH_LINUX_TCP_GATEWAY_NETWORK "Include WITH_LINUX_TCP_GATEWAY_NETWORK support?" ON)
    option(WITH_LINUX_TCP_CLIENT_NETWORK "Include WITH_LINUX_TCP_CLIENT_NETWORK support?" ON)
    if (NOT WITH_LINUX_TCP_CLIENT_NETWORK AND NOT WITH_LINUX_TCP_GATEWAY_NETWORK)
        message(FATAL_ERROR "With WITH_LINUX_TCP_NETWORK but neither with WITH_LINUX_TCP_CLIENT_NETWORK nor with WITH_LINUX_TCP_GATEWAY_NETWORK")
    endif ()
    if (WITH_LINUX_TCP_CLIENT_NETWORK)
        add_definitions(-DWITH_LINUX_TCP_CLIENT_NETWORK)
        message("Building with WITH_LINUX_TCP_CLIENT_NETWORK support")
        option(WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST "Include WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST support?" ON)
        if (WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST)
            add_definitions(-DWITH_CLIENT_NETWORK_BROADCAST)
            add_definitions(-DWITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST)
            message("Building with WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST support")
        endif ()
    endif ()
    if (WITH_LINUX_TCP_GATEWAY_NETWORK)
        add_definitions(-DWITH_LINUX_TCP_GATEWAY_NETWORK)
        message("Building with WITH_LINUX_TCP_GATEWAY_NETWORK support")
        option(WITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST "Include WITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST support?" ON)
        if (WITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST)
            add_definitions(-DWITH_GATEWAY_NETWORK_BROADCAST)
            add_definitions(-DWITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST)
            message("Building with WITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST support")
        endif ()
    endif ()

    if (WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST OR WITH_LINUX_TCP_GATEWAY_NETWORK_BROADCAST)
        add_definitions(-DWITH_LINUX_TCP_NETWORK_BROADCAST)
    endif ()


    if (WITH_LINUX_TCP_GATEWAY_NETWORK)
        set(LINUX_TCP_GATWAY_NETWORK_FILES
                network/MqttSnGatewayNetworkInterface.c
                network/MqttSnGatewayNetworkInterface.h
                network/linux/gateway/ip/tcp/MqttSnGatewayTcpNetwork.h
                network/linux/gateway/ip/tcp/MqttSnGatewayTcpNetwork.c
                network/linux/shared/ip/tcphelper/MqttSnTcpNetworkMessageParser.c
                network/linux/shared/ip/tcphelper/MqttSnTcpNetworkMessageParser.h
                network/shared/ip/IpHelper.c
                network/shared/ip/IpHelper.h
                network/linux/shared/ip/MqttSnIpNetworkHelper.h
                network/linux/shared/ip/MqttSnIpNetworkHelper.c
                network/linux/shared/ip/tcphelper/MqttSnTcpNetworkHelper.c
                network/linux/shared/ip/tcphelper/MqttSnTcpNetworkHelper.h)
        if (WITH_LOGGING)
            set(LINUX_TCP_GATWAY_NETWORK_FILES ${LINUX_TCP_GATWAY_NETWORK_FILES}
                    network/shared/ip/IpHelperLogging.c
                    network/shared/ip/IpHelperLogging.h
                    network/linux/shared/ip/MqttSnIpNetworkHelperLogging.h
                    network/linux/shared/ip/MqttSnIpNetworkHelperLogging.c
                    network/shared/gateway/logging/MqttSnDebugMessageLogging.c
                    network/shared/gateway/logging/MqttSnDebugMessageLogging.h)
        endif ()
        add_library(lib-mqtt-sn-linux-tcp-gateway-network ${LINUX_TCP_GATWAY_NETWORK_FILES})
        target_include_directories(lib-mqtt-sn-linux-tcp-gateway-network PRIVATE .)
        target_compile_options(lib-mqtt-sn-linux-tcp-gateway-network PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
        if (WITH_LINUX_TCP_GATEWAY_NETWORK)
            target_link_libraries(lib-mqtt-sn-linux-tcp-gateway-network lib-mqtt-sn-linux-udp-gateway-network)
        endif ()
    endif ()

    if (WITH_LINUX_TCP_CLIENT_NETWORK)
        set(LINUX_TCP_CLIENT_NETWORK_FILES
                network/MqttSnClientNetworkInterface.c
                network/MqttSnClientNetworkInterface.h
                network/linux/client/ip/tcp/MqttSnClientTcpNetwork.h
                network/linux/client/ip/tcp/MqttSnClientTcpNetwork.c
                network/linux/shared/ip/tcphelper/MqttSnTcpNetworkMessageParser.c
                network/linux/shared/ip/tcphelper/MqttSnTcpNetworkMessageParser.h
                network/shared/ip/IpHelper.c
                network/shared/ip/IpHelper.h
                network/linux/shared/ip/MqttSnIpNetworkHelper.h
                network/linux/shared/ip/MqttSnIpNetworkHelper.c
                network/linux/shared/ip/tcphelper/MqttSnTcpNetworkHelper.c
                network/linux/shared/ip/tcphelper/MqttSnTcpNetworkHelper.h)
        if (WITH_LOGGING)
            set(LINUX_TCP_CLIENT_NETWORK_FILES ${LINUX_TCP_CLIENT_NETWORK_FILES}
                    network/shared/ip/IpHelperLogging.c
                    network/shared/ip/IpHelperLogging.h
                    network/linux/shared/ip/MqttSnIpNetworkHelperLogging.h
                    network/linux/shared/ip/MqttSnIpNetworkHelperLogging.c
                    network/shared/client/logging/MqttSnDebugMessageLogging.c
                    network/shared/client/logging/MqttSnDebugMessageLogging.h)
        endif ()
        add_library(lib-mqtt-sn-linux-tcp-client-network ${LINUX_TCP_CLIENT_NETWORK_FILES})
        target_include_directories(lib-mqtt-sn-linux-tcp-client-network PRIVATE .)
        target_compile_options(lib-mqtt-sn-linux-tcp-client-network PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
        if (WITH_LINUX_TCP_CLIENT_NETWORK_BROADCAST)
            target_link_libraries(lib-mqtt-sn-linux-tcp-client-network lib-mqtt-sn-linux-udp-client-network)
        endif ()
    endif ()
endif ()

if (NOT WITH_LINUX_PLUGIN_CLIENT_NETWORK AND NOT WITH_LINUX_UDP_CLIENT_NETWORK AND NOT WITH_LINUX_TCP_CLIENT_NETWORK)
    message(FATAL_ERROR "Neither WITH_LINUX_PLUGIN_CLIENT_NETWORK nor with WITH_LINUX_UDP_CLIENT_NETWORK nor with WITH_LINUX_TCP_CLIENT_NETWORK")
else ()
    add_library(lib-mqtt-sn-linux-client-network ${LINUX_CLIENT_PLUGIN_NETWORK_FILES} ${LINUX_UDP_CLIENT_NETWORK_FILES} ${LINUX_TCP_CLIENT_NETWORK_FILES})
    target_include_directories(lib-mqtt-sn-linux-client-network PRIVATE .)
    target_compile_options(lib-mqtt-sn-linux-client-network PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(lib-mqtt-sn-linux-client-network lib-mqtt-sn-ring-buffer)
endif ()

if (NOT WITH_LINUX_PLUGIN_GATEWAY_NETWORK AND NOT WITH_LINUX_UDP_GATEWAY_NETWORK AND NOT WITH_LINUX_TCP_GATEWAY_NETWORK)
    message(FATAL_ERROR "Neither with WITH_LINUX_PLUGIN_GATEWAY_NETWORK nor with WITH_LINUX_UDP_GATEWAY_NETWORK nor with WITH_LINUX_TCP_GATEWAY_NETWORK")
else ()
    add_library(lib-mqtt-sn-linux-gateway-network ${LINUX_GATEWAY_PLUGIN_NETWORK_FILES} ${LINUX_UDP_GATEWAY_NETWORK_FILES} ${LINUX_TCP_GATWAY_NETWORK_FILES})
    target_include_directories(lib-mqtt-sn-linux-gateway-network PRIVATE .)
    target_compile_options(lib-mqtt-sn-linux-gateway-network PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(lib-mqtt-sn-linux-gateway-network lib-mqtt-sn-ring-buffer)
endif ()

set(MQTT_SN_CLIENT_TEST_FILES
        client/pub/config/publish_client_config.c client/pub/config/publish_client_config.h
        config/network/gateway_network_config.c config/network/gateway_network_config.h
        config/client/mqtt_sn_gateway_config.c config/client/mqtt_sn_gateway_config.h
        config/common/config_command_helper.c config/common/config_command_helper.h
        config/common/mqtt_sn_version_config.c config/common/mqtt_sn_version_config.h
        config/network/client_network_config.c config/network/client_network_config.h
        config/common/mqtt_sn_logger_config.c config/common/mqtt_sn_logger_config.h
        client/sub/config/subscription_client_config.c client/sub/config/subscription_client_config.h
        gateway/echogateway/config/echogateway_config.c gateway/echogateway/config/echogateway_config.h
        config/client/client_connect_config.c config/client/client_connect_config.h
        config/client/client_register_config.c config/client/client_register_config.h
        config/client/client_subscribe_config.c config/client/client_subscribe_config.h
        config/client/client_publish_config.c config/client/client_publish_config.h
        config/client/client_find_mqtt_sn_gateway_config.c config/client/client_find_mqtt_sn_gateway_config.h
        config/gateway/gateway_advertisment_config.c config/gateway/gateway_advertisment_config.h
        gateway/echogateway/linux/starter/echogateway_starter.c gateway/echogateway/linux/starter/echogateway_starter.h
        gateway/echogateway/echogateway.c gateway/echogateway/echogateway.h
        )

set(MQTT_SN_CLIENT_PUB_FILES client/pub/linux/main.c ${MQTT_SN_CLIENT_TEST_FILES})
add_executable(MqttSnClientPub ${MQTT_SN_CLIENT_PUB_FILES})
target_include_directories(MqttSnClientPub PRIVATE .)
target_compile_options(MqttSnClientPub PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(MqttSnClientPub
        lib-mqtt-sn-ring-buffer
        lib-mqtt-sn-message-parser
        lib-mqtt-sn-logging
        lib-mqtt-sn-linux-client-network
        lib-mqtt-sn-linux-gateway-network)

set(MQTT_SN_CLIENT_SUB_FILES client/sub/linux/main.c ${MQTT_SN_CLIENT_TEST_FILES})
add_executable(MqttSnClientSub ${MQTT_SN_CLIENT_SUB_FILES})
target_include_directories(MqttSnClientSub PRIVATE .)
target_compile_options(MqttSnClientSub PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(MqttSnClientSub
        lib-mqtt-sn-ring-buffer
        lib-mqtt-sn-message-parser
        lib-mqtt-sn-logging
        lib-mqtt-sn-linux-client-network
        lib-mqtt-sn-linux-gateway-network)

set(MQTT_SN_ECHOGATEWAY_FILES gateway/echogateway/linux/main.c         parser/MqttSnMessageParser.c parser/MqttSnMessageParser.h
         ${MQTT_SN_CLIENT_TEST_FILES})
add_executable(MqttSnEchoGateway ${MQTT_SN_ECHOGATEWAY_FILES})
target_include_directories(MqttSnEchoGateway PRIVATE .)
target_compile_options(MqttSnEchoGateway PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(MqttSnEchoGateway
        lib-mqtt-sn-ring-buffer
        lib-mqtt-sn-message-parser
        lib-mqtt-sn-logging
        lib-mqtt-sn-linux-client-network
        pthread dl)

set(MQTT_SN_FORWARDER_FILES
        forwarder/MqttSnForwarder.c
        forwarder/MqttSnForwarder.h
        platform/device_address.c
        platform/device_address.h forwarder/MqttSnForwarderParser.c forwarder/MqttSnForwarderParser.h)
if (WITH_LOGGING)
    set(MQTT_SN_FORWARDER_FILES ${MQTT_SN_FORWARDER_FILES}
            forwarder/MqttSnForwarderLogging.c
            forwarder/MqttSnForwarderLogging.h
            parser/logging/MqttSnForwarderLoggingMessages.c
            parser/logging/MqttSnForwarderLoggingMessages.h)
endif ()
add_library(lib-mqtt-sn-forwarder ${MQTT_SN_FORWARDER_FILES})
target_include_directories(lib-mqtt-sn-forwarder PRIVATE .)
target_compile_options(lib-mqtt-sn-forwarder PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(lib-mqtt-sn-forwarder
        lib-mqtt-sn-ring-buffer
        lib-mqtt-sn-message-parser
        lib-mqtt-sn-linux-client-network
        lib-mqtt-sn-linux-gateway-network)
if (WITH_LOGGING)
    target_link_libraries(lib-mqtt-sn-forwarder lib-mqtt-sn-logging)
endif ()
if (WITH_LINUX_PLUGIN_NETWORK)
    target_link_libraries(lib-mqtt-sn-forwarder
            lib-mqtt-sn-linux-plugin-client-network
            lib-mqtt-sn-linux-plugin-gateway-network)
endif ()

set(MQTT_SN_FORWARDER_LINUX_STARTER_FILES
        forwarder/starter/linux/forwarder_starter.h
        forwarder/starter/linux/forwarder_starter.c
        network/shared/ip/IpHelper.c
        network/shared/ip/IpHelper.h)
if (WITH_LOGGING)
    set(MQTT_SN_FORWARDER_LINUX_STARTER_FILES ${MQTT_SN_FORWARDER_LINUX_STARTER_FILES}
            network/shared/ip/IpHelperLogging.c
            network/shared/ip/IpHelperLogging.h)
endif ()
add_library(lib-mqtt-sn-linux-forwarder-starter ${MQTT_SN_FORWARDER_LINUX_STARTER_FILES})
target_include_directories(lib-mqtt-sn-linux-forwarder-starter PRIVATE .)
target_compile_options(lib-mqtt-sn-linux-forwarder-starter PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(lib-mqtt-sn-linux-forwarder-starter lib-mqtt-sn-forwarder pthread)

set(MQTT_SN_FORWADER_CONFIG_FILES
        forwarder/config/forwarder_config.c
        forwarder/config/forwarder_config.h)
if (WITH_LOGGING)
    set(MQTT_SN_FORWADER_CONFIG_FILES ${MQTT_SN_FORWADER_CONFIG_FILES}
            forwarder/config/forwarder_config_logger.c
            forwarder/config/forwarder_config_logger.h)
endif ()
add_library(lib-mqtt-sn-forwarder-config ${MQTT_SN_FORWADER_CONFIG_FILES})
target_include_directories(lib-mqtt-sn-forwarder-config PRIVATE .)
target_compile_options(lib-mqtt-sn-forwarder-config PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})

option(WITH_MQTT_SN_FORWADER_CONFIG_FILE "Include WITH_MQTT_SN_FORWADER_CONFIG_FILE support?" ON)
if (WITH_MQTT_SN_FORWADER_CONFIG_FILE)
    message("Building with WITH_MQTT_SN_FORWADER_CONFIG_FILE support")
    add_definitions(-DWITH_MQTT_SN_FORWADER_CONFIG_FILE)
endif ()


add_executable(MqttSnForwarder forwarder/starter/linux/main.c)
target_include_directories(MqttSnForwarder PRIVATE .)
target_compile_options(MqttSnForwarder PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(MqttSnForwarder lib-mqtt-sn-forwarder lib-mqtt-sn-forwarder-config lib-mqtt-sn-linux-forwarder-starter lib-mqtt-sn-platform-abstraction-layer)

add_executable(TcpCMqttSnForwarderTester testclients/tcptestermain.c platform/device_address.h)
target_include_directories(TcpCMqttSnForwarderTester PRIVATE .)
target_link_libraries(TcpCMqttSnForwarderTester pthread)

add_executable(UdpCMqttSnForwarderTester testclients/udptestermain.c platform/device_address.h)
target_include_directories(UdpCMqttSnForwarderTester PRIVATE .)
target_link_libraries(UdpCMqttSnForwarderTester pthread)

option(WITH_HIREDIS_PLUGIN "Include HIREDIS_PLUGIN support?" OFF)
if (WITH_HIREDIS_PLUGIN)
    add_subdirectory(plugins)
endif ()

add_executable(UdpGatewayMulticastTester testclients/udpmulticastmain.c)
target_compile_definitions(UdpGatewayMulticastTester PRIVATE CLIENT_PORT=11110 MQTT_SN_MULTICAST_IP="224.1.1.100" MQTT_SN_MULTICAST_PORT=5353)
target_include_directories(UdpGatewayMulticastTester PRIVATE .)
target_link_libraries(UdpGatewayMulticastTester lib-mqtt-sn-forwarder lib-mqtt-sn-linux-udp-gateway-network lib-mqtt-sn-linux-gateway-network lib-mqtt-sn-linux-client-network)
if (WITH_LOGGING)
    target_link_libraries(UdpGatewayMulticastTester lib-mqtt-sn-logging)
endif ()


add_executable(UdpClientMulticastTester testclients/udpmulticastmain.c)
target_compile_definitions(UdpClientMulticastTester PRIVATE CLIENT_PORT=11111 MQTT_SN_MULTICAST_IP="224.1.1.101" MQTT_SN_MULTICAST_PORT=5352)
target_include_directories(UdpClientMulticastTester PRIVATE .)
target_link_libraries(UdpClientMulticastTester lib-mqtt-sn-forwarder lib-mqtt-sn-linux-udp-client-network lib-mqtt-sn-linux-gateway-network lib-mqtt-sn-linux-client-network)
if (WITH_LOGGING)
    target_link_libraries(UdpClientMulticastTester lib-mqtt-sn-logging)
endif ()

if (WITH_TESTING)
    message("Bulding as C++ project")

    if (${CMAKE_CXX_STANDARD} LESS 11)
        message(FATAL_ERROR "Cannot build with less than C++11")
    endif ()


    # https://github.com/google/googletest/blob/master/googletest/README.md#using-cmake

    # Download and unpack googletest at configure time
    configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
    if (result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif ()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
    if (result)
        message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif ()

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
            ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
            EXCLUDE_FROM_ALL)

    # The gtest/gtest_main targets carry header search path
    # dependencies automatically when using CMake 2.8.11 or
    # later. Otherwise we have to add them here ourselves.
    if (CMAKE_VERSION VERSION_LESS 2.8.11)
        include_directories("${gtest_SOURCE_DIR}/include")
    endif ()

    # add gmock-global
    include_directories(test/include/gmock-global)

    enable_testing()
    # Now simply link against gtest or gtest_main as needed. Eg

    # create a testing mock libraries for gateway network and client network

    #mock and test libraries
    add_library(mock-client-network
            test/shared/MockMqttSnClientNetwork/ClientNetworkMock.h
            test/shared/MockMqttSnClientNetwork/ClientNetworkMockInterface.cpp
            test/shared/MockMqttSnClientNetwork/ClientNetworkMockInterface.h)
    target_include_directories(mock-client-network PRIVATE .)
    target_link_libraries(mock-client-network gmock)

    add_library(mock-gateway-network
            test/shared/MockMqttSnGatewayNetwork/GatewayNetworkMock.h
            test/shared/MockMqttSnGatewayNetwork/GatewayNetworkMockInterface.cpp
            test/shared/MockMqttSnGatewayNetwork/GatewayNetworkMockInterface.h)
    target_include_directories(mock-gateway-network PRIVATE .)
    target_link_libraries(mock-gateway-network gmock)

    add_library(mock-fixed-size-ring-buffer
            test/shared/MockMqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferMock.h
            test/shared/MockMqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferMockInterface.cpp
            test/shared/MockMqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferMockInterface.h)
    target_include_directories(mock-fixed-size-ring-buffer PRIVATE .)
    target_link_libraries(mock-fixed-size-ring-buffer gmock)

    add_library(placeholder-network-context
            test/shared/PlaceholderNetworkContext/PlaceholderNetworkContext.cpp
            test/shared/PlaceholderNetworkContext/PlaceholderNetworkContext.h)
    target_include_directories(placeholder-network-context PRIVATE .)

    # test
    add_executable(mqtt-sn-client-network-interface-test
            network/MqttSnClientNetworkInterface.c
            network/MqttSnClientNetworkInterface.h
            test/MqttSnClientNetworkInterface/MqttSnClientNetworkInterfaceTests.cpp
            test/MqttSnClientNetworkInterface/MqttSnClientNetworkInterfaceTests.h)
    target_include_directories(mqtt-sn-client-network-interface-test PRIVATE .)
    target_link_libraries(mqtt-sn-client-network-interface-test gtest gtest_main gmock mock-client-network)

    add_executable(mqtt-sn-gateway-network-interface-test
            network/MqttSnGatewayNetworkInterface.c
            network/MqttSnGatewayNetworkInterface.h
            test/MqttSnGatewayNetworkInterface/MqttSnGatewayNetworkInterfaceTests.cpp
            test/MqttSnGatewayNetworkInterface/MqttSnGatewayNetworkInterfaceTests.h)
    target_include_directories(mqtt-sn-gateway-network-interface-test PRIVATE .)
    target_link_libraries(mqtt-sn-gateway-network-interface-test gtest gtest_main gmock mock-gateway-network)

    add_executable(mqtt-sn-fixed-size-ring-buffer-test
            ringbuffer/MqttSnFixedSizeRingBuffer.c
            ringbuffer/MqttSnFixedSizeRingBuffer.c
            test/MqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferTests.cpp
            test/MqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferTests.h)
    target_include_directories(mqtt-sn-fixed-size-ring-buffer-test PRIVATE .)
    target_link_libraries(mqtt-sn-fixed-size-ring-buffer-test gtest gtest_main lib-mqtt-sn-ring-buffer)

    add_executable(mqtt-sn-forwarder-test
            forwarder/MqttSnForwarder.c
            forwarder/MqttSnForwarder.h
            network/MqttSnClientNetworkInterface.c
            network/MqttSnClientNetworkInterface.h
            network/MqttSnGatewayNetworkInterface.c
            network/MqttSnGatewayNetworkInterface.h
            parser/MqttSnMessageParser.c
            parser/MqttSnMessageParser.h

            test/MqttSnForwarder/MqttSnForwarderTests.cpp
            test/MqttSnForwarder/MqttSnForwarderTests.h
            test/MqttSnForwarder/MqttSnForwarderTestsGlobalVariables.h

            test/MqttSnForwarder/ForwarderInitTest/MqttSnForwarderInitTests.cpp
            test/MqttSnForwarder/ForwarderInitTest/MqttSnForwarderInitTests.h

            test/MqttSnForwarder/ForwarderDeinitTest/MqttSnForwarderDeinitTests.cpp
            test/MqttSnForwarder/ForwarderDeinitTest/MqttSnForwarderDeinitTests.h

            test/MqttSnForwarder/ForwarderLoopTest/MqttSnForwarderLoopTests.cpp
            test/MqttSnForwarder/ForwarderLoopTest/MqttSnForwarderLoopTests.h

            test/MqttSnForwarder/AddForwardingHeaderToClientMessagesTest/AddForwardingHeaderToClientMessagesTests.cpp
            test/MqttSnForwarder/AddForwardingHeaderToClientMessagesTest/AddForwardingHeaderToClientMessagesTests.h

            test/MqttSnForwarder/RemoveForwardingHeaderFromGatewayMessageTest/RemoveForwardingHeaderFromGatewayMessagesTests.cpp
            test/MqttSnForwarder/RemoveForwardingHeaderFromGatewayMessageTest/RemoveForwardingHeaderFromGatewayMessagesTests.h

            test/MqttSnForwarder/SendBufferedMessagesToClientTest/SendBufferedMessagesToClientsTests.cpp
            test/MqttSnForwarder/SendBufferedMessagesToClientTest/SendBufferedMessagesToClientsTests.h

            test/MqttSnForwarder/SendBufferedMessagesToGatewayTest/SendBufferedMessagesToGatewayTests.cpp
            test/MqttSnForwarder/SendBufferedMessagesToGatewayTest/SendBufferedMessagesToGatewayTests.h)
    target_include_directories(mqtt-sn-forwarder-test PRIVATE .)
    target_link_libraries(mqtt-sn-forwarder-test gtest gtest_main gmock
            mock-client-network mock-gateway-network mock-fixed-size-ring-buffer
            lib-mqtt-sn-client-network lib-mqtt-sn-gateway-network lib-mqtt-sn-msg-parser)
    if (WITH_LOGGING)
        target_link_libraries(mqtt-sn-forwarder-test lib-mqtt-sn-logging)
    endif ()

    option(WITH_LINE_COVERAGE "Include LINE COVERAGE?" OFF)
    if (WITH_LINE_COVERAGE)
        if (${CMAKE_VERSION} VERSION_GREATER "3.12.0" AND CMAKE_COMPILER_IS_GNUCXX)
            LIST(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules")
            INCLUDE(CodeCoverage)
            APPEND_COVERAGE_COMPILER_FLAGS()
            LIST(APPEND Coverage_GENHTML_ARGS "--rc lcov_branch_coverage=1")
            set(COVERAGE_LCOV_EXCLUDES '/usr/*' 'test/*' '${PROJECT_SOURCE_DIR}/test/*' '${CMAKE_CURRENT_BINARY_DIR}/*' '${PROJECT_SOURCE_DIR}/forwarder/network/*')

            SETUP_TARGET_FOR_COVERAGE_LCOV(NAME "mqtt-sn-fixed-size-ring-buffer-test-coverage"
                    EXECUTABLE "mqtt-sn-fixed-size-ring-buffer-test"
                    DEPENDENCIES "mqtt-sn-fixed-size-ring-buffer-test")

            SETUP_TARGET_FOR_COVERAGE_LCOV(NAME "mqtt-sn-client-network-interface-test-coverage"
                    EXECUTABLE "mqtt-sn-client-network-interface-test"
                    DEPENDENCIES "mqtt-sn-client-network-interface-test")

            SETUP_TARGET_FOR_COVERAGE_LCOV(NAME "mqtt-sn-gateway-network-interface-test-coverage"
                    EXECUTABLE "mqtt-sn-gateway-network-interface-test"
                    DEPENDENCIES "mqtt-sn-gateway-network-interface-test")

            SETUP_TARGET_FOR_COVERAGE_LCOV_LOCAL_EXLUCDE(NAME "mqtt-sn-forwarder-test-coverage"
                    EXECUTABLE "mqtt-sn-forwarder-test"
                    DEPENDENCIES "mqtt-sn-forwarder-test")

        else ()
            message("Consider to switch to CMake 3.12.0 or newer and GNUCXX for test coverage support.")
        endif ()
    endif ()
endif ()

if (WITH_ARDUINO)
    message("Bulding as C++ project for Arduino")
    if (${CMAKE_CXX_STANDARD} LESS 11)
        message(FATAL_ERROR "Cannot build with less than C++11")
    endif ()

    if (WITH_ARDUINO)
        option(WITH_LINUX_FAKE_ARDUINO_CLASSES "Include WITH_LINUX_FAKE_ARDUINO_CLASSES support?" ON)
        if (WITH_LINUX_FAKE_ARDUINO_CLASSES)
            #target_compile_definitions(-DWITH_LINUX_FAKE_ARDUINO_CLASSES)
            #add_definitions(-DWITH_LINUX_FAKE_ARDUINO_CLASSES)
            set(LINUX_FAKE_ARDUINO_CLASSES_FILES
                    utility/linux/digital_fake/digital.c
                    utility/linux/digital_fake/digital.h
                    utility/linux/eeprom_fake/EEPROM.cpp
                    utility/linux/eeprom_fake/EEPROM.h
                    utility/linux/esp_fake/ESPClass.cpp
                    utility/linux/esp_fake/ESPClass.h
                    utility/linux/ipaddress_fake/IPAddress.cpp
                    utility/linux/ipaddress_fake/IPAddress.h
                    utility/linux/serial_fake/SerialClass.cpp
                    utility/linux/serial_fake/SerialClass.h
                    utility/linux/wifi_fake/ESP8266WiFi.cpp
                    utility/linux/wifi_fake/ESP8266WiFi.h)
            SET_SOURCE_FILES_PROPERTIES(${LINUX_FAKE_ARDUINO_CLASSES_FILES} PROPERTIES LANGUAGE CXX)
            set(Linux_FAKE_ARDUINO_CLASSES_INCLUDE_DIRECTORIES
                    .
                    utility/linux/digital_fake
                    utility/linux/eeprom_fake
                    utility/linux/esp_fake
                    utility/linux/ipaddress_fake
                    utility/linux/serial_fake
                    utility/linux/wifi_fake)
            add_library(lib-arduino-linux-fake ${LINUX_FAKE_ARDUINO_CLASSES_FILES})
            target_include_directories(lib-arduino-linux-fake PRIVATE ${Linux_FAKE_ARDUINO_CLASSES_INCLUDE_DIRECTORIES})
        endif ()
        set(MQTT_SN_ARDUINO_ESP8266_EEPROM_CONFIG_LINE_LOADER_FILES
                utility/arduino/eeprom/eeprom_config.h
                utility/arduino/eeprom/eeprom_config.c)
        if (WITH_LOGGING)
            set(MQTT_SN_ARDUINO_ESP8266_EEPROM_CONFIG_LINE_LOADER_FILES ${MQTT_SN_ARDUINO_ESP8266_EEPROM_CONFIG_LINE_LOADER_FILES}
                    utility/arduino/eeprom/eeprom_config_logging.c
                    utility/arduino/eeprom/eeprom_config_logging.h)
        endif ()
        add_library(lib-mqtt-sn-arduino-esp8266-eeprom-config-line-loader ${MQTT_SN_ARDUINO_ESP8266_EEPROM_CONFIG_LINE_LOADER_FILES})
        target_include_directories(lib-mqtt-sn-arduino-esp8266-eeprom-config-line-loader PRIVATE .)
        target_compile_options(lib-mqtt-sn-arduino-esp8266-eeprom-config-line-loader PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
        if (WITH_LINUX_FAKE_ARDUINO_CLASSES)
            SET_SOURCE_FILES_PROPERTIES(${MQTT_SN_ARDUINO_ESP8266_EEPROM_CONFIG_LINE_LOADER_FILES} PROPERTIES LANGUAGE CXX)
            target_compile_definitions(lib-mqtt-sn-arduino-esp8266-eeprom-config-line-loader PRIVATE WITH_LINUX_FAKE_ARDUINO_CLASSES)
            target_include_directories(lib-mqtt-sn-arduino-esp8266-eeprom-config-line-loader PRIVATE ${Linux_FAKE_ARDUINO_CLASSES_INCLUDE_DIRECTORIES})
        endif ()

        set(MQTT_SN_ARDUINO_ESP8266_EEPROM_FORWADER_CONFIG_LINE_LOADER_FILES
                utility/arduino/eeprom/eeprom_config.h
                utility/arduino/eeprom/eeprom_config.c
                utility/arduino/eeprom/forwarder/forwarder_eeprom_loader.c
                utility/arduino/eeprom/forwarder/forwarder_eeprom_loader.h)
        if (WITH_LOGGING)
            set(MQTT_SN_ARDUINO_ESP8266_EEPROM_FORWADER_CONFIG_LINE_LOADER_FILES ${MQTT_SN_ARDUINO_ESP8266_EEPROM_FORWADER_CONFIG_LINE_LOADER_FILES}
                    utility/arduino/eeprom/forwarder/forwarder_eeprom_loader_logging.c
                    utility/arduino/eeprom/forwarder/forwarder_eeprom_loader_logging.h)
        endif ()
        add_library(lib-mqtt-sn-arduino-esp8266-eeprom-forwarder-config-line-loader ${MQTT_SN_ARDUINO_ESP8266_EEPROM_FORWADER_CONFIG_LINE_LOADER_FILES})
        target_include_directories(lib-mqtt-sn-arduino-esp8266-eeprom-forwarder-config-line-loader PRIVATE .)
        target_link_libraries(lib-mqtt-sn-arduino-esp8266-eeprom-forwarder-config-line-loader lib-mqtt-sn-forwarder-config lib-mqtt-sn-arduino-esp8266-eeprom-config-line-loader)
        target_compile_options(lib-mqtt-sn-arduino-esp8266-eeprom-forwarder-config-line-loader PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
        if (WITH_LINUX_FAKE_ARDUINO_CLASSES)
            SET_SOURCE_FILES_PROPERTIES(${MQTT_SN_ARDUINO_ESP8266_EEPROM_FORWADER_CONFIG_LINE_LOADER_FILES} PROPERTIES LANGUAGE CXX)
            target_compile_definitions(lib-mqtt-sn-arduino-esp8266-eeprom-forwarder-config-line-loader PRIVATE WITH_LINUX_FAKE_ARDUINO_CLASSES)
            target_include_directories(lib-mqtt-sn-arduino-esp8266-eeprom-forwarder-config-line-loader PRIVATE ${Linux_FAKE_ARDUINO_CLASSES_INCLUDE_DIRECTORIES})
        endif ()

        set(MQTT_SN_ARDUINO_ESP8266_SYSTEM_FILES
                utility/arduino/system/system.c
                utility/arduino/system/system.h
                network/arduino/shared/ip/ArduinoIpAddressHelper.cpp
                network/arduino/shared/ip/ArduinoIpAddressHelper.hpp
                utility/arduino/eeprom/forwarder/forwarder_eeprom_loader.c
                utility/arduino/eeprom/forwarder/forwarder_eeprom_loader.h)
        if (WITH_LOGGING)
            set(MQTT_SN_ARDUINO_ESP8266_SYSTEM_FILES ${MQTT_SN_ARDUINO_ESP8266_SYSTEM_FILES}
                    utility/arduino/system/system_logging.c
                    utility/arduino/system/system_logging.h)
        endif ()
        add_library(lib-mqtt-sn-arduino-esp8266-system ${MQTT_SN_ARDUINO_ESP8266_SYSTEM_FILES})
        target_include_directories(lib-mqtt-sn-arduino-esp8266-system PRIVATE .)
        target_link_libraries(lib-mqtt-sn-arduino-esp8266-system lib-mqtt-sn-arduino-esp8266-eeprom-forwarder-config-line-loader)
        target_compile_options(lib-mqtt-sn-arduino-esp8266-system PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
        target_compile_definitions(lib-mqtt-sn-arduino-esp8266-system PRIVATE WITH_EEPROM WITH_RESTART)

        if (WITH_LINUX_FAKE_ARDUINO_CLASSES)
            SET_SOURCE_FILES_PROPERTIES(${MQTT_SN_ARDUINO_ESP8266_SYSTEM_FILES} PROPERTIES LANGUAGE CXX)
            target_compile_definitions(lib-mqtt-sn-arduino-esp8266-system PRIVATE WITH_LINUX_FAKE_ARDUINO_CLASSES)
            target_include_directories(lib-mqtt-sn-arduino-esp8266-system PRIVATE ${Linux_FAKE_ARDUINO_CLASSES_INCLUDE_DIRECTORIES})
        endif ()
    endif ()
endif ()
