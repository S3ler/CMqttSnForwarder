cmake_minimum_required(VERSION 3.10)
project(CHiredisNetworkPlugin C)

set(CMAKE_C_STANDARD 99)

set(HIREDIS_NETWORK_PLUGIN_FILES

        ../../forwarder/network/client/plugin/client_network_plugin_interface.h
        network/client/plugin/hiredis_client_network_plugin.c
        network/client/plugin/hiredis_client_network_plugin.h

        ../../forwarder/network/gateway/plugin/gateway_network_plugin_interface.h
        network/gateway/plugin/hiredis_gateway_network_plugin.c
        network/gateway/plugin/hiredis_gateway_network_plugin.h

        network/shared/plugin/hiredis/hiredis_network_plugin.c
        network/shared/plugin/hiredis/hiredis_network_plugin.h

        network/shared/plugin/plugin_device_address_converter.c
        network/shared/plugin/plugin_device_address_converter.h
        network/shared/plugin/plugin_json.c
        network/shared/plugin/plugin_json.h)

find_library(hiredis /usr/local/)
add_library(hiredis_plugin SHARED ${HIREDIS_NETWORK_PLUGIN_FILES})

set(CMQTTSNFORWADER__PLUGIN_ERROR_COMPILE_FLAGS -Wall -Wextra -pedantic -Werror
        -Wno-unused-parameter -Wno-missing-field-initializers -Wno-unused-value -Wno-unused-but-set-variable)

option(WITH_PLUGIN_ERROR_MSG "Include PLUGIN_ERROR_MSG support?" ON)
if (WITH_PLUGIN_ERROR_MSG)
    message("Building with WITH_PLUGIN_ERROR_MSG support")
    add_definitions(-DWITH_PLUGIN_ERROR_MSG)
endif ()

target_include_directories(hiredis_plugin PRIVATE external/hiredis)
target_include_directories(hiredis_plugin PRIVATE /usr/local/include/hiredis)
target_link_libraries(hiredis_plugin hiredis)
target_compile_options(hiredis_plugin PUBLIC ${CMQTTSNFORWADER__PLUGIN_ERROR_COMPILE_FLAGS})
