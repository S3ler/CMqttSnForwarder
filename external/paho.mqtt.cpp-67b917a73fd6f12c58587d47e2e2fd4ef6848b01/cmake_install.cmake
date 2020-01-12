# Install script for directory: /home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/samples" TYPE FILE FILES
    "/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/samples/MQTTAsync_publish.c"
    "/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/samples/MQTTAsync_subscribe.c"
    "/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/samples/MQTTClient_publish.c"
    "/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/samples/MQTTClient_publish_async.c"
    "/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/samples/MQTTClient_subscribe.c"
    "/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/samples/paho_c_pub.c"
    "/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/samples/paho_c_sub.c"
    "/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/samples/paho_cs_pub.c"
    "/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/samples/paho_cs_sub.c"
    "/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/samples/pubsub_opts.c"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE FILES
    "/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/CONTRIBUTING.md"
    "/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/epl-v10"
    "/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/edl-v10"
    "/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/README.md"
    "/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/notice.html"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/src/cmake_install.cmake")
  include("/home/bele/CMqttSnForwarder/external/paho.mqtt.cpp-67b917a73fd6f12c58587d47e2e2fd4ef6848b01/test/cmake_install.cmake")

endif()

