cmake_minimum_required(VERSION 3.12)
#project(CMqttSnForwarder C)
project(CMqttSnForwarder C CXX)

#set(CMAKE_C_STANDARD 99)
#enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)

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
        forwarder/MqttSnClientNetworkInterface.c)

set(MQTT_SN_FIXED_SIZE_RING_BUFFER_FILES
        forwarder/MqttSnFixedSizeRingBuffer.h
        forwarder/MqttSnFixedSizeRingBuffer.c)

set(TCP_GATWAY_NETWORK_FILES
        forwarder/network/gateway/tcp/MqttSnGatewayTcpNetwork.h
        forwarder/network/gateway/tcp/MqttSnGatewayTcpNetwork.c)

set(TCP_CLIENT_NETWORK_FILES
        forwarder/network/client/tcp/MqttSnClientTcpNetwork.h
        forwarder/network/client/tcp/MqttSnClientTcpNetwork.c)

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

    enable_testing()

    # Now simply link against gtest or gtest_main as needed. Eg
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
            test/network/gateway/Tests/MqttSnGatewayNetworkInterfaceSendReceiveTests.cpp test/network/gateway/Tcp/LinuxTcpMqttSnGatewayNetworkInterfaceTests.cpp)

    target_link_libraries(GatewayTests
            gtest gtest_main gmock gmock_main pthread
            lib-mqtt-sn-forwarder
            lib-mqtt-sn-gateway-network)

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
            test/network/client/MockClientNetwork/MockClient/MockClientMqttSnMessageData.cpp
            test/network/client/MockClientNetwork/MockClient/MockClientMqttSnMessageData.h
            test/network/client/TestConfigurations/MqttSnClientNetworkTestValueParameter.h
            test/network/client/TestConfigurations/MqttSnGatewayClientNetworkTestConfiguration.h
            test/network/client/TestConfigurations/MockClientConfiguration.h
            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.cpp
            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.h
            test/network/client/TestConfigurations/ClientNetworkMessageDataGenerator.cpp
            test/network/client/TestConfigurations/ClientNetworkMessageDataGenerator.h
            test/network/client/TestConfigurations/CartesianTestGenerator.cpp
            test/network/client/TestConfigurations/CartesianTestGenerator.h
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
            test/network/client/MockClientNetwork/MockClient/MockClientMqttSnMessageData.cpp
            test/network/client/MockClientNetwork/MockClient/MockClientMqttSnMessageData.h
            test/network/client/TestConfigurations/MqttSnClientNetworkTestValueParameter.h
            test/network/client/TestConfigurations/MqttSnGatewayClientNetworkTestConfiguration.h
            test/network/client/TestConfigurations/MockClientConfiguration.h
            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.cpp
            test/network/client/Tests/MqttSnClientNetworkInterfaceTests.h
            test/network/client/TestConfigurations/ClientNetworkMessageDataGenerator.cpp
            test/network/client/TestConfigurations/ClientNetworkMessageDataGenerator.h
            test/network/client/TestConfigurations/CartesianTestGenerator.cpp
            test/network/client/TestConfigurations/CartesianTestGenerator.h

            test/network/client/Tcp/MockClientLinuxTcpNetworkImplementation.cpp
            test/network/client/Tcp/MockClientLinuxTcpNetworkImplementation.h
            test/network/client/Tcp/TcpMqttSnClientNetworkInterfaceTests.cpp)

    #  add_executable(MqttSnForwarder_LinuxTcpClientNetwork_Tests
    #          test/network/client/Tcp/MockClientLinuxTcpNetworkImplementation.cpp
    #          test/network/client/Tcp/MockClientLinuxTcpNetworkImplementation.h
    #          test/network/client/Tcp/TcpMqttSnClientNetworkInterfaceTests.cpp)

    target_link_libraries(MqttSnForwarder_LinuxTcpClientNetwork_Tests
            lib-mqtt-sn-client-mock-network
            gtest gtest_main gmock gmock_main pthread
            lib-mqtt-sn-gateway-network lib-mqtt-sn-forwarder lib-mqtt-sn-client-network)

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
            test/network/gateway/Tests/MqttSnGatewayNetworkInterfaceSendReceiveTests.h
            test/network/gateway/Tcp/MockGatewayLinuxTcpNetworkImplementation.cpp
            test/network/gateway/Tcp/MockGatewayLinuxTcpNetworkImplementation.h
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
            test/network/gateway/TestConfigurations/GetParameterMqttSnGatewayNetworkTestTypeParameter.h
            )
    target_link_libraries(MqttSnForwarder_LinuxTcpGatewayNetwork_Tests
            gtest gtest_main gmock gmock_main pthread lib-mqtt-sn-gateway-network)
    #add_executable(MqttSnForwarderTest
    #        test/MqttSnForwarder_Tests.cpp
    #        test/MqttSnForwarder_LinuxTcpGatewayNetwork_Tests.cpp test/MqttSnForwarder_LinuxTcpClientNetwork_Tests.cpp)
    #target_link_libraries(MqttSnForwarderTest
    #        gtest gtest_main gmock gmock_main
    #        lib-mqtt-sn-forwarder
    #        lib-mqtt-sn-gateway-mock-network
    #        lib-mqtt-sn-client-network)
    #add_test(NAME example_test COMMAND MqttSnForwarderTest)


endif ()



