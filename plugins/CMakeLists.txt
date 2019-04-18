cmake_minimum_required(VERSION 3.10)
project(CHiredisNetworkPlugin C)

set(CMAKE_C_STANDARD 99)

set(HIREDIS_NETWORK_PLUGIN_FILES
        network/client/plugin/client_network_plugin_interface.h
        network/client/plugin/hiredis_client_network_plugin.c
        network/client/plugin/hiredis_client_network_plugin.h
        network/gateway/plugin/gateway_plugin_interface.h
        network/gateway/plugin/hiredis_gateway_network_plugin.c
        network/shared/plugin/plugin_json.c
        network/shared/plugin/plugin_json.h)

find_library(hiredis /usr/local/)

add_library(hiredis_plugin SHARED ${HIREDIS_NETWORK_PLUGIN_FILES})
target_include_directories(hiredis_plugin PRIVATE external/hiredis)
target_include_directories(hiredis_plugin PRIVATE /usr/local/include/hiredis)
target_link_libraries(hiredis_plugin hiredis)
