#*******************************************************************************
# Copyright 2018 Gabriel Nikol
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#*******************************************************************************/
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(PAHOMQTTSNGATEWAY C CXX)

## build settings
SET(PAHO_VERSION_MAJOR 1)
SET(PAHO_VERSION_MINOR 0)
SET(PAHO_VERSION_PATCH 0)

SET(PROGNAME_PREFIX Paho)


MESSAGE(STATUS "CMake version: " ${CMAKE_VERSION})
MESSAGE(STATUS "CMake system name: " ${CMAKE_SYSTEM_NAME})

SET(PROGNAME ${PROGNAME_PREFIX}-MQTT-SNGateway)
SET(APPL mainGateway)

SET(LPROGNAME ${PROGNAME_PREFIX}-MQTT-SNLogmonitor)
SET(LAPPL mainLogmonitor)

SET(TESTPROGNAME ${PROGNAME_PREFIX}-testPFW)
SET(TESTAPPL mainTestProcess)


SET(CONFIG gateway.conf)
SET(CLIENTS clients.conf)
SET(PREDEFTOPIC predefinedTopic.conf)

SET(SRCDIR paho.mqtt-sn-embedded-c-c9e807da319bd68bff71cc4817ea5a4f4b25f49d/MQTTSNGateway/src)
SET(SUBDIR ${SRCDIR}/../../MQTTSNPacket/src)

SET(OS linux)
SET(SENSORNET udp)
SET(TEST tests)

#SET(INSTALL_DIR ../../)
#SET(CONFIG_DIR ../../)

SET(CCPSRCS
        ${SRCDIR}/MQTTGWConnectionHandler.cpp
        ${SRCDIR}/MQTTGWPacket.cpp
        ${SRCDIR}/MQTTGWPublishHandler.cpp
        ${SRCDIR}/MQTTGWSubscribeHandler.cpp
        ${SRCDIR}/MQTTSNGateway.cpp
        ${SRCDIR}/MQTTSNGWBrokerRecvTask.cpp
        ${SRCDIR}/MQTTSNGWBrokerSendTask.cpp
        ${SRCDIR}/MQTTSNGWClient.cpp
        ${SRCDIR}/MQTTSNGWClientRecvTask.cpp
        ${SRCDIR}/MQTTSNGWClientSendTask.cpp
        ${SRCDIR}/MQTTSNGWConnectionHandler.cpp
        ${SRCDIR}/MQTTSNGWLogmonitor.cpp
        ${SRCDIR}/MQTTSNGWPacket.cpp
        ${SRCDIR}/MQTTSNGWPacketHandleTask.cpp
        ${SRCDIR}/MQTTSNGWProcess.cpp
        ${SRCDIR}/MQTTSNGWPublishHandler.cpp
        ${SRCDIR}/MQTTSNGWSubscribeHandler.cpp
        ${SRCDIR}/MQTTSNGWEncapsulatedPacket.cpp
        ${SRCDIR}/MQTTSNGWForwarder.cpp
        ${SRCDIR}/MQTTSNGWQoSm1Proxy.cpp
        ${SRCDIR}/MQTTSNGWAdapter.cpp
        ${SRCDIR}/MQTTSNGWAggregater.cpp
        ${SRCDIR}/MQTTSNGWClientList.cpp
        ${SRCDIR}/MQTTSNGWTopic.cpp
        ${SRCDIR}/MQTTSNGWAdapterManager.cpp
        ${SRCDIR}/MQTTSNAggregateConnectionHandler.cpp
        ${SRCDIR}/MQTTSNGWMessageIdTable.cpp
        ${SRCDIR}/MQTTSNGWAggregateTopicTable.cpp
        ${SRCDIR}/${OS}/${SENSORNET}/SensorNetwork.cpp
        ${SRCDIR}/${OS}/Timer.cpp
        ${SRCDIR}/${OS}/Network.cpp
        ${SRCDIR}/${OS}/Threading.cpp
        ${SRCDIR}/${TEST}/TestProcess.cpp
        ${SRCDIR}/${TEST}/TestQue.cpp
        ${SRCDIR}/${TEST}/TestTree23.cpp
        ${SRCDIR}/${TEST}/TestTopics.cpp
        ${SRCDIR}/${TEST}/TestTopicIdMap.cpp
        ${SRCDIR}/${TEST}/TestTask.cpp)


SET(CSRCS
        ${SUBDIR}/MQTTSNConnectClient.c
        ${SUBDIR}/MQTTSNConnectServer.c
        ${SUBDIR}/MQTTSNDeserializePublish.c
        ${SUBDIR}/MQTTSNPacket.c
        ${SUBDIR}/MQTTSNSearchClient.c
        ${SUBDIR}/MQTTSNSearchServer.c
        ${SUBDIR}/MQTTSNSerializePublish.c
        ${SUBDIR}/MQTTSNSubscribeClient.c
        ${SUBDIR}/MQTTSNSubscribeServer.c
        ${SUBDIR}/MQTTSNUnsubscribeClient.c
        ${SUBDIR}/MQTTSNUnsubscribeServer.c)

# cmake detects this automatically
# SET(CXX g++)
# SET(CPPFLAG )

# SET(INCLUDE )
include_directories(
        ${SRCDIR}
        ${SRCDIR}/${OS}
        ${SRCDIR}/${OS}/${SENSORNET}
        ${SUBDIR}
        ${SUBDIR}/${TEST}
)

# SET(DEFS )
# SET(LIB )
# SET(LIB /usr/local/lib)
# SET(LDFLAGS )
set(CMAKE_CXX_STANDARD 11)
# these are the default CMAKE_CXX_FLAGS in cmake for release
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")
SET(LDADD pthread ssl crypto rt)
SET(OUTDIR Build)
SET(PROG ${OUTDIR}/${PROGNAME})
SET(LPROG ${OUTDIR}/${LPROGNAME})
SET(TPROG ${OUTDIR}/${TPROGNAME})

# for ssl:

# build and link libraries of MQTT-SNGateway, MQTT-SNLogmonitor, testPFW
add_library(Lib${PROGNAME} ${CCPSRCS} ${CSRCS} ${SRCDIR}/${APPL})
target_link_libraries(Lib${PROGNAME} ${LDADD})

add_executable(${PROGNAME} ${CCPSRCS} ${CSRCS} ${SRCDIR}/${APPL})
target_link_libraries(${PROGNAME} ${LDADD})
add_custom_command(TARGET ${PROGNAME}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${SRCDIR}/../${CONFIG} ${CONFIG}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${SRCDIR}/../${CLIENTS} ${CLIENTS}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${SRCDIR}/../${PREDEFTOPIC} ${PREDEFTOPIC}
        )

add_executable(Lib${LPROGNAME} ${CCPSRCS} ${CSRCS} ${SRCDIR}/${LAPPL})
target_link_libraries(Lib${LPROGNAME} ${LDADD})

add_executable(${LPROGNAME} ${CCPSRCS} ${CSRCS} ${SRCDIR}/${LAPPL})
target_link_libraries(${LPROGNAME} ${LDADD})
add_custom_command(TARGET ${LPROGNAME}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${SRCDIR}/../${CONFIG} ${CONFIG}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${SRCDIR}/../${CLIENTS} ${CLIENTS}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${SRCDIR}/../${PREDEFTOPIC} ${PREDEFTOPIC}
        )

add_executable(${TESTPROGNAME} ${CCPSRCS} ${CSRCS} ${SRCDIR}/${TEST}/${TESTAPPL})
target_link_libraries(${TESTPROGNAME} ${LDADD})



#set_target_properties(${PROGNAME} PROPERTIES
#        RUNTIME_OUTPUT_DIRECTORY ${OUTDIR}
#        )

#set_target_properties(${LPROGNAME} PROPERTIES
#        RUNTIME_OUTPUT_DIRECTORY ${OUTDIR}
#        )

#set_target_properties(${TESTPROGNAME} PROPERTIES
#        RUNTIME_OUTPUT_DIRECTORY ${OUTDIR}
#        )


# install MQTT-SNGateway, MQTT-SNLogmonitor to INSTALL_DIR and the config-files to CONFIG_DIR
#SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
#install(TARGETS ${PROGNAME} ${LPROGNAME} DESTINATION ${INSTALL_DIR})

#add_custom_command(TARGET ${PROGNAME}
#        COMMAND ${CMAKE_COMMAND} -E copy ${CONFIG} ${CONFIG_DIR}
#        COMMAND ${CMAKE_COMMAND} -E copy ${CLIENTS} ${CONFIG_DIR}
#        COMMAND ${CMAKE_COMMAND} -E copy ${PREDEFTOPIC} ${CONFIG_DIR}
#        )

SET(CLIENT_VERSION ${PAHO_VERSION_MAJOR}.${PAHO_VERSION_MINOR}.${PAHO_VERSION_PATCH})

set(PAHO_PACKET_SAMPLE_PATH paho.mqtt-sn-embedded-c-c9e807da319bd68bff71cc4817ea5a4f4b25f49d/MQTTSNPacket/samples)
set(PAHO_PACKET_SRC_PATH paho.mqtt-sn-embedded-c-c9e807da319bd68bff71cc4817ea5a4f4b25f49d/MQTTSNPacket/src)
set(PAHO_PACKET_TEST_PATH paho.mqtt-sn-embedded-c-c9e807da319bd68bff71cc4817ea5a4f4b25f49d/MQTTSNPacket/test)

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

add_executable(Pahotest1 ../testclients/pahotest1main.c)
target_link_libraries(Pahotest1 LibPahotest1)

ADD_EXECUTABLE(LibPahoqos-1pub ${PAHO_PACKET_SAMPLE_PATH}/qos-1pub.c ${PAHO_PACKET_SAMPLE_PATH}/transport.c)
target_include_directories(LibPahoqos-1pub PRIVATE ${PAHO_PACKET_SRC_PATH})
TARGET_LINK_LIBRARIES(LibPahoqos-1pub LibPahoMQTTSNPacketClient)

# paho.mqtt-sn-embedded-c-c9e807da319bd68bff71cc4817ea5a4f4b25f49d/MQTTSNGateway/src
SET(PAHO_GATEWAY_SRC_FILES
        paho.mqtt-sn-embedded-c-c9e807da319bd68bff71cc4817ea5a4f4b25f49d/MQTTSNGateway/src)

#add_subdirectory(paho.mqtt.c-d631449f16ec78c37e063c220eb6cfc546a4facb)
add_subdirectory(paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01)
