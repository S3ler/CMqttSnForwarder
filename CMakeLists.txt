cmake_minimum_required(VERSION 3.10)
project(CMqttSnForwarder C CXX)

if (DEFINED CMAKE_CXX_COMPILER_ENV_VAR)
    set(CMAKE_CXX_STANDARD 11)
endif ()
if(DEFINED CMAKE_C_COMPILER_ENV_VAR AND NOT DEFINED CMAKE_CXX_COMPILER_ENV_VAR)
    set(CMAKE_C_STANDARD 99)
endif ()
if (NOT DEFINED CMAKE_C_STANDARD AND NOT DEFINED CMAKE_CXX_STANDARD)
    message(FATAL_ERROR "Neither C nor C++ project")
endif ()

set(IMPLEMENTATIONL_FILES
        forwarder/MqttSnClientNetworkInterface.h
        forwarder/MqttSnForwarder.h
        forwarder/MqttSnGatewayNetworkInterface.h
        forwarder/MqttSnFordwarder.c
        forwarder/global_defines.h
        forwarder/MqttSnGatewayNetworkInterface.c
        forwarder/MqttSnClientNetworkInterface.c
)
include_directories(forwarder)

set(MQTT_SN_FIXED_SIZE_RING_BUFFER_FILES
        forwarder/MqttSnFixedSizeRingBuffer.h
        forwarder/MqttSnFixedSizeRingBuffer.c)

set(TCP_GATWAY_NETWORK_FILES
        forwarder/network/gateway/tcp/MqttSnGatewayTcpNetwork.h
        forwarder/network/gateway/tcp/MqttSnGatewayTcpNetwork.c
        forwarder/network/tcphelper/MqttSnTcpNetworkMessageParser.c
        forwarder/network/tcphelper/MqttSnTcpNetworkMessageParser.h)

set(TCP_CLIENT_NETWORK_FILES
        forwarder/network/client/tcp/MqttSnClientTcpNetwork.h
        forwarder/network/client/tcp/MqttSnClientTcpNetwork.c
        forwarder/network/tcphelper/MqttSnTcpNetworkMessageParser.c
        forwarder/network/tcphelper/MqttSnTcpNetworkMessageParser.h)

add_library(lib-mqtt-sn-forwarder ${IMPLEMENTATIONL_FILES})

add_library(lib-mqtt-sn-ring-buffer ${MQTT_SN_FIXED_SIZE_RING_BUFFER_FILES})

add_library(lib-mqtt-sn-gateway-network ${TCP_GATWAY_NETWORK_FILES})
target_link_libraries(lib-mqtt-sn-gateway-network lib-mqtt-sn-ring-buffer)

add_library(lib-mqtt-sn-client-network ${TCP_CLIENT_NETWORK_FILES})
target_link_libraries(lib-mqtt-sn-client-network lib-mqtt-sn-ring-buffer)


if (CMAKE_C_STANDARD)
    message("Bulding as C project")
    add_executable(CMqttSnForwarder main/main.c)
    target_link_libraries(CMqttSnForwarder lib-mqtt-sn-forwarder lib-mqtt-sn-gateway-network lib-mqtt-sn-client-network)
    add_executable(CMqttSnForwarderTester testermain.c forwarder/global_defines.h)
    target_link_libraries(CMqttSnForwarderTester lib-mqtt-sn-forwarder)
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
            test/network/client/MockClientNetwork/MockClient/MockClient.cpp
            test/network/client/MockClientNetwork/MockClient/MockClient.h
            test/network/client/MockClientNetwork/MockClient/MockClientNetworkInterface.h
            test/network/client/MockClientNetwork/MockClient/MockClientNetworkReceiverInterface.h
            test/network/client/MockClientNetwork/MockClient/MockClientNetworkReceiver.h
            test/network/client/MockClientNetwork/MockGateway/ClientNetworkGatewayLooper.cpp
            test/network/client/MockClientNetwork/MockGateway/ClientNetworkGatewayLooper.h
            test/network/client/MockClientNetwork/MockClient/CompareableMqttSnMessageData.h
            test/network/client/TestConfigurations/MqttSnClientNetworkTestValueParameter.h
            test/network/client/TestConfigurations/MqttSnGatewayClientNetworkTestConfiguration.h
            test/network/client/TestConfigurations/MockClientConfiguration.h
            test/network/client/TestConfigurations/ClientNetworkMessageDataGenerator.cpp
            test/network/client/TestConfigurations/ClientNetworkMessageDataGenerator.h
            test/network/client/TestConfigurations/CartesianProductTestCaseGenerator.cpp
            test/network/client/TestConfigurations/CartesianProductTestCaseGenerator.h
            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.cpp
            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.h)
    include_directories(test/network/client/MockClientNetwork
            test/network/client/TestConfigurations
            test/network/client/Tests)
    target_link_libraries(lib-mqtt-sn-client-mock-network
            gtest gtest_main gmock gmock_main pthread
            lib-mqtt-sn-forwarder)

    add_executable(MqttSnForwarder_LinuxTcpClientNetwork_Tests

            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.cpp
            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.h

            ${TCP_CLIENT_NETWORK_FILES}
            test/network/client/Tcp/MockClientLinuxTcpNetworkImplementation.cpp
            test/network/client/Tcp/MockClientLinuxTcpNetworkImplementation.h
            test/network/client/Tcp/TcpMqttSnClientNetworkInterfaceTests.cpp)

    target_link_libraries(MqttSnForwarder_LinuxTcpClientNetwork_Tests
            gtest gtest_main gmock gmock_main pthread
            mock-mqtt-sn-fixed-size-ring-buffer
            lib-mqtt-sn-client-mock-network)

    add_library(lib-mqtt-sn-gateway-mock-network
            test/network/gateway/MockGatewayNetwork/MockGateway.h
            forwarder/global_defines.h
            test/network/gateway/MockGatewayNetwork/MockGateway.cpp
            test/network/gateway/MockGatewayNetwork/MockGatewayNetworkInterface.h

            test/network/gateway/MockGatewayNetwork/MockGatewayNetworkReceiverInterface.h
            test/network/gateway/MockGatewayNetwork/MockGatewayNetworkReceiver.h
            test/network/gateway/TestConfigurations/MqttSnGatewayNetworkValueParameter.h
            test/network/gateway/TestConfigurations/MockGatewayConfiguration.h
            test/network/gateway/TestConfigurations/MqttSnGatewayNetworkMessageDataGenerator.cpp
            test/network/gateway/TestConfigurations/MqttSnGatewayNetworkMessageDataGenerator.h
            test/network/gateway/TestConfigurations/MqttSnForwarderGatewayNetworkTestConfiguration.h
            test/network/gateway/MockForwarderGatewayNetworkLooper/MockForwarderGatewayNetworkLooper.cpp
            test/network/gateway/MockForwarderGatewayNetworkLooper/MockForwarderGatewayNetworkLooper.h
            test/network/gateway/TestConfigurations/GetParameterMqttSnGatewayNetworkTestTypeParameter.cpp
            test/network/gateway/TestConfigurations/GetParameterMqttSnGatewayNetworkTestTypeParameter.h)
    target_link_libraries(lib-mqtt-sn-gateway-mock-network gtest gtest_main gmock gmock_main pthread)

    add_executable(MqttSnForwarder_LinuxTcpGatewayNetwork_Tests
            forwarder/global_defines.h

            test/network/gateway/Tests/MqttSnGatewayNetworkInterfaceSendReceiveTests.h
            test/network/gateway/Tcp/MockGatewayLinuxTcpNetworkImplementation.cpp
            test/network/gateway/Tcp/MockGatewayLinuxTcpNetworkImplementation.h
            test/network/gateway/MockGatewayNetwork/MockGateway.h
            test/network/gateway/MockGatewayNetwork/MockGateway.cpp
            test/network/gateway/MockGatewayNetwork/MockGatewayNetworkInterface.h

            test/network/gateway/MockGatewayNetwork/MockGatewayNetworkReceiverInterface.h
            test/network/gateway/MockGatewayNetwork/MockGatewayNetworkReceiver.h
            test/network/gateway/TestConfigurations/MqttSnGatewayNetworkValueParameter.h
            test/network/gateway/TestConfigurations/MockGatewayConfiguration.h
            test/network/gateway/TestConfigurations/MqttSnGatewayNetworkMessageDataGenerator.cpp
            test/network/gateway/TestConfigurations/MqttSnGatewayNetworkMessageDataGenerator.h
            test/network/gateway/TestConfigurations/MqttSnForwarderGatewayNetworkTestConfiguration.h
            test/network/gateway/MockForwarderGatewayNetworkLooper/MockForwarderGatewayNetworkLooper.cpp
            test/network/gateway/MockForwarderGatewayNetworkLooper/MockForwarderGatewayNetworkLooper.h
            test/network/gateway/TestConfigurations/GetParameterMqttSnGatewayNetworkTestTypeParameter.cpp
            test/network/gateway/TestConfigurations/GetParameterMqttSnGatewayNetworkTestTypeParameter.h)
    target_link_libraries(MqttSnForwarder_LinuxTcpGatewayNetwork_Tests
            gtest gtest_main gmock gmock_main pthread lib-mqtt-sn-gateway-network)

    add_library(mock-mqtt-sn-fixed-size-ring-buffer
            test/MqttSnClientNetworkInterface/MockMqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferMockInterface.cpp
            test/MqttSnClientNetworkInterface/MockMqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferMockInterface.h
            test/MqttSnClientNetworkInterface/MockMqttSnFixedSizeRingBuffer/MqttSnFixedSizeRingBufferMock.h)
    include_directories(test/MqttSnClientNetworkInterface/MockMqttSnFixedSizeRingBuffer)
    target_link_libraries(mock-mqtt-sn-fixed-size-ring-buffer
            gtest gtest_main gmock gmock_main lib-mqtt-sn-forwarder)

    add_library(mock-mqtt-sn-client-network
            test/MqttSnClientNetworkInterface/MockMqttSnClientNetwork/ClientNetworkMockInterface.cpp
            test/MqttSnClientNetworkInterface/MockMqttSnClientNetwork/ClientNetworkMockInterface.h
            test/MqttSnClientNetworkInterface/MockMqttSnClientNetwork/ClientNetworkMock.h
            test/MqttSnClientNetworkInterface/MockMqttSnClientNetwork/FakeClientNetworkInterface.h)
    include_directories(test/MqttSnClientNetworkInterface/MockMqttSnClientNetwork)
    target_link_libraries(mock-mqtt-sn-client-network
            gtest gtest_main gmock gmock_main lib-mqtt-sn-forwarder)

    add_library(mock-mqtt-sn-gateway-network
            test/MqttSnGatewayNetworkInterface/MockMqttSnGatewayNetwork/GatewayNetworkMockInterface.cpp
            test/MqttSnGatewayNetworkInterface/MockMqttSnGatewayNetwork/GatewayNetworkMock.h
            test/MqttSnGatewayNetworkInterface/MockMqttSnGatewayNetwork/GatewayNetworkMockInterface.h
            test/MqttSnGatewayNetworkInterface/MockMqttSnGatewayNetwork/FakeClientNetworkInterface.h)
    include_directories(test/MqttSnGatewayNetworkInterface/MockMqttSnGatewayNetwork)
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
            test/MqttSnFixedSizeRingBuffer/ComparableMqttSnMessageData.h)
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
            test/MqttSnForwarder/PlaceholderNetworkContext/PlaceholderNetworkContext.h
            test/MqttSnForwarder/MqttSnForwarderLoopTests.cpp
            test/MqttSnForwarder/MqttSnForwarderLoopTests.h
            test/MqttSnForwarder/MqttSnForwarderDeinitTests.cpp
            test/MqttSnForwarder/MqttSnForwarderDeinitTests.h
            test/MqttSnForwarder/MqttSnForwarderInitTests.cpp
            test/MqttSnForwarder/MqttSnForwarderInitTests.h)
    target_link_libraries(MqttSnForwarderIntegrationTests
            gtest gtest_main gmock gmock_main lib-mqtt-sn-forwarder
            mock-mqtt-sn-fixed-size-ring-buffer mock-mqtt-sn-client-network mock-mqtt-sn-gateway-network)

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
endif ()



