//
// Created by SomeDude on 16.04.2019.
//
#include <errno.h>
#include "plugin_json.h"


char *generate_json_from_device_address(const uint8_t *src_device_address, const uint16_t src_device_address_len) {
  const char *device_address_key_str = "device_address";
  char *dst = NULL;
  uint32_t dst_len = 0;
  if (malloc_json_key_array(&dst, &dst_len, device_address_key_str, src_device_address_len)) {
    return NULL;
  }
  if (generate_json_array_from_uint8_array(device_address_key_str,
                                           dst,
                                           dst_len,
                                           src_device_address,
                                           src_device_address_len)) {
    free(dst);
    return NULL;
  }
  return dst;
}

void free_json_device_address(char* json_device_address){
  free(json_device_address);
}

char *generate_json_from_data(uint8_t *src_data, uint16_t src_data_len) {
  const char *device_address_key_str = "data";
  char *dst = NULL;
  uint32_t dst_len = 0;
  if (malloc_json_key_array(&dst, &dst_len, device_address_key_str, src_data_len)) {
    return NULL;
  }
  if (generate_json_array_from_uint8_array(device_address_key_str, dst, dst_len, src_data, src_data_len)) {
    free(dst);
    return NULL;
  }
  return dst;
}

void free_json_data(char* json_data){
  free(json_data);
}

int malloc_json_key_array(char **dst, uint32_t *dst_len, const char *key, uint16_t d_a_len) {
  const char *array_symbols_str = "\"\"=[]";
  const char *max_number_str = "255,";
  *dst_len = strlen(key) + strlen(array_symbols_str) + strlen(max_number_str) * d_a_len + 1;
  *dst = malloc(*dst_len);
  if (*dst == NULL) {
    *dst_len = 0;
    return -1;
  }
  memset(*dst, 0, *dst_len);
  return 0;
}

int generate_json_array_from_uint8_array(const char *key,
                                         char *dst,
                                         uint32_t dst_len,
                                         const uint8_t *src_d_a,
                                         const uint16_t srd_d_a_len) {
  uint32_t used_bytes = 0;
  const char *array_key_start_str = "\"\"=[";
  uint32_t array_key_start_str_len = strlen(array_key_start_str) + strlen(key) + 1;
  char *key_str = malloc(array_key_start_str_len);
  if (key_str == NULL) {
    return -1;
  }
  memset(key_str, 0, array_key_start_str_len);
  snprintf(key_str, array_key_start_str_len, "\"%s\"=[", key);
  used_bytes += strlen(key_str);
  if (dst_len < used_bytes) {
    return -1;
  }
  strcat(dst, key_str);
  free(key_str);

  for (uint8_t i = 0; i < srd_d_a_len; ++i) {
    char number_str[4 + 1] = {0};
    if (i + 1 != srd_d_a_len) {
      snprintf(number_str, 4 + 1, "%d,", src_d_a[i]);
    } else {
      snprintf(number_str, 4 + 1, "%d", src_d_a[i]);
    }
    used_bytes += strlen(number_str);
    if (dst_len < used_bytes) {
      return -1;
    }
    strcat(dst, number_str);
  }
  const char *close_bracket = "]";
  used_bytes += strlen(close_bracket);
  if (dst_len < used_bytes) {
    return -1;
  }
  strcat(dst, close_bracket);
  return 0;
}

int parse_device_address_from_json(uint8_t *device_address,
                                   uint16_t *device_address_len,
                                   uint8_t max_device_address_len,
                                   const char *json_str,
                                   size_t json_str_len) {
  const char *key = "device_address";
  return parse_uint8_array_from_json(key,
                                     device_address,
                                     device_address_len,
                                     max_device_address_len,
                                     json_str,
                                     json_str_len);
}

int parse_data_from_json(uint8_t *data,
                         uint16_t *data_len,
                         uint16_t max_data_len,
                         const char *json_str,
                         size_t json_str_len) {
  const char *key = "data";
  return parse_uint8_array_from_json(key,
                                     data,
                                     data_len,
                                     max_data_len,
                                     json_str,
                                     json_str_len);
}

int parse_uint8_array_from_json(const char *key,
                                uint8_t *data,
                                uint16_t *data_len,
                                uint16_t max_data_len,
                                const char *json_str,
                                size_t json_str_len) {
  if (strlen(json_str) != json_str_len) {
    return -1;
  }

  const char *array_key_start_str = "\"\"=[";
  uint32_t array_key_start_str_len = strlen(array_key_start_str) + strlen(key) + 1;
  char *key_str = malloc(array_key_start_str_len);
  if (key_str == NULL) {
    return -1;
  }
  memset(key_str, 0, array_key_start_str_len);
  snprintf(key_str, array_key_start_str_len, "\"%s\"=[", key);

  const char *start_key_str = strstr(json_str, key_str);
  free(key_str);
  if (start_key_str == NULL) {
    // key does not exist
    return -1;
  }

  const char *array_start = strchr(start_key_str, '[');
  if (array_start == NULL) {
    return -1;
  }
  const char *array_end = strchr(start_key_str, ']');
  if (array_end == NULL) {
    return -1;
  }
  size_t array_len = (array_end + 1) - (array_start + 1);
  char *array_cpy = calloc(1, array_len);
  if (array_cpy == NULL) {
    return -1;
  }
  memcpy(array_cpy, array_start + 1, array_len - 1);

  int rc = 0;
  char *tk = strtok(array_cpy, ", ");
  while (tk != NULL) {
    char *end_ptr;
    long int n = strtol(tk, &end_ptr, 10);
    if (errno == EOVERFLOW) {
      rc = -1;
      break;
    }
    if (*end_ptr != '\0') {
      // no conversion performed
      rc = -1;
      break;
    }
    if (*data_len + 1 > max_data_len) {
      rc = -1;
      break;
    }
    data[*data_len] = n;
    *data_len += 1;
    tk = strtok(NULL, ", ");
  }

  free(array_cpy);
  return rc;
}
