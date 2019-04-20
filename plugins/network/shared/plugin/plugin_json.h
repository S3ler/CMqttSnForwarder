//
// Created by SomeDude on 16.04.2019.
//

#ifndef CMQTTSNFORWARDER_PLUGINS_NETWORK_SHARED_PLUGIN_PLUGIN_JSON_H_
#define CMQTTSNFORWARDER_PLUGINS_NETWORK_SHARED_PLUGIN_PLUGIN_JSON_H_

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int malloc_json_key_array(char **dst, uint32_t *dst_len, const char *key, uint16_t d_a_len);
char *generate_json_from_data(uint8_t *src_data, uint16_t src_data_len);
char *generate_json_from_device_address(const uint8_t *src_device_address, const uint16_t src_device_address_len);
int generate_json_array_from_uint8_array(const char *key,
                                         char *dst,
                                         uint32_t dst_len,
                                         const uint8_t *src_d_a,
                                         const uint16_t srd_d_a_len);

int parse_device_address_from_json(uint8_t *device_address,
                                   uint16_t *device_address_len,
                                   uint8_t max_device_address_len,
                                   const char *json_str,
                                   size_t json_str_len);
int parse_data_from_json(uint8_t *data,
                         uint16_t *data_len,
                         uint16_t max_data_len,
                         const char *json_str,
                         size_t json_str_len);

int parse_uint8_array_from_json(const char *key,
                                uint8_t *data,
                                uint16_t *data_len,
                                uint16_t max_data_len,
                                const char *json_str,
                                size_t json_str_len);

void free_json_device_address(char *json_device_address);
void free_json_data(char *json_data);

#endif //CMQTTSNFORWARDER_PLUGINS_NETWORK_SHARED_PLUGIN_PLUGIN_JSON_H_
