cmake_minimum_required(VERSION 3.10)
project(CMqttSnForwarder C CXX)

if (BUILD_TESTING)
    project(CMqttSnForwarder C CXX)
else ()
    project(CMqttSnForwarder C)
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

set(IMPLEMENTATIONL_FILES
        forwarder/MqttSnClientNetworkInterface.h
        forwarder/MqttSnForwarder.h
        forwarder/MqttSnGatewayNetworkInterface.h
        forwarder/MqttSnFordwarder.c
        forwarder/global_defines.h
        forwarder/MqttSnGatewayNetworkInterface.c
        forwarder/MqttSnClientNetworkInterface.c
        forwarder/network/plugin/plugin_interface.h forwarder/network/plugin/MqttSnGatewayPluginNetwork.h forwarder/network/plugin/MqttSnGatewayPluginNetwork.c)
include_directories(forwarder)

set(CMQTTSNFORWADER_ERROR_COMPILE_FLAGS -Wall -Wextra -pedantic -Werror -Wno-unused-parameter)

set(MQTT_SN_FIXED_SIZE_RING_BUFFER_FILES
        forwarder/MqttSnFixedSizeRingBuffer.h
        forwarder/MqttSnFixedSizeRingBuffer.c)

set(TCP_GATWAY_NETWORK_FILES
        forwarder/network/gateway/tcp/MqttSnGatewayTcpNetwork.h
        forwarder/network/gateway/tcp/MqttSnGatewayTcpNetwork.c
        forwarder/network/tcphelper/MqttSnTcpNetworkMessageParser.c
        forwarder/network/tcphelper/MqttSnTcpNetworkMessageParser.h
        forwarder/network/iphelper/MqttSnIpNetworkHelper.h
        forwarder/network/iphelper/MqttSnIpNetworkHelper.c)

set(UDP_GATWAY_NETWORK_FILES
        forwarder/network/gateway/udp/MqttSnGatewayUdpNetwork.h
        forwarder/network/gateway/udp/MqttSnGatewayUdpNetwork.c
        forwarder/network/udphelper/MqttSnUdpNetworkMessageParser.h
        forwarder/network/udphelper/MqttSnUdpNetworkMessageParser.c
        forwarder/network/iphelper/MqttSnIpNetworkHelper.h
        forwarder/network/iphelper/MqttSnIpNetworkHelper.c)

set(TCP_CLIENT_NETWORK_FILES
        forwarder/network/client/tcp/MqttSnClientTcpNetwork.h
        forwarder/network/client/tcp/MqttSnClientTcpNetwork.c
        forwarder/network/tcphelper/MqttSnTcpNetworkMessageParser.c
        forwarder/network/tcphelper/MqttSnTcpNetworkMessageParser.h
        forwarder/network/iphelper/MqttSnIpNetworkHelper.h
        forwarder/network/iphelper/MqttSnIpNetworkHelper.c)

set(UDP_CLIENT_NETWORK_FILES
        forwarder/network/client/udp/MqttSnClientUdpNetwork.c
        forwarder/network/client/udp/MqttSnClientUdpNetwork.h
        forwarder/network/udphelper/MqttSnUdpNetworkMessageParser.h
        forwarder/network/udphelper/MqttSnUdpNetworkMessageParser.c
        forwarder/network/iphelper/MqttSnIpNetworkHelper.h
        forwarder/network/iphelper/MqttSnIpNetworkHelper.c)

add_library(lib-mqtt-sn-forwarder ${IMPLEMENTATIONL_FILES})
target_link_libraries(lib-mqtt-sn-forwarder dl)
target_compile_options(lib-mqtt-sn-forwarder PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})

add_library(lib-mqtt-sn-ring-buffer ${MQTT_SN_FIXED_SIZE_RING_BUFFER_FILES})
target_compile_options(lib-mqtt-sn-ring-buffer PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})

add_library(lib-mqtt-sn-gateway-network ${TCP_GATWAY_NETWORK_FILES} ${UDP_GATWAY_NETWORK_FILES})
target_link_libraries(lib-mqtt-sn-gateway-network lib-mqtt-sn-ring-buffer)
target_compile_options(lib-mqtt-sn-gateway-network PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})

add_library(lib-mqtt-sn-client-network ${TCP_CLIENT_NETWORK_FILES} ${UDP_CLIENT_NETWORK_FILES}
        forwarder/network/iphelper/MqttSnIpNetworkHelper.h forwarder/network/iphelper/MqttSnIpNetworkHelper.c)
target_link_libraries(lib-mqtt-sn-client-network lib-mqtt-sn-ring-buffer)
target_compile_options(lib-mqtt-sn-client-network PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})

if (NOT BUILD_TESTING)
    message("Bulding as C project")
    add_executable(CMqttSnForwarder
            main/main.c
            main/forwarder_config.h
            main/forwarder_config.c
            main/config.h forwarder/network/iphelper/MqttSnIpNetworkHelper.h forwarder/network/iphelper/MqttSnIpNetworkHelper.c main/forwarder_starter.h main/forwarder_starter.c)
    target_link_libraries(CMqttSnForwarder
            pthread
            lib-mqtt-sn-forwarder lib-mqtt-sn-gateway-network lib-mqtt-sn-client-network)
    target_compile_options(CMqttSnForwarder PUBLIC ${CMQTTSNFORWADER_ERROR_COMPILE_FLAGS})

    add_executable(TcpCMqttSnForwarderTester tcptestermain.c forwarder/global_defines.h)
    target_link_libraries(TcpCMqttSnForwarderTester lib-mqtt-sn-forwarder pthread)

    add_executable(UdpCMqttSnForwarderTester udptestermain.c forwarder/global_defines.h)
    target_link_libraries(UdpCMqttSnForwarderTester lib-mqtt-sn-forwarder pthread)
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
            test/network/client/Tcp/MqttSnClientNetworkTcpNetworkDefragmentationTestParameter.h)
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
            )
    target_link_libraries(MqttSnForwarderLinuxUdpGatewayNetworkTests
            gtest gtest_main gmock gmock_main pthread
            mock-mqtt-sn-fixed-size-ring-buffer
            lib-mqtt-sn-gateway-mock-network)


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
            test/MqttSnClientNetworkInterface/MockMqttSnClientNetwork/FakeClientNetworkInterface.h
            test/network/helper/mock/udp/MockLinuxUdpHelper.cpp
            test/network/helper/mock/udp/MockLinuxUdpHelper.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.c
            test/network/helper/mock/udp/MockLinuxUdpDatagram.cpp
            test/network/helper/mock/udp/MockLinuxUdpDatagram.h)
    include_directories(test/MqttSnClientNetworkInterface/MockMqttSnClientNetwork)
    target_link_libraries(mock-mqtt-sn-client-network
            gtest gtest_main gmock gmock_main lib-mqtt-sn-forwarder)

    add_library(mock-mqtt-sn-gateway-network
            test/MqttSnGatewayNetworkInterface/MockMqttSnGatewayNetwork/GatewayNetworkMockInterface.cpp
            test/MqttSnGatewayNetworkInterface/MockMqttSnGatewayNetwork/GatewayNetworkMock.h
            test/MqttSnGatewayNetworkInterface/MockMqttSnGatewayNetwork/GatewayNetworkMockInterface.h
            test/MqttSnGatewayNetworkInterface/MockMqttSnGatewayNetwork/FakeClientNetworkInterface.h
            test/network/helper/mock/udp/MockLinuxUdpHelper.cpp
            test/network/helper/mock/udp/MockLinuxUdpHelper.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.h
            forwarder/network/iphelper/MqttSnIpNetworkHelper.c
            test/network/helper/mock/udp/MockLinuxUdpDatagram.cpp
            test/network/helper/mock/udp/MockLinuxUdpDatagram.h)
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
                EXCLUDES '${PROJECT_SOURCE_DIR}/forwarder/MqttSnFordwarder.*' '${PROJECT_SOURCE_DIR}/forwarder/MqttSnClientNetworkInterface.*' '${PROJECT_SOURCE_DIR}/forwarder/MqttSnGatewayNetworkInterface.*')

        SETUP_TARGET_FOR_COVERAGE_LCOV_LOCAL_EXLUCDE(NAME "MqttSnForwarderIntegrationTests_coverage"
                EXECUTABLE "MqttSnForwarderIntegrationTests"
                DEPENDENCIES "MqttSnForwarderIntegrationTests"
                EXCLUDES '${PROJECT_SOURCE_DIR}/forwarder/MqttSnFixedSizeRingBuffer.*')

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



