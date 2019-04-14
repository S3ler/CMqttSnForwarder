//
// Created by SomeDude on 11.04.2019.
//

#include "MqttSnForwarderLoggingBasic.h"
#include <string.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <stdint.h>
#include <time.h>

int log_int8(const MqttSnLogger *logger, int8_t n) {
  const char *int8_max_str = "-128";
  int int8_max_str_length = strlen(int8_max_str);
  char n_str[int8_max_str_length + 1];
  snprintf((char *) &n_str, int8_max_str_length + 1, "%d", n);
  return log_str(logger, n_str);
}

int log_int16(const MqttSnLogger *logger, int16_t n) {
  const char *int16_max_str = "-32766";
  int int16_max_str_length = strlen(int16_max_str);
  char n_str[int16_max_str_length + 1];
  snprintf((char *) &n_str, int16_max_str_length + 1, "%d", n);
  return log_str(logger, n_str);
}

int log_int32(const MqttSnLogger *logger, int32_t n) {
  const char *int32_max_str = "-2147483646";
  int int32_max_str_length = strlen(int32_max_str);
  char n_str[int32_max_str_length + 1];
  snprintf((char *) &n_str, int32_max_str_length + 1, "%d", n);
  return log_str(logger, n_str);
}

int log_int64(const MqttSnLogger *logger, int64_t n) {
  const char *int64_max_str = "-9223372036854775806";
  int int64_max_str_length = strlen(int64_max_str);
  char n_str[int64_max_str_length + 1];
  snprintf((char *) &n_str, int64_max_str_length + 1, "%li", n);
  return log_str(logger, n_str);
}

int log_uint8(const MqttSnLogger *logger, uint8_t n) {
  const char *uint8_max_str = "255";
  int uint8_max_str_length = strlen(uint8_max_str);
  char n_str[uint8_max_str_length + 1];
  snprintf((char *) &n_str, uint8_max_str_length + 1, "%u", n);
  return log_str(logger, n_str);
}

int log_uint16(const MqttSnLogger *logger, uint16_t n) {
  const char *uint16_max_str = "65535";
  int uint16_max_str_length = strlen(uint16_max_str);
  char n_str[uint16_max_str_length + 1];
  snprintf((char *) &n_str, uint16_max_str_length + 1, "%u", n);
  return log_str(logger, n_str);
}

int log_uint32(const MqttSnLogger *logger, uint32_t n) {
  const char *uint32_max_str = "4294967295";
  int uint32_max_str_length = strlen(uint32_max_str);
  char n_str[uint32_max_str_length + 1];
  snprintf((char *) &n_str, uint32_max_str_length + 1, "%u", n);
  return log_str(logger, n_str);
}

int log_uint64(const MqttSnLogger *logger, uint64_t n) {
  const char *uint64_str = "18446744073709551615";
  int uint64_str_length = strlen(uint64_str);
  char t_str[uint64_str_length + 1];
  snprintf((char *) &t_str, uint64_str_length + 1, "%ju", n);
  return log_str(logger, t_str);
}

int log_uintmax(const MqttSnLogger *logger, uintmax_t n) {
  const char *uintmax_str = "18446744073709551615"; // uintmax_t 18446744073709551615
  int uintmax_str_length = strlen(uintmax_str);
  char t_str[uintmax_str_length + 1];
  snprintf((char *) &t_str, uintmax_str_length + 1, "%ju", n);
  return log_str(logger, t_str);
}

int log_char_key_int8_value(const MqttSnLogger *logger, char key, int8_t val) {
  log_char(logger, key);
  return log_int8(logger, val);
}

int log_char_key_uint8_value(const MqttSnLogger *logger, char key, uint8_t val) {
  log_char(logger, key);
  return log_uint8(logger, val);
}

int log_char_key_uint16_value(const MqttSnLogger *logger, char key, uint16_t val) {
  log_char(logger, key);
  return log_uint16(logger, val);
}

int log_str_key_uint8_value(const MqttSnLogger *logger, const char *key, uint8_t val) {
  log_str(logger, key);
  return log_uint8(logger, val);
}

int log_str_key_uint16_value(const MqttSnLogger *logger, const char *key, uint16_t val) {
  log_str(logger, key);
  return log_uint16(logger, val);
}
int log_uint8_array(const MqttSnLogger *logger, const uint8_t *data, uint16_t data_length) {
  const char *comma = ", ";
  for (uint16_t i = 0; i < data_length; ++i) {
    if (log_uint8(logger, data[i])) {
      return -1;
    }
    if (i + 1 < data_length) {
      if (log_str(logger, comma)) {
        return -1;
      }
    }
  }
  return 0;
}

int get_timestamp(uint64_t *t) {
  time_t result = time(NULL);
  if (result == -1) {
    return -1;
  }
  *t = result;
  return 0;
}

int log_current_time(const MqttSnLogger *logger) {
  uintmax_t t;
  if (get_timestamp(&t)) {
    return -1;
  }
  return log_uintmax(logger, t);
}

int log_msg_start(const MqttSnLogger *logger) {
  if (log_current_time(logger)) { return -1; }
  const char *ds = ": ";
  return log_str(logger, ds);
}

int log_device_address(const MqttSnLogger *logger, const device_address *address) {
  const char *dot = ".";
  for (size_t i = 0; i < sizeof(device_address); ++i) {
    if (log_uint8(logger, address->bytes[i])) {
      return -1;
    }
    if (i + 1 < sizeof(device_address)) {
      if (log_str(logger, dot)) {
        return -1;
      }
    }
  }
  return 0;
}

