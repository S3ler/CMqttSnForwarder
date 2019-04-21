cmake_minimum_required(VERSION 3.10)
project(CMqttSnForwarder C CXX)

if (BUILD_TESTING)
    project(CMqttSnForwarder C CXX)
else ()
    project(CMqttSnForwarder C)
    set(VERSION a1)
    set(MAJOR 0)
    set(MINOR 0)
    set(TWEAK 10)
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

option(WITH_LOGGING "Include LOGGING support?" ON)
if (WITH_LOGGING)
    message("Building with LOGGING support")
    add_definitions(-DWITH_LOGGING)
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
    #option(WITH_MQTT_SN_MESSAGE_LOGGING "Include MQTT_SN_MESSAGE_LOGGING support?" ON)
    #if (WITH_MQTT_SN_MESSAGE_LOGGING)
    #    set(MQTT_SN_LOGGING_FILES )
    #endif ()
    set(LOGGING_FILES ${CORE_LOGGING_FILES} ${MQTT_SN_LOGGING_FILES} ${STDOUT_LOGGING_FILES})

    add_library(lib-mqtt-sn-logging ${LOGGING_FILES})
    target_compile_options(lib-mqtt-sn-logging PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
endif ()

option(WITH_PLUGIN "Include PLUGIN support?" ON)
if (WITH_PLUGIN)
    message("Building with PLUGIN support")
    add_definitions(-DWITH_PLUGIN)
    set(PLUGIN_FILES
            forwarder/network/client/plugin/client_network_plugin_interface.h
            forwarder/network/gateway/plugin/gateway_network_plugin_interface.h

            forwarder/network/gateway/plugin/MqttSnGatewayPluginNetwork.h
            forwarder/network/gateway/plugin/MqttSnGatewayPluginNetwork.c
            forwarder/network/client/plugin/MqttSnClientPluginNetwork.h
            forwarder/network/client/plugin/MqttSnClientPluginNetwork.c)

    if (WITH_LOGGING)
        set(PLUGIN_FILES ${PLUGIN_FILES}
                forwarder/network/plugin/MqttSnPluginLogger.h
                forwarder/network/plugin/MqttSnPluginLogger.c)
    endif ()

endif ()

option(WITH_LINUX_TCP "Include LINUX_TCP support?" ON)
if (WITH_LINUX_TCP)
    message("Building with LINUX_TCP support")
    add_definitions(-DWITH_LINUX_GATEWAY_NETWORK_TCP)
    add_definitions(-DWITH_LINUX_CLIENT_NETWORK_TCP)

    set(TCP_GATWAY_NETWORK_FILES
            forwarder/network/gateway/tcp/MqttSnGatewayTcpNetwork.h
            forwarder/network/gateway/tcp/MqttSnGatewayTcpNetwork.c
            forwarder/network/tcphelper/MqttSnTcpNetworkMessageParser.c
            forwarder/network/tcphelper/MqttSnTcpNetworkMessageParser.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.c)
    set(GATEWAY_NETWORK_FILES ${GATEWAY_NETWORK_FILES} ${TCP_GATWAY_NETWORK_FILES})

    set(TCP_CLIENT_NETWORK_FILES
            forwarder/network/client/tcp/MqttSnClientTcpNetwork.h
            forwarder/network/client/tcp/MqttSnClientTcpNetwork.c
            forwarder/network/tcphelper/MqttSnTcpNetworkMessageParser.c
            forwarder/network/tcphelper/MqttSnTcpNetworkMessageParser.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.c)
    set(CLIENT_NETWORK_FILES ${CLIENT_NETWORK_FILES} ${TCP_CLIENT_NETWORK_FILES})

endif ()


option(WITH_LINUX_UDP "Include LINUX_UDP support?" ON)
if (WITH_LINUX_UDP)
    message("Building with LINUX_UDP support")
    add_definitions(-DWITH_LINUX_GATEWAY_NETWORK_UDP)
    add_definitions(-DWITH_LINUX_CLIENT_NETWORK_UDP)

    set(UDP_GATWAY_NETWORK_FILES
            forwarder/network/gateway/udp/MqttSnGatewayUdpNetwork.h
            forwarder/network/gateway/udp/MqttSnGatewayUdpNetwork.c
            forwarder/network/udphelper/MqttSnUdpNetworkMessageParser.h
            forwarder/network/udphelper/MqttSnUdpNetworkMessageParser.c
            forwarder/network/iphelper/MqttSnIpNetworkHelper.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.c)
    set(GATEWAY_NETWORK_FILES ${GATEWAY_NETWORK_FILES} ${UDP_GATWAY_NETWORK_FILES})

    set(UDP_CLIENT_NETWORK_FILES
            forwarder/network/client/udp/MqttSnClientUdpNetwork.c
            forwarder/network/client/udp/MqttSnClientUdpNetwork.h
            forwarder/network/udphelper/MqttSnUdpNetworkMessageParser.h
            forwarder/network/udphelper/MqttSnUdpNetworkMessageParser.c
            forwarder/network/iphelper/MqttSnIpNetworkHelper.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.c)
    set(CLIENT_NETWORK_FILES ${CLIENT_NETWORK_FILES} ${UDP_CLIENT_NETWORK_FILES})
endif ()


option(WITH_HIREDIS_PLUGIN "Include HIREDIS_PLUGIN support?" ON)
if (WITH_HIREDIS_PLUGIN)
    add_subdirectory(plugins)
endif ()


add_library(lib-mqtt-sn-gateway-network ${GATEWAY_NETWORK_FILES})
target_compile_options(lib-mqtt-sn-gateway-network PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(lib-mqtt-sn-gateway-network lib-mqtt-sn-ring-buffer)

add_library(lib-mqtt-sn-client-network ${CLIENT_NETWORK_FILES})
target_compile_options(lib-mqtt-sn-client-network PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(lib-mqtt-sn-client-network lib-mqtt-sn-ring-buffer)

set(MQTT_SN_FIXED_SIZE_RING_BUFFER_FILES
        forwarder/MqttSnFixedSizeRingBuffer.h
        forwarder/MqttSnFixedSizeRingBuffer.c)

add_library(lib-mqtt-sn-ring-buffer ${MQTT_SN_FIXED_SIZE_RING_BUFFER_FILES})
target_compile_options(lib-mqtt-sn-ring-buffer PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})

set(PARSER_FILES
        forwarder/MqttSnMessageParser.h
        forwarder/MqttSnMessageParser.c)
add_library(lib-mqtt-sn-msg-parser ${PARSER_FILES})
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
include_directories(forwarder)

add_library(lib-mqtt-sn-forwarder ${FORWARDER_FILES})
target_compile_options(lib-mqtt-sn-forwarder PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
target_link_libraries(lib-mqtt-sn-forwarder lib-mqtt-sn-msg-parser)
if (WITH_PLUGIN)
    target_link_libraries(lib-mqtt-sn-forwarder dl)
endif ()

if (NOT BUILD_TESTING)
    message("Bulding as C project")
    add_executable(CMqttSnForwarder
            main/main.c
            main/forwarder_starter.h
            main/forwarder_starter.c
            main/forwarder_config.h
            main/forwarder_config.c)
    target_compile_options(CMqttSnForwarder PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})
    target_link_libraries(CMqttSnForwarder
            pthread
            lib-mqtt-sn-forwarder lib-mqtt-sn-gateway-network lib-mqtt-sn-client-network)
    if (WITH_LOGGING)
        target_link_libraries(CMqttSnForwarder lib-mqtt-sn-logging)
    endif ()


    add_executable(TcpCMqttSnForwarderTester tcptestermain.c forwarder/global_defines.h)
    target_link_libraries(TcpCMqttSnForwarderTester pthread)

    add_executable(UdpCMqttSnForwarderTester udptestermain.c forwarder/global_defines.h)
    target_link_libraries(UdpCMqttSnForwarderTester pthread)
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
    add_library(lib-mqtt-sn-client-mock-network
            forwarder/global_defines.h
            test/network/client/MockClientNetwork/MockClient/ComparableClientMqttSnMessageData.h

            test/network/client/MockClientNetwork/MockClient/MockClient.cpp
            test/network/client/MockClientNetwork/MockClient/MockClient.h
            test/network/client/MockClientNetwork/MockClient/MockClientNetworkInterface.h
            test/network/client/MockClientNetwork/MockClient/MockClientNetworkReceiverInterface.h
            test/network/client/MockClientNetwork/MockClient/MockClientNetworkReceiver.h

            test/network/client/MockClientNetwork/MockForwardLooper/ClientNetworkGatewayLooper.cpp
            test/network/client/MockClientNetwork/MockForwardLooper/ClientNetworkGatewayLooper.h

            test/network/client/TestConfigurations/MqttSnClientNetworkTestValueParameter.h
            test/network/client/TestConfigurations/MqttSnGatewayClientNetworkTestConfiguration.h
            test/network/client/TestConfigurations/MockClientConfiguration.h
            test/network/client/TestConfigurations/ClientNetworkMessageDataGenerator.cpp
            test/network/client/TestConfigurations/ClientNetworkMessageDataGenerator.h

            test/network/client/TestConfigurations/CartesianProductTestCaseGenerator.cpp
            test/network/client/TestConfigurations/CartesianProductTestCaseGenerator.h
            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.cpp
            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.h)
    include_directories(test/network/client
            test/network/client/MockClientNetwork
            test/network/client/MockClientNetwork/MockClient
            test/network/client/MockClientNetwork/MockForwardLooper
            test/network/client/TestConfigurations
            test/network/client/Tests)
    target_link_libraries(lib-mqtt-sn-client-mock-network
            gtest gtest_main gmock gmock_main pthread
            lib-mqtt-sn-forwarder)

    add_executable(MqttSnForwarderLinuxTcpClientNetworkTests

            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.cpp
            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.h

            ${TCP_CLIENT_NETWORK_FILES}

            test/network/client/Tcp/MockClientLinuxTcpNetworkImplementation.cpp
            test/network/client/Tcp/MockClientLinuxTcpNetworkImplementation.h
            test/network/client/Tcp/TcpMqttSnClientNetworkInterfaceTests.cpp

            test/network/client/Tcp/GetMqttSnClientNetworkTcpNetworkDefragmentationTestParameter.cpp
            test/network/client/Tcp/GetMqttSnClientNetworkTcpNetworkDefragmentationTestParameter.h
            test/network/client/Tcp/MqttSnClientNetworkInterfaceMessageDefragmentationTests.cpp
            test/network/client/Tcp/MqttSnClientNetworkInterfaceMessageDefragmentationTests.h
            test/network/client/Tcp/MqttSnClientNetworkTcpNetworkDefragmentationTestParameter.h
            ${LOGGING_FILES})
    target_link_libraries(MqttSnForwarderLinuxTcpClientNetworkTests
            gtest gtest_main gmock gmock_main pthread
            mock-mqtt-sn-fixed-size-ring-buffer
            lib-mqtt-sn-client-mock-network)

    add_executable(MqttSnForwarderLinuxUdpClientNetworkTests

            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.cpp
            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.h

            ${UDP_CLIENT_NETWORK_FILES}

            test/network/client/Udp/MockClientLinuxUdpNetworkImplementation.cpp
            test/network/client/Udp/MockClientLinuxUdpNetworkImplementation.h
            test/network/client/Udp/UdpMqttSnClientNetworkInterfaceTests.cpp

            test/network/helper/mock/udp/MockLinuxUdpHelper.cpp
            test/network/helper/mock/udp/MockLinuxUdpHelper.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.c
            test/network/helper/mock/udp/MockLinuxUdpDatagram.cpp
            test/network/helper/mock/udp/MockLinuxUdpDatagram.h
            ${LOGGING_FILES}
            )
    target_link_libraries(MqttSnForwarderLinuxUdpClientNetworkTests
            gtest gtest_main gmock gmock_main pthread
            mock-mqtt-sn-fixed-size-ring-buffer
            lib-mqtt-sn-client-mock-network)

    add_library(lib-mqtt-sn-gateway-mock-network
            forwarder/global_defines.h
            test/network/gateway/MockGateway/ComparableGatewayMqttSnMessageData.h

            test/network/gateway/MockGateway/MockGateway.cpp
            test/network/gateway/MockGateway/MockGateway.h
            test/network/gateway/MockGateway/MockGatewayNetworkInterface.h
            test/network/gateway/MockGateway/MockGatewayNetworkReceiverInterface.h
            test/network/gateway/MockGateway/MockGatewayNetworkReceiver.h

            test/network/gateway/MockForwardLooper/MockForwarderGatewayNetworkLooper.cpp
            test/network/gateway/MockForwardLooper/MockForwarderGatewayNetworkLooper.h

            test/network/gateway/TestConfigurations/MqttSnGatewayNetworkValueParameter.h
            test/network/gateway/TestConfigurations/MqttSnForwarderGatewayNetworkTestConfiguration.h
            test/network/gateway/TestConfigurations/MockGatewayConfiguration.h
            test/network/gateway/TestConfigurations/MqttSnGatewayNetworkMessageDataGenerator.cpp
            test/network/gateway/TestConfigurations/MqttSnGatewayNetworkMessageDataGenerator.h

            test/network/gateway/TestConfigurations/GetParameterMqttSnGatewayNetworkTestTypeParameter.cpp
            test/network/gateway/TestConfigurations/GetParameterMqttSnGatewayNetworkTestTypeParameter.h
            test/network/gateway/Tests/MqttSnGatewayNetworkInterfaceSendReceiveTests.cpp
            test/network/gateway/Tests/MqttSnGatewayNetworkInterfaceSendReceiveTests.h


            )
    include_directories(test/network/gateway
            test/network/gateway/MockGateway
            test/network/gateway/MockForwardLooper
            test/network/gateway/TestConfigurations
            test/network/gateway/Tests)
    target_link_libraries(lib-mqtt-sn-gateway-mock-network
            gtest gtest_main gmock gmock_main pthread
            lib-mqtt-sn-forwarder)

    add_executable(MqttSnForwarderLinuxTcpGatewayNetworkTests

            test/network/gateway/Tests/MqttSnGatewayNetworkInterfaceSendReceiveTests.cpp
            test/network/gateway/Tests/MqttSnGatewayNetworkInterfaceSendReceiveTests.h

            ${TCP_GATWAY_NETWORK_FILES}

            test/network/gateway/Tcp/MockGatewayLinuxTcpNetworkImplementation.cpp
            test/network/gateway/Tcp/MockGatewayLinuxTcpNetworkImplementation.h
            test/network/gateway/Tcp/LinuxTcpMqttSnGatewayNetworkInterfaceTests.cpp

            test/network/gateway/Tcp/MqttSnGatewayNetworkInterfaceMessageDefragmentationTests.cpp
            test/network/gateway/Tcp/MqttSnGatewayNetworkInterfaceMessageDefragmentationTests.h
            test/network/gateway/Tcp/GetMqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter.cpp
            test/network/gateway/Tcp/GetMqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter.h
            test/network/gateway/Tcp/MqttSnGatewayNetworkTcpNetworkDefragmentationTestParameter.h
            ${LOGGING_FILES}
            )
    target_link_libraries(MqttSnForwarderLinuxTcpGatewayNetworkTests
            gtest gtest_main gmock gmock_main pthread
            mock-mqtt-sn-fixed-size-ring-buffer
            lib-mqtt-sn-gateway-mock-network)

    add_executable(MqttSnForwarderLinuxUdpGatewayNetworkTests

            test/network/gateway/Tests/MqttSnGatewayNetworkInterfaceSendReceiveTests.cpp
            test/network/gateway/Tests/MqttSnGatewayNetworkInterfaceSendReceiveTests.h

            ${UDP_GATWAY_NETWORK_FILES}

            test/network/gateway/Udp/MockGatewayLinuxUdpNetworkImplementation.cpp
            test/network/gateway/Udp/MockGatewayLinuxUdpNetworkImplementation.h
            test/network/gateway/Udp/LinuxUdpMqttSnGatewayNetworkInterfaceTests.cpp

            forwarder/global_defines.h
            test/network/gateway/MockGateway/ComparableGatewayMqttSnMessageData.h
            test/network/helper/mock/udp/MockLinuxUdpHelper.cpp
            test/network/helper/mock/udp/MockLinuxUdpHelper.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.c
            test/network/helper/mock/udp/MockLinuxUdpDatagram.cpp
            test/network/helper/mock/udp/MockLinuxUdpDatagram.h
            ${LOGGING_FILES}
            )
    target_link_libraries(MqttSnForwarderLinuxUdpGatewayNetworkTests
            gtest gtest_main gmock gmock_main pthread
            mock-mqtt-sn-fixed-size-ring-buffer
            lib-mqtt-sn-gateway-mock-network)


    add_library(mock-mqtt-sn-fixed-size-ring-buffer
            test/shared/MockMqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferMockInterface.cpp
            test/shared/MockMqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferMockInterface.h
            test/shared/MockMqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferMock.h)
    include_directories(test/shared/MockMqttSnFixedSizeRingBuffer)
    target_link_libraries(mock-mqtt-sn-fixed-size-ring-buffer
            gtest gtest_main gmock gmock_main lib-mqtt-sn-forwarder)

    add_library(mock-mqtt-sn-client-network
            test/shared/MockMqttSnClientNetwork/ClientNetworkMockInterface.cpp
            test/shared/MockMqttSnClientNetwork/ClientNetworkMockInterface.h
            test/shared/MockMqttSnClientNetwork/ClientNetworkMock.h
            test/shared/MockMqttSnClientNetwork/FakeClientNetworkInterface.h
            test/network/helper/mock/udp/MockLinuxUdpHelper.cpp
            test/network/helper/mock/udp/MockLinuxUdpHelper.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.c
            test/network/helper/mock/udp/MockLinuxUdpDatagram.cpp
            test/network/helper/mock/udp/MockLinuxUdpDatagram.h)
    include_directories(test/shared/MockMqttSnClientNetwork)
    target_link_libraries(mock-mqtt-sn-client-network
            gtest gtest_main gmock gmock_main lib-mqtt-sn-forwarder)

    add_library(mock-mqtt-sn-gateway-network
            test/shared/MockMqttSnGatewayNetwork/GatewayNetworkMockInterface.cpp
            test/shared/MockMqttSnGatewayNetwork/GatewayNetworkMock.h
            test/shared/MockMqttSnGatewayNetwork/GatewayNetworkMockInterface.h
            test/shared/MockMqttSnGatewayNetwork/FakeClientNetworkInterface.h
            test/network/helper/mock/udp/MockLinuxUdpHelper.cpp
            test/network/helper/mock/udp/MockLinuxUdpHelper.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.c
            test/network/helper/mock/udp/MockLinuxUdpDatagram.cpp
            test/network/helper/mock/udp/MockLinuxUdpDatagram.h)
    include_directories(test/shared/MockMqttSnGatewayNetwork)
    target_link_libraries(mock-mqtt-sn-gateway-network
            gtest gtest_main gmock gmock_main lib-mqtt-sn-forwarder)

    add_executable(MqttSnGatewayNetworkInterfaceUnitTests
            test/MqttSnGatewayNetworkInterface/MqttSnGatewayNetworkInterfaceTests.cpp
            test/MqttSnGatewayNetworkInterface/MqttSnGatewayNetworkInterfaceTests.h)
    target_link_libraries(MqttSnGatewayNetworkInterfaceUnitTests
            gtest gtest_main gmock gmock_main lib-mqtt-sn-forwarder mock-mqtt-sn-gateway-network)

    add_executable(MqttSnClientNetworkInterfaceUnitTests
            test/MqttSnClientNetworkInterface/MqttSnClientNetworkInterfaceTests.cpp
            test/MqttSnClientNetworkInterface/MqttSnClientNetworkInterfaceTests.h)
    target_link_libraries(MqttSnClientNetworkInterfaceUnitTests
            gtest gtest_main gmock gmock_main lib-mqtt-sn-forwarder mock-mqtt-sn-client-network)

    add_executable(MqttSnFixedSizeRingBufferUnitTests
            test/MqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferTests.cpp
            test/MqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferTests.h
            test/shared/MqttSnFixedSizeRingBufferTests/CompareableMqttSnMessageData.h
            test/shared/MqttSnFixedSizeRingBufferTests/CompareableMqttSnDeviceAddress.h
            test/shared/MqttSnMessageDataGenerator/MqttSnMessageDataGenerator.cpp
            test/shared/MqttSnMessageDataGenerator/MqttSnMessageDataGenerator.h)
    target_link_libraries(MqttSnFixedSizeRingBufferUnitTests
            gtest gtest_main gmock gmock_main lib-mqtt-sn-ring-buffer)

    add_executable(MqttSnForwarderIntegrationTests
            test/MqttSnForwarder/MqttSnForwarderTests.cpp
            test/MqttSnForwarder/MqttSnForwarderTests.h
            test/MqttSnForwarder/AddForwardingHeaderToClientMessagesTests.cpp
            test/MqttSnForwarder/AddForwardingHeaderToClientMessagesTests.h
            test/MqttSnForwarder/MqttSnForwarderTestsGlobalVariables.h
            test/MqttSnForwarder/AddMqttSnForwardingHeaderTests.cpp
            test/MqttSnForwarder/AddMqttSnForwardingHeaderTests.h
            test/MqttSnForwarder/RemoveMqttSnForwardingHeaderTests.cpp
            test/MqttSnForwarder/RemoveMqttSnForwardingHeaderTests.h
            test/MqttSnForwarder/RemoveForwardingHeaderFromGatewayMessagesTests.cpp
            test/MqttSnForwarder/RemoveForwardingHeaderFromGatewayMessagesTests.h
            test/MqttSnForwarder/SendBufferedMessagesToGatewayTests.cpp
            test/MqttSnForwarder/SendBufferedMessagesToGatewayTests.h
            test/MqttSnForwarder/SendBufferedMessagesToClientsTests.cpp
            test/MqttSnForwarder/SendBufferedMessagesToClientsTests.h
            test/shared/PlaceholderNetworkContext/PlaceholderNetworkContext.h
            test/MqttSnForwarder/MqttSnForwarderLoopTests.cpp
            test/MqttSnForwarder/MqttSnForwarderLoopTests.h
            test/MqttSnForwarder/MqttSnForwarderDeinitTests.cpp
            test/MqttSnForwarder/MqttSnForwarderDeinitTests.h
            test/MqttSnForwarder/MqttSnForwarderInitTests.cpp
            test/MqttSnForwarder/MqttSnForwarderInitTests.h
            test/shared/MqttSnMessageDataGenerator/MqttSnMessageDataGenerator.cpp
            test/shared/MqttSnMessageDataGenerator/MqttSnMessageDataGenerator.h
            forwarder/MqttSnMessageParser.c
            forwarder/MqttSnMessageParser.h
            ${LOGGING_FILES})
    target_link_libraries(MqttSnForwarderIntegrationTests
            gtest gtest_main gmock gmock_main lib-mqtt-sn-forwarder
            mock-mqtt-sn-fixed-size-ring-buffer mock-mqtt-sn-client-network mock-mqtt-sn-gateway-network)


    if (${CMAKE_VERSION} VERSION_GREATER "3.12.0" AND CMAKE_COMPILER_IS_GNUCXX)
        #set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
        LIST(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules")
        INCLUDE(CodeCoverage)
        APPEND_COVERAGE_COMPILER_FLAGS()
        #LIST(APPEND Coverage_GENHTML_ARGS "--rc lcov_branch_coverage=1")
        set(COVERAGE_LCOV_EXCLUDES '/usr/*' 'test/*' '${PROJECT_SOURCE_DIR}/test/*' '${CMAKE_CURRENT_BINARY_DIR}/*')

        #set(RING_EXCLUDES '${PROJECT_SOURCE_DIR}/forwarder/MqttSnFordwarder.*' '${PROJECT_SOURCE_DIR}/forwarder/MqttSnClientNetworkInterface.*' '${PROJECT_SOURCE_DIR}/forwarder/MqttSnGatewayNetworkInterface.*')
        SETUP_TARGET_FOR_COVERAGE_LCOV_LOCAL_EXLUCDE(NAME "MqttSnFixedSizeRingBufferUnitTests_coverage"
                EXECUTABLE "MqttSnFixedSizeRingBufferUnitTests"
                DEPENDENCIES "MqttSnFixedSizeRingBufferUnitTests"
                EXCLUDES '${PROJECT_SOURCE_DIR}/forwarder/MqttSnFordwarder.*'
                '${PROJECT_SOURCE_DIR}/forwarder/MqttSnClientNetworkInterface.*'
                '${PROJECT_SOURCE_DIR}/forwarder/MqttSnGatewayNetworkInterface.*')

        #SETUP_TARGET_FOR_COVERAGE_LCOV_LOCAL_EXLUCDE(NAME "MqttSnForwarderIntegrationTests_coverage"
        #        EXECUTABLE "MqttSnForwarderIntegrationTests"
        #        DEPENDENCIES "MqttSnForwarderIntegrationTests"
        #        EXCLUDES '${PROJECT_SOURCE_DIR}/forwarder/MqttSnFixedSizeRingBuffer.*')

        #SETUP_TARGET_FOR_COVERAGE_GCOVR_XML(NAME "MqttSnForwarderIntegrationTests_coverage"
        #        EXECUTABLE "MqttSnForwarderIntegrationTests"
        #        DEPENDENCIES "MqttSnForwarderIntegrationTests")
        #SETUP_TARGET_FOR_COVERAGE_LCOV(NAME "MqttSnClientNetworkInterfaceUnitTests_coverage"
        #        EXECUTABLE "MqttSnClientNetworkInterfaceUnitTests")
        #SETUP_TARGET_FOR_COVERAGE_LCOV(NAME "MqttSnGatewayNetworkInterfaceUnitTests_coverage"
        #        EXECUTABLE "MqttSnGatewayNetworkInterfaceUnitTests")
    else ()
        message("Consider to switch to CMake 3.12.0 and GNUCXX for test coverage")
    endif ()


endif ()
