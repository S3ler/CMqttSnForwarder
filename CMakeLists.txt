cmake_minimum_required(VERSION 3.10)
project(CMqttSnForwarder C CXX)

option(WITH_ARDUINO "Include WITH_ARDUINO support?" OFF)
if (NOT WITH_TESTING AND NOT WITH_ARDUINO)
    project(CMqttSnForwarder C)
elseif (WITH_TESTING)
    project(CMqttSnForwarder C CXX)
elseif (WITH_ARDUINO)
    project(CMqttSnForwarder C CXX)
endif ()

MESSAGE(STATUS "CMake version: " ${CMAKE_VERSION})
MESSAGE(STATUS "CMake system name: " ${CMAKE_SYSTEM_NAME})

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
        ringbuffer/logging/MqttSnFixedSizeRingBufferLogging.c
        ringbuffer/logging/MqttSnFixedSizeRingBufferLogging.h
        ringbuffer/MqttSnFixedSizeRingBuffer.h
        ringbuffer/MqttSnFixedSizeRingBuffer.c)
add_library(lib-mqtt-sn-ring-buffer ${MQTT_SN_FIXED_SIZE_RING_BUFFER_FILES})
target_include_directories(lib-mqtt-sn-ring-buffer PRIVATE .)
target_compile_options(lib-mqtt-sn-ring-buffer PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(lib-mqtt-sn-ring-buffer lib-mqtt-sn-platform-abstraction-layer)
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

    set(FILE_LOGGING_FILES
            logging/linux/file/FileLogging.c
            logging/linux/file/FileLogging.h)
    set(FILE_STDOUT_LOGGING_FILES
            logging/linux/filestdout/FileStdoutLogging.c
            logging/linux/filestdout/FileStdoutLogging.h)
    set(STDOUT_LOGGING_FILES
            logging/linux/stdout/StdoutLogging.c
            logging/linux/stdout/StdoutLogging.h)
    set(STDERR_LOGGING_FILES
            logging/linux/stderr/StderrLogging.c
            logging/linux/stderr/StderrLogging.h)
    set(LINUX_LOGGING_STARTER_FILES
            logging/linux/starter/linux_logging_starter.c
            logging/linux/starter/linux_logging_starter.h)

    set(LOGGING_FILES ${CORE_LOGGING_FILES} ${MQTT_SN_LOGGING_FILES} ${FILE_LOGGING_FILES} ${FILE_STDOUT_LOGGING_FILES} ${STDOUT_LOGGING_FILES} ${STDERR_LOGGING_FILES})

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

add_library(lib-mqtt-sn-common-config
        config/common/config_command_helper.c config/common/config_command_helper.h
        config/common/mqtt_sn_version_config.c config/common/mqtt_sn_version_config.h
        config/common/mqtt_sn_logger_config.c config/common/mqtt_sn_logger_config.h
        config/client/mqtt_sn_gateway_config.c config/client/mqtt_sn_gateway_config.h
        config/starter/starter_helper.c config/starter/starter_helper.h)
target_include_directories(lib-mqtt-sn-common-config PRIVATE .)
target_compile_options(lib-mqtt-sn-common-config PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(lib-mqtt-sn-common-config lib-mqtt-sn-logging)

add_library(lib-mqtt-sn-client-network-config config/network/client/client_network_config.c config/network/client/client_network_config.h)
target_include_directories(lib-mqtt-sn-client-network-config PRIVATE .)
target_compile_options(lib-mqtt-sn-client-network-config PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(lib-mqtt-sn-client-network-config lib-mqtt-sn-common-config)

add_library(lib-mqtt-sn-gateway-network-config config/network/gateway/gateway_network_config.c config/network/gateway/gateway_network_config.h)
target_include_directories(lib-mqtt-sn-gateway-network-config PRIVATE .)
target_compile_options(lib-mqtt-sn-gateway-network-config PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(lib-mqtt-sn-gateway-network-config lib-mqtt-sn-common-config)

add_library(lib-mqtt-sn-find-gateway-config config/client/client_find_mqtt_sn_gateway_config.c config/client/client_find_mqtt_sn_gateway_config.h)
target_include_directories(lib-mqtt-sn-find-gateway-config PRIVATE .)
target_compile_options(lib-mqtt-sn-find-gateway-config PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(lib-mqtt-sn-find-gateway-config lib-mqtt-sn-common-config)

add_library(lib-mqtt-sn-client-config
        config/client/client_connect_config.c config/client/client_connect_config.h
        config/client/client_publish_config.c config/client/client_publish_config.h
        config/client/client_register_config.c config/client/client_register_config.h
        config/client/client_subscribe_config.c config/client/client_subscribe_config.h
        config/client/mqtt_sn_gateway_config.c config/client/mqtt_sn_gateway_config.h
        config/client/MqttSnClient_config.c config/client/MqttSnClient_config.h)
target_include_directories(lib-mqtt-sn-client-config PRIVATE .)
target_compile_options(lib-mqtt-sn-client-config PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(lib-mqtt-sn-client-config lib-mqtt-sn-common-config lib-mqtt-sn-find-gateway-config)

add_library(lib-mqtt-sn-gateway-config
        config/gateway/gateway_advertisment_config.c config/gateway/gateway_advertisment_config.h
        config/gateway/gateway_client_connection_config.c config/gateway/gateway_client_connection_config.h
        gateway/config/gateway_config.c gateway/config/gateway_config.h
        #config/gateway/broker/broker_config.c config/gateway/broker/broker_config.h
        )
target_include_directories(lib-mqtt-sn-gateway-config PRIVATE .)
target_compile_options(lib-mqtt-sn-gateway-config PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(lib-mqtt-sn-gateway-config lib-mqtt-sn-client-network-config lib-mqtt-sn-common-config)

add_executable(MqttSnGateway gateway/starter/linux/main.c)
target_compile_options(MqttSnGateway PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_include_directories(MqttSnGateway PRIVATE .)
target_link_libraries(MqttSnGateway lib-mqtt-sn-linux-gateway-starter lib-mqtt-sn-gateway-config lib-mqtt-sn-gateway lib-mqtt-sn-linux-client-network)
if (WITH_LOGGING)
    target_link_libraries(MqttSnGateway lib-mqtt-sn-logging)
endif ()
if (WITH_LINUX_PLUGIN_NETWORK)
    target_link_libraries(MqttSnGateway lib-mqtt-sn-linux-plugin-client-network)
endif ()


add_library(lib-mqtt-sn-find-gateway client/MqttSnFindGatewayClient.c client/MqttSnFindGatewayClient.h)
target_include_directories(lib-mqtt-sn-find-gateway PRIVATE .)
target_compile_options(lib-mqtt-sn-find-gateway PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(lib-mqtt-sn-find-gateway lib-mqtt-sn-message-parser lib-mqtt-sn-linux-gateway-network)

add_subdirectory(external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201)
set(FATFS_FILES
        external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/diskio.c
        external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/diskio.h
        external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ff.c
        external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ff.h
        external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ffconf.h
        external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ffsystem.c
        external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/ffunicode.c
        external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source/integer.h)
add_library(lib-fatfs ${FATFS_FILES})
#target_compile_options(lib-fatfs PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
#target_include_directories(lib-fatfs PRIVATE external/linux/gadget114514/fatfs-example-c00d2f9a408f195182f63c79a160ddbad37a5201/fatfs/source)

set(DATABASE_FILES
        gateway/database/db_entry_mqtt_sn_client.c
        gateway/database/db_entry_mqtt_sn_client.h
        gateway/database/db_entry_mqtt_sn_client_publish.h
        gateway/database/db_entry_mqtt_sn_client_registration.h
        gateway/database/db_entry_mqtt_sn_client_will_message.h
        # gateway/database/db_entry_mqtt_sn_gateway_discovered.c
        gateway/database/db_entry_mqtt_sn_gateway_discovered.h
        gateway/database/db_entry_mqtt_sn_subscription.h
        gateway/database/db_entry_mqtt_subscription.h
        gateway/database/db_handler.c
        gateway/database/db_handler.h
        gateway/database/db_handler_result.h
        gateway/database/db_handler_gateway_discovered.c
        gateway/database/db_handler_client.c
        gateway/database/ftfs_db.c
        gateway/database/ftfs_db.h)
if (WITH_LOGGING)
    set(DATABASE_FILES ${DATABASE_FILES}
            gateway/database/logging/db_logging.c
            gateway/database/logging/db_logging.h)
endif ()
add_library(lib-mqtt-sn-gateway-database ${DATABASE_FILES})
target_compile_definitions(lib-mqtt-sn-gateway-database PUBLIC WITH_DB_DEBUG_LOGGING)
target_compile_options(lib-mqtt-sn-gateway-database PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_include_directories(lib-mqtt-sn-gateway-database PRIVATE .)
target_link_libraries(lib-mqtt-sn-gateway-database lib-fatfs lib-mqtt-sn-message-parser lib-mqtt-sn-logging)
if (WITH_LOGGING)
    target_link_libraries(lib-mqtt-sn-gateway-database lib-mqtt-sn-logging)
endif ()

set(MQTT_SN_GATEWAY_FILES
        gateway/MqttSnGateway.c
        gateway/MqttSnGateway.h
        gateway/MqttSnGatewayDiscoveryHandler.c gateway/MqttSnGatewayDiscoveryHandler.h
        gateway/MqttSnGatewayClientConnectionHandler.c gateway/MqttSnGatewayClientConnectionHandler.h
        gateway/MqttSnGatewayForwarder.c gateway/MqttSnGatewayForwarder.h
        gateway/MqttClient.c
        gateway/MqttClient.h
        )
if (WITH_LOGGING)
    set(MQTT_SN_GATEWAY_FILES ${MQTT_SN_GATEWAY_FILES}
            gateway/logging/MqttSnGatewayLogging.c
            gateway/logging/MqttSnGatewayLogging.h
            gateway/logging/MqttSnGatewayClientConnectionLogging.c
            gateway/logging/MqttSnGatewayClientConnectionLogging.h
            gateway/logging/MqttSnGatewayDiscoveryLogging.c
            gateway/logging/MqttSnGatewayDiscoveryLogging.h)
endif ()
add_library(lib-mqtt-sn-gateway ${MQTT_SN_GATEWAY_FILES})
target_compile_options(lib-mqtt-sn-gateway PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_include_directories(lib-mqtt-sn-gateway PRIVATE .)
target_link_libraries(lib-mqtt-sn-gateway lib-mqtt-sn-gateway-database lib-mqtt-sn-message-parser lib-mqtt-sn-logging lib-mqtt-sn-platform-abstraction-layer pthread)
if (WITH_LOGGING)
    target_link_libraries(lib-mqtt-sn-gateway lib-mqtt-sn-logging)
endif ()

option(WITH_MQTT_SN_CLIENT "Include WITH_MQTT_SN_CLIENT support?" ON)
if (WITH_MQTT_SN_CLIENT)
    add_library(lib-mqtt-sn-client
            client/MqttSnClient.c client/MqttSnClient.h
            client/MqttSnClientAwaitMessage.c client/MqttSnClientAwaitMessage.h
            client/MqttSnClientLogger.c client/MqttSnClientLogger.h
            client/pub/config/publish_client_config.c client/pub/config/publish_client_config.h)
    target_include_directories(lib-mqtt-sn-client PRIVATE .)
    target_compile_options(lib-mqtt-sn-client PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(lib-mqtt-sn-client lib-mqtt-sn-message-parser lib-mqtt-sn-linux-gateway-network lib-mqtt-sn-platform-abstraction-layer)
    if (WITH_LOGGING)
        target_link_libraries(lib-mqtt-sn-client lib-mqtt-sn-logging)
    endif ()
    if (WITH_LINUX_PLUGIN_NETWORK)
        target_link_libraries(lib-mqtt-sn-client lib-mqtt-sn-linux-plugin-gateway-network)
    endif ()

    add_executable(MqttSnFindGatewayMonitor client/findgatewaymonitor/linux/main.c
            client/findgatewaymonitor/linux/starter/publish_client_starter.c client/findgatewaymonitor/linux/starter/publish_client_starter.h
            platform/platform_compatibility.c platform/platform_compatibility.h
            client/findgatewaymonitor/config/find_gateway_monitor_config.c client/findgatewaymonitor/config/find_gateway_monitor_config.h
            parser/logging/MqttSnForwarderLoggingMessages.c parser/logging/MqttSnForwarderLoggingMessages.h
            parser/MqttSnMessageParser.c parser/MqttSnMessageParser.h)
    target_compile_definitions(MqttSnFindGatewayMonitor PUBLIC CLIENT_FIND_MQTT_SN_GATEWAY_CONFIG_DEFAULT_FIND_PATTERN=ADVERTISMENT_FIND_MQTT_SN_GATEWAY_PATTERN_TYPE)
    target_include_directories(MqttSnFindGatewayMonitor PRIVATE .)
    target_compile_options(MqttSnFindGatewayMonitor PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(MqttSnFindGatewayMonitor
            lib-mqtt-sn-platform-abstraction-layer
            lib-mqtt-sn-find-gateway
            lib-mqtt-sn-find-gateway-config
            lib-mqtt-sn-gateway-network-config
            pthread)
    if (WITH_LINUX_PLUGIN_NETWORK)
        target_link_libraries(MqttSnFindGatewayMonitor lib-mqtt-sn-linux-plugin-gateway-network)
    endif ()
    add_executable(MqttSnClientPub client/pub/linux/main.c
            client/pub/linux/starter/publish_client_starter.c client/pub/linux/starter/publish_client_starter.h
            parser/MqttSnMessageParser.c parser/MqttSnMessageParser.h)
    target_include_directories(MqttSnClientPub PRIVATE .)
    target_compile_options(MqttSnClientPub PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(MqttSnClientPub lib-mqtt-sn-client lib-mqtt-sn-client-config lib-mqtt-sn-gateway-network-config pthread)
endif ()

option(WITH_MQTT_SN_FORWARDER "Include WITH_MQTT_SN_FORWARDER support?" ON)
if (WITH_MQTT_SN_FORWARDER)
    set(MQTT_SN_FORWARDER_FILES
            forwarder/MqttSnForwarder.c
            forwarder/MqttSnForwarder.h
            platform/device_address.c
            platform/device_address.h
            forwarder/MqttSnForwarderParser.c
            forwarder/MqttSnForwarderParser.h)
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

    add_library(lib-mqtt-sn-forwarder-config forwarder/config/forwarder_config.c forwarder/config/forwarder_config.h)
    target_include_directories(lib-mqtt-sn-forwarder-config PRIVATE .)
    target_compile_options(lib-mqtt-sn-forwarder-config PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(lib-mqtt-sn-forwarder-config lib-mqtt-sn-client-network-config lib-mqtt-sn-gateway-network-config lib-mqtt-sn-client-config)

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
    target_link_libraries(lib-mqtt-sn-linux-forwarder-starter lib-mqtt-sn-forwarder-config lib-mqtt-sn-forwarder pthread)


    option(WITH_MQTT_SN_FORWADER_CONFIG_FILE "Include WITH_MQTT_SN_FORWADER_CONFIG_FILE support?" ON)
    if (WITH_MQTT_SN_FORWADER_CONFIG_FILE)
        message("Building with WITH_MQTT_SN_FORWADER_CONFIG_FILE support")
        add_definitions(-DWITH_MQTT_SN_FORWADER_CONFIG_FILE)
    endif ()


    add_executable(MqttSnForwarder forwarder/starter/linux/main.c)
    target_include_directories(MqttSnForwarder PRIVATE .)
    target_compile_options(MqttSnForwarder PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(MqttSnForwarder lib-mqtt-sn-forwarder lib-mqtt-sn-forwarder-config lib-mqtt-sn-linux-forwarder-starter lib-mqtt-sn-platform-abstraction-layer)

endif ()

option(WITH_MQTT_SN_GATEWAY "Include WITH_MQTT_SN_GATEWAY support?" ON)
if (WITH_MQTT_SN_GATEWAY)
    file(GLOB PAHO_SOURCE_FILES
            gateway_old/paho/pahomqttembeddedc/MQTTPacket/src/*.c
            gateway_old/paho/pahomqttembeddedc/MQTTPacket/src/*.h
            gateway_old/paho/pahomqttembeddedc/MQTTClient/src/*.cpp
            gateway_old/paho/pahomqttembeddedc/MQTTClient/src/*.h
            gateway_old/paho/pahomqttembeddedc/MQTTClient/src/linux/*.cpp
            gateway_old/paho/pahomqttembeddedc/MQTTClient/src/linux/*.h)
    set(PAHO_SOURCE_FILES ${PAHO_SOURCE_FILES} gateway_old/paho/PahoMqttMessageHandler.h gateway_old/paho/PahoMqttMessageHandler.cpp
            gateway_old/paho/pahomqttembeddedc/MQTTClient/src/FP.h
            gateway_old/paho/pahomqttembeddedc/MQTTClient/src/MQTTClient.hpp
            gateway_old/paho/pahomqttembeddedc/MQTTClient/src/MQTTLogging.h
            )
    add_library(lib-paho-mqtt-client ${PAHO_SOURCE_FILES})
    target_include_directories(lib-paho-mqtt-client PRIVATE .)
    #target_compile_options(lib-paho-mqtt-client PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})

    add_definitions(-DGATEWAY_TRANSMISSION_PROTOCOL_UDP)
    set(BUILD_LINUX_UDP ON)
    set(MQTT_SN_GATEWAY_FILES
            gateway_old/abstraction/Arduino.cpp gateway_old/abstraction/Arduino.h
            gateway_old/abstraction/SD.cpp gateway_old/abstraction/SD.h
            gateway_old/core/core_defines.h
            gateway_old/core/CoreImpl.cpp
            gateway_old/core/CoreImpl.h
            gateway_old/core/CoreInterface.h
            gateway_old/core/Gateway.cpp
            gateway_old/core/Gateway.h
            gateway_old/core/global_defines.h
            gateway_old/core/LoggerInterface.h
            gateway_old/core/MqttMessageHandlerInterface.cpp
            gateway_old/core/MqttMessageHandlerInterface.h
            gateway_old/core/mqttsn_messages.h
            gateway_old/core/MqttSnMessageHandler.cpp
            gateway_old/core/MqttSnMessageHandler.h
            gateway_old/core/PersistentInterface.cpp
            gateway_old/core/PersistentInterface.h
            gateway_old/core/SocketInterface.cpp
            gateway_old/core/SocketInterface.h
            gateway_old/core/System.h
            gateway_old/implementation/LinuxGateway.cpp gateway_old/implementation/LinuxGateway.h
            gateway_old/implementation/LinuxLogger.cpp gateway_old/implementation/LinuxLogger.h
            gateway_old/implementation/LinuxPersistent.cpp gateway_old/implementation/LinuxPersistent.h
            gateway_old/implementation/LinuxSystem.cpp gateway_old/implementation/LinuxSystem.h
            gateway_old/Udp/LinuxUdpSocket.cpp gateway_old/Udp/LinuxUdpSocket.h)
    if (WITH_LOGGING)
        set(MQTT_SN_GATEWAY_FILES ${MQTT_SN_GATEWAY_FILES}
                gateway_old/implementation/LinuxLogger.cpp
                gateway_old/implementation/LinuxLogger.h)
    endif ()
    add_library(lib-mqtt-sn-gateway_old ${MQTT_SN_GATEWAY_FILES})
    target_include_directories(lib-mqtt-sn-gateway_old PRIVATE .)
    #target_compile_options(lib-mqtt-sn-gateway PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(lib-mqtt-sn-gateway_old lib-paho-mqtt-client pthread)

    add_executable(linux-mqtt-sn-gateway_old gateway_old/main.cpp)
    target_link_libraries(linux-mqtt-sn-gateway_old lib-mqtt-sn-gateway_old lib-paho-mqtt-client)
    target_include_directories(linux-mqtt-sn-gateway_old PRIVATE .)

    #        lib-mqtt-sn-ring-buffer
    #        lib-mqtt-sn-message-parser
    #        lib-mqtt-sn-linux-client-network
    #        lib-paho-mqtt-client
    #        lib-mqtt-sn-platform-abstraction-layer)
    #if (WITH_LOGGING)
    #    target_link_libraries(lib-mqtt-sn-gateway lib-mqtt-sn-logging)
    #endif ()
    #if (WITH_LINUX_PLUGIN_NETWORK)
    #    target_link_libraries(lib-mqtt-sn-gateway
    #            lib-mqtt-sn-linux-plugin-client-network)
    #endif ()

    set(MQTT_SN_GATEWAY_LINUX_STARTER_FILES
            gateway/starter/linux/gateway_starter.h
            gateway/starter/linux/gateway_starter.c
            network/shared/ip/IpHelper.c
            network/shared/ip/IpHelper.h)
    if (WITH_LOGGING)
        set(MQTT_SN_GATEWAY_LINUX_STARTER_FILES ${MQTT_SN_GATEWAY_LINUX_STARTER_FILES}
                network/shared/ip/IpHelperLogging.c
                network/shared/ip/IpHelperLogging.h)
    endif ()
    add_library(lib-mqtt-sn-linux-gateway-starter ${MQTT_SN_GATEWAY_LINUX_STARTER_FILES})
    target_include_directories(lib-mqtt-sn-linux-gateway-starter PRIVATE .)
    target_compile_options(lib-mqtt-sn-linux-gateway-starter PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(lib-mqtt-sn-linux-gateway-starter lib-mqtt-sn-gateway lib-mqtt-sn-gateway-config pthread)


    option(WITH_MQTT_SN_ECHOGATEWAY "Include WITH_MQTT_SN_ECHOGATEWAY support?" ON)
    if (WITH_MQTT_SN_ECHOGATEWAY)
        add_library(lib-mqtt-sn-echogateway
                echogateway/echogateway.c echogateway/echogateway.h
                echogateway/config/echogateway_config.c echogateway/config/echogateway_config.h
                parser/MqttSnMessageParser.c parser/MqttSnMessageParser.h)
        target_include_directories(lib-mqtt-sn-echogateway PRIVATE .)
        target_compile_options(lib-mqtt-sn-echogateway PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
        target_link_libraries(lib-mqtt-sn-echogateway lib-mqtt-sn-message-parser lib-mqtt-sn-linux-client-network lib-mqtt-sn-platform-abstraction-layer pthread)
        if (WITH_LOGGING)
            target_link_libraries(lib-mqtt-sn-echogateway lib-mqtt-sn-logging)
        endif ()
        if (WITH_LINUX_PLUGIN_NETWORK)
            target_link_libraries(lib-mqtt-sn-echogateway lib-mqtt-sn-linux-plugin-client-network)
        endif ()

        add_executable(MqttSnEchoGateway echogateway/linux/main.c
                parser/MqttSnMessageParser.c parser/MqttSnMessageParser.h
                echogateway/linux/starter/echogateway_starter.c echogateway/linux/starter/echogateway_starter.h)
        target_include_directories(MqttSnEchoGateway PRIVATE .)
        target_compile_options(MqttSnEchoGateway PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
        target_link_libraries(MqttSnEchoGateway lib-mqtt-sn-echogateway lib-mqtt-sn-gateway-config pthread)
    endif ()

endif ()

#set(MQTT_SN_CLIENT_SUB_FILES client/sub/linux/main.c ${MQTT_SN_CLIENT_TEST_FILES} parser/MqttSnMessageParser.c parser/MqttSnMessageParser.h
#client/sub/config/subscription_client_config.c client/sub/config/subscription_client_config.h)
#add_executable(MqttSnClientSub ${MQTT_SN_CLIENT_SUB_FILES})
#target_include_directories(MqttSnClientSub PRIVATE .)
#target_compile_options(MqttSnClientSub PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
#target_link_libraries(MqttSnClientSub
#        lib-mqtt-sn-ring-buffer
#        lib-mqtt-sn-message-parser
#        lib-mqtt-sn-logging
#        lib-mqtt-sn-linux-client-network
#        lib-mqtt-sn-linux-gateway-network
#        pthread dl)


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


option(WITH_MQTT_SN_PARSER_TESTING "Include WITH_MQTT_SN_PARSER_TESTING support?" ON)
if (WITH_MQTT_SN_PARSER_TESTING)
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

    add_executable(mqtt-sn-message-parser-tests test/parser/MqttSnParserEncapsulationMessageParseTests.cpp test/parser/MqttSnParserEncapsulationMessageParseTests.h)
    target_include_directories(mqtt-sn-message-parser-tests PRIVATE .)
    target_link_libraries(mqtt-sn-message-parser-tests gtest gtest_main gmock lib-mqtt-sn-message-parser)


    add_executable(MqttSnMultipleForwarderTests test/MqttSnMultipleForwarderTest/MqttSnMultipleForwarderTests.cpp test/MqttSnMultipleForwarderTest/MqttSnMultipleForwarderTests.h)
    target_include_directories(MqttSnMultipleForwarderTests PRIVATE .)
    target_link_libraries(MqttSnMultipleForwarderTests gtest gtest_main gmock LibMqttSnClientTestContainer LibMqttSnForwarderTestContainer LibMqttSnGatewayTestContainer)

    add_library(LibMqttBrokerTestContainer test/MqttBroker/MqttBrokerTestContainerInterface.hpp test/MqttBroker/MqttBrokerMosquittoDockerTestContainer.cpp test/MqttBroker/MqttBrokerMosquittoDockerTestContainer.h)
    target_include_directories(LibMqttBrokerTestContainer PRIVATE .)
    target_compile_options(LibMqttBrokerTestContainer PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(LibMqttBrokerTestContainer pthread)

    add_library(LibMqttSnGatewayTestContainer test/MqttSnGateway/MqttSnGatewayTestContainerInterface.h
            test/MqttSnGateway/MqttSnGatewayTestType.cpp test/MqttSnGateway/MqttSnGatewayTestType.h
            test/MqttSnGateway/MqttSnGatewayTestContainerFactory.cpp test/MqttSnGateway/MqttSnGatewayTestContainerInterface.h
            test/MqttSnGateway/MqttSnGatewayArsmbDirectTestContainer.cpp test/MqttSnGateway/MqttSnGatewayArsmbDirectTestContainer.h)
    target_include_directories(LibMqttSnGatewayTestContainer PRIVATE .)
    target_compile_options(LibMqttSnGatewayTestContainer PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(LibMqttSnGatewayTestContainer lib-mqtt-sn-linux-gateway-starter lib-mqtt-sn-gateway-config lib-mqtt-sn-gateway lib-mqtt-sn-linux-client-network)
    if (WITH_LINUX_PLUGIN_NETWORK)
        target_link_libraries(LibMqttSnGatewayTestContainer lib-mqtt-sn-linux-plugin-client-network)
    endif ()

    add_library(LibMqttSnForwarderTestContainer test/MqttSnForwarder/MqttSnForwarderTestContainer.cpp test/MqttSnForwarder/MqttSnForwarderTestContainer.h)
    target_include_directories(LibMqttSnForwarderTestContainer PRIVATE .)
    target_compile_options(LibMqttSnForwarderTestContainer PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(LibMqttSnForwarderTestContainer lib-mqtt-sn-forwarder lib-mqtt-sn-forwarder-config lib-mqtt-sn-linux-forwarder-starter lib-mqtt-sn-platform-abstraction-layer)

    add_library(LibMqttSnClientTestContainer test/MqttSnClient/MqttSnClientTestContainer.cpp test/MqttSnClient/MqttSnClientTestContainer.h
            client/pub/linux/starter/publish_client_starter.c client/pub/linux/starter/publish_client_starter.h
            parser/MqttSnMessageParser.c parser/MqttSnMessageParser.h)
    target_include_directories(LibMqttSnClientTestContainer PRIVATE .)
    target_compile_options(LibMqttSnClientTestContainer PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(LibMqttSnClientTestContainer lib-mqtt-sn-client lib-mqtt-sn-client-config lib-mqtt-sn-gateway-network-config lib-mqtt-sn-platform-abstraction-layer)

    add_library(LibMqttSnMessageTester
            test/MqttSnMessageTester/MqttSnMessageTester.hpp test/MqttSnMessageTester/MqttSnMessageTester.cpp
            test/MqttSnMessageTester/MqttSnTestMessageSender.hpp test/MqttSnMessageTester/MqttSnTestMessageSender.cpp
            test/MqttSnMessageTester/MqttSnTestMessages.hpp test/MqttSnMessageTester/MqttSnTestMessageReceiverInterface.hpp
            test/MqttSnMessageTester/MqttSnTestMessageReceiverMock.hpp
            test/MqttSnMessageTester/NetworkContextTestContainer.cpp test/MqttSnMessageTester/NetworkContextTestContainer.h)
    target_include_directories(LibMqttSnMessageTester PRIVATE .)
    #target_compile_options(LibMqttSnMessageTester PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(LibMqttSnMessageTester lib-mqtt-sn-linux-client-network lib-mqtt-sn-client-network-config lib-mqtt-sn-linux-gateway-network lib-mqtt-sn-gateway-network-config lib-mqtt-sn-ring-buffer)
    target_link_libraries(LibMqttSnMessageTester gtest gmock pthread lib-mqtt-sn-forwarder lib-mqtt-sn-forwarder-config lib-mqtt-sn-linux-forwarder-starter)
    if (WITH_LINUX_PLUGIN_NETWORK)
        target_link_libraries(LibMqttSnMessageTester lib-mqtt-sn-linux-plugin-client-network)
    endif ()


    add_executable(runMqttSnGatewayArsmbDirectConnectTests
            test/MqttSnGateway/compliance/MqttSnGatewayComplianceConnectTest.cpp
            test/MqttSnGateway/compliance/MqttSnGatewayComplianceConnectTest.h)
    target_include_directories(runMqttSnGatewayArsmbDirectConnectTests PRIVATE .)
    target_compile_options(runMqttSnGatewayArsmbDirectConnectTests PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(runMqttSnGatewayArsmbDirectConnectTests gtest gtest_main gmock pthread LibMqttSnGatewayTestContainer LibMqttSnMessageTester LibMqttBrokerTestContainer)

    #Eclipse Paho stuff
    set(PAHO_PROJECT_PATH external/paho.mqtt-sn-embedded-c-c9e807da319bd68bff71cc4817ea5a4f4b25f49d)

    ## build settings
    SET(PAHO_VERSION_MAJOR 1)
    SET(PAHO_VERSION_MINOR 0)
    SET(PAHO_VERSION_PATCH 0)
    SET(CLIENT_VERSION ${PAHO_VERSION_MAJOR}.${PAHO_VERSION_MINOR}.${PAHO_VERSION_PATCH})

    set(PAHO_PACKET_SAMPLE_PATH external/paho.mqtt-sn-embedded-c-c9e807da319bd68bff71cc4817ea5a4f4b25f49d/MQTTSNPacket/samples)
    set(PAHO_PACKET_SRC_PATH external/paho.mqtt-sn-embedded-c-c9e807da319bd68bff71cc4817ea5a4f4b25f49d/MQTTSNPacket/src)
    set(PAHO_PACKET_TEST_PATH external/paho.mqtt-sn-embedded-c-c9e807da319bd68bff71cc4817ea5a4f4b25f49d/MQTTSNPacket/test)

    ADD_LIBRARY(LibPahotest1 ${PAHO_PACKET_TEST_PATH}/test1.c)
    TARGET_LINK_LIBRARIES(LibPahotest1 LibPahoMQTTSNPacketClient LibPahoMQTTSNPacketServer)
    target_include_directories(LibPahotest1 PRIVATE ${PAHO_PACKET_SRC_PATH})

    #add_executable(PPahotest1 ${PAHO_PACKET_TEST_PATH}/test1.c)
    #target_include_directories(PPahotest1 PRIVATE ${PAHO_PACKET_SRC_PATH})
    #target_link_libraries(PPahotest1 LibPahotest1)

    ADD_LIBRARY(LibPahotest2 ${PAHO_PACKET_TEST_PATH}/test2.c)
    TARGET_LINK_LIBRARIES(LibPahotest2 LibPahoMQTTSNPacketClient LibPahoMQTTSNPacketServer)
    target_include_directories(LibPahotest2 PRIVATE ${PAHO_PACKET_SRC_PATH})

    ADD_LIBRARY(LibPahoMQTTSNPacketClient SHARED ${PAHO_PACKET_SRC_PATH}/MQTTSNConnectClient.c ${PAHO_PACKET_SRC_PATH}/MQTTSNPacket.c ${PAHO_PACKET_SRC_PATH}/MQTTSNSearchClient.c ${PAHO_PACKET_SRC_PATH}/MQTTSNSubscribeClient.c
            ${PAHO_PACKET_SRC_PATH}/MQTTSNUnsubscribeClient.c ${PAHO_PACKET_SRC_PATH}/MQTTSNSerializePublish.c ${PAHO_PACKET_SRC_PATH}/MQTTSNDeserializePublish.c)

    ADD_LIBRARY(LibPahoMQTTSNPacketServer SHARED ${PAHO_PACKET_SRC_PATH}/MQTTSNConnectServer.c ${PAHO_PACKET_SRC_PATH}/MQTTSNPacket.c ${PAHO_PACKET_SRC_PATH}/MQTTSNSearchServer.c ${PAHO_PACKET_SRC_PATH}/MQTTSNSubscribeServer.c
            ${PAHO_PACKET_SRC_PATH}/MQTTSNUnsubscribeServer.c ${PAHO_PACKET_SRC_PATH}/MQTTSNSerializePublish.c ${PAHO_PACKET_SRC_PATH}/MQTTSNDeserializePublish.c)

    add_executable(Pahotest1 testclients/pahotest1main.c)
    target_link_libraries(Pahotest1 LibPahotest1)

    ADD_EXECUTABLE(LibPahoqos-1pub ${PAHO_PACKET_SAMPLE_PATH}/qos-1pub.c ${PAHO_PACKET_SAMPLE_PATH}/transport.c)
    target_include_directories(LibPahoqos-1pub PRIVATE ${PAHO_PACKET_SRC_PATH})
    TARGET_LINK_LIBRARIES(LibPahoqos-1pub LibPahoMQTTSNPacketClient)

endif (WITH_MQTT_SN_PARSER_TESTING)

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
                    utility/linux/wifi_fake/WiFi.cpp
                    utility/linux/wifi_fake/WiFi.h)
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
                config/common/arduino/eeprom/eeprom_config.h
                config/common/arduino/eeprom/eeprom_config.c)
        if (WITH_LOGGING)
            set(MQTT_SN_ARDUINO_ESP8266_EEPROM_CONFIG_LINE_LOADER_FILES ${MQTT_SN_ARDUINO_ESP8266_EEPROM_CONFIG_LINE_LOADER_FILES}
                    config/common/arduino/eeprom/eeprom_config_logging.c
                    config/common/arduino/eeprom/eeprom_config_logging.h)
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
                config/common/arduino/eeprom/eeprom_config.h
                config/common/arduino/eeprom/eeprom_config.c
                config/common/arduino/eeprom/forwarder/forwarder_eeprom_loader.c
                config/common/arduino/eeprom/forwarder/forwarder_eeprom_loader.h)
        if (WITH_LOGGING)
            set(MQTT_SN_ARDUINO_ESP8266_EEPROM_FORWADER_CONFIG_LINE_LOADER_FILES ${MQTT_SN_ARDUINO_ESP8266_EEPROM_FORWADER_CONFIG_LINE_LOADER_FILES}
                    config/common/arduino/eeprom/forwarder/forwarder_eeprom_loader_logging.c
                    config/common/arduino/eeprom/forwarder/forwarder_eeprom_loader_logging.h)
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
                config/common/arduino/system/system.c
                config/common/arduino/system/system.h
                network/arduino/shared/ip/ArduinoIpAddressHelper.cpp
                network/arduino/shared/ip/ArduinoIpAddressHelper.hpp
                config/common/arduino/eeprom/forwarder/forwarder_eeprom_loader.c
                config/common/arduino/eeprom/forwarder/forwarder_eeprom_loader.h)
        if (WITH_LOGGING)
            set(MQTT_SN_ARDUINO_ESP8266_SYSTEM_FILES ${MQTT_SN_ARDUINO_ESP8266_SYSTEM_FILES}
                    config/common/arduino/system/system_logging.c
                    config/common/arduino/system/system_logging.h)
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

        #if (WITH_LOGGING)
        #    set(MQTT_SN_ARDUINO_SSD1306_SCREEN_FILES
        #            logging/arduino/SSD1306/SSD1306Logger.cpp
        #            logging/arduino/SSD1306/SSD1306Logger.hpp)
        #    add_library(lib-mqtt-sn-arduino-ssd1306-screen-logger ${MQTT_SN_ARDUINO_SSD1306_SCREEN_FILES})
        #    target_include_directories(lib-mqtt-sn-arduino-ssd1306-screen-logger PRIVATE .)
        #endif ()

    endif ()
endif ()
