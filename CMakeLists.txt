cmake_minimum_required(VERSION 3.10)
project(CMqttSnForwarder C CXX)

if (BUILD_TESTING)
    project(CMqttSnForwarder C CXX)
else ()
    project(CMqttSnForwarder C)
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
endif (BUILD_TESTING)

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

option(WITH_CONFIG_FILE "Include CONFIG_FILE support?" ON)
if (WITH_CONFIG_FILE)
    message("Building with CONFIG_FILE support")
    add_definitions(-DWITH_CONFIG_FILE)
endif ()


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
            forwarder/MqttSnForwarderLogging.h
            forwarder/MqttSnForwarderLogging.c
            forwarder/logging/MqttSnForwarderLoggingBasic.c
            forwarder/logging/MqttSnForwarderLoggingBasic.h
            forwarder/logging/MqttSnForwarderLoggingMessages.c
            forwarder/logging/MqttSnForwarderLoggingMessages.h)

    set(STDOUT_LOGGING_FILES
            forwarder/logging/linux/stdout/StdoutLogging.c
            forwarder/logging/linux/stdout/StdoutLogging.h)
    set(STDERR_LOGGING_FILES
            forwarder/logging/linux/stderr/StderrLogging.c
            forwarder/logging/linux/stderr/StderrLogging.h)
    #option(WITH_MQTT_SN_MESSAGE_LOGGING "Include MQTT_SN_MESSAGE_LOGGING support?" ON)
    #if (WITH_MQTT_SN_MESSAGE_LOGGING)
    #    set(MQTT_SN_LOGGING_FILES )
    #endif ()
    set(LOGGING_FILES ${CORE_LOGGING_FILES} ${MQTT_SN_LOGGING_FILES} ${STDOUT_LOGGING_FILES} ${STDERR_LOGGING_FILES})

    add_library(lib-mqtt-sn-logging ${LOGGING_FILES})
    target_include_directories(lib-mqtt-sn-logging PRIVATE .)
    target_compile_options(lib-mqtt-sn-logging PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
endif ()

option(WITH_PLUGIN "Include PLUGIN support?" ON)
if (WITH_PLUGIN)
    add_definitions(-DWITH_PLUGIN)
    message("Building with PLUGIN support")
    set(PLUGIN_FILES
            forwarder/network/linux/client/plugin/client_network_plugin_interface.c
            forwarder/network/linux/client/plugin/client_network_plugin_interface.h
            forwarder/network/linux/gateway/plugin/gateway_network_plugin_interface.c
            forwarder/network/linux/gateway/plugin/gateway_network_plugin_interface.h

            forwarder/network/linux/gateway/plugin/MqttSnGatewayPluginNetwork.h
            forwarder/network/linux/gateway/plugin/MqttSnGatewayPluginNetwork.c
            forwarder/network/linux/client/plugin/MqttSnClientPluginNetwork.h
            forwarder/network/linux/client/plugin/MqttSnClientPluginNetwork.c)

    if (WITH_LOGGING)
        set(PLUGIN_FILES ${PLUGIN_FILES}
                forwarder/network/linux/shared/plugin/MqttSnPluginLogger.h
                forwarder/network/linux/shared/plugin/MqttSnPluginLogger.c)
    endif ()

endif ()

option(WITH_LINUX_TCP "Include LINUX_TCP support?" ON)
if (WITH_LINUX_TCP)
    add_definitions(-DWITH_TCP)
    message("Building with LINUX_TCP support")
    option(WITH_TCP_BROADCAST "Include TCP_BROADCAST support?" ON)
    if (WITH_TCP_BROADCAST)
        add_definitions(-DWITH_TCP_BROADCAST)
    endif ()
    add_definitions(-DWITH_LINUX_GATEWAY_NETWORK_TCP)
    add_definitions(-DWITH_LINUX_CLIENT_NETWORK_TCP)
    set(TCP_GATWAY_NETWORK_FILES
            forwarder/network/linux/gateway/ip/tcp/MqttSnGatewayTcpNetwork.h
            forwarder/network/linux/gateway/ip/tcp/MqttSnGatewayTcpNetwork.c
            forwarder/network/linux/shared/ip/tcphelper/MqttSnTcpNetworkMessageParser.c
            forwarder/network/linux/shared/ip/tcphelper/MqttSnTcpNetworkMessageParser.h
            forwarder/network/shared/ip/IpHelper.c
            forwarder/network/shared/ip/IpHelper.h
            forwarder/network/linux/shared/ip/MqttSnIpNetworkHelper.h
            forwarder/network/linux/shared/ip/MqttSnIpNetworkHelper.c
            forwarder/network/linux/shared/ip/tcphelper/MqttSnTcpNetworkHelper.c
            forwarder/network/linux/shared/ip/tcphelper/MqttSnTcpNetworkHelper.h)
    if (WITH_TCP_BROADCAST)
        set(GATEWAY_NETWORK_FILES ${GATEWAY_NETWORK_FILES}
                forwarder/network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.c
                forwarder/network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.h)
    endif ()
    set(GATEWAY_NETWORK_FILES ${GATEWAY_NETWORK_FILES} ${TCP_GATWAY_NETWORK_FILES})

    set(TCP_CLIENT_NETWORK_FILES
            forwarder/network/linux/client/ip/tcp/MqttSnClientTcpNetwork.h
            forwarder/network/linux/client/ip/tcp/MqttSnClientTcpNetwork.c
            forwarder/network/linux/shared/ip/tcphelper/MqttSnTcpNetworkMessageParser.c
            forwarder/network/linux/shared/ip/tcphelper/MqttSnTcpNetworkMessageParser.h
            forwarder/network/shared/ip/IpHelper.c
            forwarder/network/shared/ip/IpHelper.h
            forwarder/network/linux/shared/ip/MqttSnIpNetworkHelper.h
            forwarder/network/linux/shared/ip/MqttSnIpNetworkHelper.c
            forwarder/network/linux/shared/ip/tcphelper/MqttSnTcpNetworkHelper.c
            forwarder/network/linux/shared/ip/tcphelper/MqttSnTcpNetworkHelper.h)
    if (WITH_TCP_BROADCAST)
        set(TCP_CLIENT_NETWORK_FILES ${TCP_CLIENT_NETWORK_FILES}
                forwarder/network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.c
                forwarder/network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.h)
    endif ()
    set(CLIENT_NETWORK_FILES ${CLIENT_NETWORK_FILES} ${TCP_CLIENT_NETWORK_FILES})

endif ()


option(WITH_LINUX_UDP "Include LINUX_UDP support?" ON)
if (WITH_LINUX_UDP)
    add_definitions(-DWITH_UDP)
    message("Building with LINUX_UDP support")
    option(WITH_UDP_BROADCAST "Include UDP_BROADCAST support?" ON)
    if (WITH_UDP_BROADCAST)
        add_definitions(-DWITH_UDP_BROADCAST)
        add_definitions(-DWITH_UDP_BROADCAST_GATEWAY)
        add_definitions(-DWITH_UDP_BROADCAST_CLIENT)
    endif ()
    add_definitions(-DWITH_LINUX_GATEWAY_NETWORK_UDP)
    add_definitions(-DWITH_LINUX_CLIENT_NETWORK_UDP)
    set(UDP_GATWAY_NETWORK_FILES
            forwarder/network/linux/gateway/ip/udp/MqttSnGatewayUdpNetwork.h
            forwarder/network/linux/gateway/ip/udp/MqttSnGatewayUdpNetwork.c
            forwarder/network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.h
            forwarder/network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.c
            forwarder/network/shared/ip/IpHelper.h
            forwarder/network/shared/ip/IpHelper.c
            forwarder/network/linux/shared/ip/MqttSnIpNetworkHelper.h
            forwarder/network/linux/shared/ip/MqttSnIpNetworkHelper.c)
    if (WITH_UDP_BROADCAST)
        set(UDP_GATWAY_NETWORK_FILES ${UDP_GATWAY_NETWORK_FILES}
                forwarder/network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.c
                forwarder/network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.h)
    endif ()
    set(GATEWAY_NETWORK_FILES ${GATEWAY_NETWORK_FILES} ${UDP_GATWAY_NETWORK_FILES})

    set(UDP_CLIENT_NETWORK_FILES
            forwarder/network/linux/client/ip/udp/MqttSnClientUdpNetwork.c
            forwarder/network/linux/client/ip/udp/MqttSnClientUdpNetwork.h
            forwarder/network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.h
            forwarder/network/linux/shared/ip/udphelper/MqttSnUdpNetworkMessageParser.c
            forwarder/network/shared/ip/IpHelper.h
            forwarder/network/shared/ip/IpHelper.c
            forwarder/network/linux/shared/ip/MqttSnIpNetworkHelper.h
            forwarder/network/linux/shared/ip/MqttSnIpNetworkHelper.c)
    if (WITH_UDP_BROADCAST)
        set(UDP_CLIENT_NETWORK_FILES ${UDP_CLIENT_NETWORK_FILES}
                forwarder/network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.c
                forwarder/network/linux/shared/ip/multicasthelper/MqttSnUdpMulticastMessageParser.h)
    endif ()
    set(CLIENT_NETWORK_FILES ${CLIENT_NETWORK_FILES} ${UDP_CLIENT_NETWORK_FILES})
endif ()


option(WITH_HIREDIS_PLUGIN "Include HIREDIS_PLUGIN support?" OFF)
if (WITH_HIREDIS_PLUGIN)
    add_subdirectory(plugins)
endif ()


add_library(lib-mqtt-sn-gateway-network ${GATEWAY_NETWORK_FILES})
target_include_directories(lib-mqtt-sn-gateway-network PRIVATE .)
target_compile_options(lib-mqtt-sn-gateway-network PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(lib-mqtt-sn-gateway-network lib-mqtt-sn-ring-buffer)

add_library(lib-mqtt-sn-client-network ${CLIENT_NETWORK_FILES})
target_include_directories(lib-mqtt-sn-client-network PRIVATE .)
target_compile_options(lib-mqtt-sn-client-network PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(lib-mqtt-sn-client-network lib-mqtt-sn-ring-buffer)

set(MQTT_SN_FIXED_SIZE_RING_BUFFER_FILES
        forwarder/MqttSnFixedSizeRingBuffer.h
        forwarder/MqttSnFixedSizeRingBuffer.c)

add_library(lib-mqtt-sn-ring-buffer ${MQTT_SN_FIXED_SIZE_RING_BUFFER_FILES})
target_include_directories(lib-mqtt-sn-ring-buffer PRIVATE forwarder)
target_compile_options(lib-mqtt-sn-ring-buffer PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})

set(PARSER_FILES
        forwarder/MqttSnMessageParser.h
        forwarder/MqttSnMessageParser.c)
add_library(lib-mqtt-sn-msg-parser ${PARSER_FILES})
target_include_directories(lib-mqtt-sn-msg-parser PRIVATE .)
target_compile_options(lib-mqtt-sn-msg-parser PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})

set(FORWARDER_FILES
        forwarder/MqttSnClientNetworkInterface.h
        forwarder/MqttSnForwarder.h
        forwarder/MqttSnGatewayNetworkInterface.h
        forwarder/MqttSnForwarder.c
        forwarder/global_defines.h
        forwarder/MqttSnGatewayNetworkInterface.c
        forwarder/MqttSnClientNetworkInterface.c
        ${PLUGIN_FILES})

add_library(lib-mqtt-sn-forwarder ${FORWARDER_FILES})
target_include_directories(lib-mqtt-sn-forwarder PRIVATE .)
target_compile_options(lib-mqtt-sn-forwarder PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(lib-mqtt-sn-forwarder lib-mqtt-sn-msg-parser)
if (WITH_PLUGIN)
    target_link_libraries(lib-mqtt-sn-forwarder dl)
endif ()

if (NOT BUILD_TESTING)
    message("Bulding as C project")

    add_executable(CMqttSnForwarder
            main/linux/main.c
            main/linux/forwarder_starter.h
            main/linux/forwarder_starter.c
            main/forwarder_config.h
            main/forwarder_config.c)
    target_include_directories(CMqttSnForwarder PRIVATE .)
    target_compile_options(CMqttSnForwarder PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(CMqttSnForwarder
            pthread
            lib-mqtt-sn-forwarder lib-mqtt-sn-gateway-network lib-mqtt-sn-client-network)
    if (WITH_LOGGING)
        target_link_libraries(CMqttSnForwarder lib-mqtt-sn-logging)
    endif ()


    add_executable(TcpCMqttSnForwarderTester testclients/tcptestermain.c forwarder/global_defines.h)
    target_link_libraries(TcpCMqttSnForwarderTester pthread)

    add_executable(UdpCMqttSnForwarderTester testclients/udptestermain.c forwarder/global_defines.h)
    target_link_libraries(UdpCMqttSnForwarderTester pthread)

    add_executable(UdpGatewayMulticastTester testclients/udpmulticastmain.c)
    target_compile_definitions(UdpGatewayMulticastTester PRIVATE CLIENT_PORT=11110 MQTT_SN_MULTICAST_IP="224.1.1.100" MQTT_SN_MULTICAST_PORT=5353)
    target_include_directories(UdpGatewayMulticastTester PRIVATE .)
    target_link_libraries(UdpGatewayMulticastTester lib-mqtt-sn-forwarder lib-mqtt-sn-gateway-network lib-mqtt-sn-client-network)
    if (WITH_LOGGING)
        target_link_libraries(UdpGatewayMulticastTester lib-mqtt-sn-logging)
    endif ()


    add_executable(UdpClientMulticastTester testclients/udpmulticastmain.c)
    target_compile_definitions(UdpClientMulticastTester PRIVATE CLIENT_PORT=11111 MQTT_SN_MULTICAST_IP="224.1.1.101" MQTT_SN_MULTICAST_PORT=5352)
    target_include_directories(UdpClientMulticastTester PRIVATE .)
    target_link_libraries(UdpClientMulticastTester lib-mqtt-sn-forwarder lib-mqtt-sn-gateway-network lib-mqtt-sn-client-network)
    if (WITH_LOGGING)
        target_link_libraries(UdpClientMulticastTester lib-mqtt-sn-logging)
    endif ()

    # This target only exists for Clion IDE support for the following files:
    add_executable(ArduinoCMqttSnForwarder main/forwarder_config.h main/forwarder_config.c forwarder/network/arduino/shared/ip/udp/UdpHelper.c forwarder/network/arduino/shared/ip/udp/UdpHelper.h forwarder/network/arduino/client/ip/MqttSnClientUdpNetwork.c forwarder/network/arduino/client/ip/MqttSnClientUdpNetwork.h forwarder/network/arduino/gateway/ip/MqttSnGatewayUdpNetwork.c forwarder/network/arduino/gateway/ip/MqttSnGatewayUdpNetwork.h forwarder/network/arduino/shared/ip/ArduinoIpAddressHelper.c forwarder/network/arduino/shared/ip/ArduinoIpAddressHelper.h utility/arduino/eeprom/eeprom_config.c utility/arduino/eeprom/eeprom_config.h utility/arduino/system/system.c utility/arduino/system/system.h)
    target_include_directories(ArduinoCMqttSnForwarder PRIVATE .)
    target_compile_options(ArduinoCMqttSnForwarder PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(ArduinoCMqttSnForwarder
            lib-mqtt-sn-forwarder lib-mqtt-sn-gateway-network lib-mqtt-sn-client-network)
    if (WITH_LOGGING)
        target_link_libraries(ArduinoCMqttSnForwarder lib-mqtt-sn-logging)
    endif ()


else ()
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
            forwarder/MqttSnClientNetworkInterface.c
            forwarder/MqttSnClientNetworkInterface.h
            test/MqttSnClientNetworkInterface/MqttSnClientNetworkInterfaceTests.cpp
            test/MqttSnClientNetworkInterface/MqttSnClientNetworkInterfaceTests.h)
    target_include_directories(mqtt-sn-client-network-interface-test PRIVATE .)
    target_link_libraries(mqtt-sn-client-network-interface-test gtest gtest_main gmock mock-client-network)

    add_executable(mqtt-sn-gateway-network-interface-test
            forwarder/MqttSnGatewayNetworkInterface.c
            forwarder/MqttSnGatewayNetworkInterface.h
            test/MqttSnGatewayNetworkInterface/MqttSnGatewayNetworkInterfaceTests.cpp
            test/MqttSnGatewayNetworkInterface/MqttSnGatewayNetworkInterfaceTests.h)
    target_include_directories(mqtt-sn-gateway-network-interface-test PRIVATE .)
    target_link_libraries(mqtt-sn-gateway-network-interface-test gtest gtest_main gmock mock-gateway-network)

    add_executable(mqtt-sn-fixed-size-ring-buffer-test
            forwarder/MqttSnFixedSizeRingBuffer.c
            forwarder/MqttSnFixedSizeRingBuffer.c
            test/MqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferTests.cpp
            test/MqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferTests.h)
    target_include_directories(mqtt-sn-fixed-size-ring-buffer-test PRIVATE .)
    target_link_libraries(mqtt-sn-fixed-size-ring-buffer-test gtest gtest_main lib-mqtt-sn-ring-buffer)

    add_executable(mqtt-sn-forwarder-test
            forwarder/MqttSnForwarder.c
            forwarder/MqttSnForwarder.h
            forwarder/MqttSnClientNetworkInterface.c
            forwarder/MqttSnClientNetworkInterface.h
            forwarder/MqttSnGatewayNetworkInterface.c
            forwarder/MqttSnGatewayNetworkInterface.h
            forwarder/MqttSnMessageParser.c
            forwarder/MqttSnMessageParser.h

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
