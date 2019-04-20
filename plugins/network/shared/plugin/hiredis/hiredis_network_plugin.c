//
// Created by SomeDude on 19.04.2019.
//

#include "hiredis_network_plugin.h"
#include "../plugin_device_address_converter.h"
#include "../plugin_json.h"

const char *hiredis_plugin_get_short_network_protocol_name() {
  return "redis";
}

uint16_t hiredis_plugin_get_maximum_message_length() {
  return 1024;
}

int hiredis_gateway_plugin_network_init(void **plugin_context) {
  hiredis_context *context = calloc(1, sizeof(hiredis_context));
  if (context == NULL) {
    printf("Can't allocate hiredis_context\n");
    return -1;
  }
  context->redis_context = 0;
  context->redis_context = NULL;
  context->redis_send_list = NULL;
  context->redis_receive_list = NULL;
  context->redis_send_list = strdup("gsl");
  context->redis_receive_list = strdup("grl");
  *plugin_context = context;
  return 0;
}

int hiredis_client_plugin_network_init(void **plugin_context) {
  hiredis_context *context = calloc(1, sizeof(hiredis_context));
  if (context == NULL) {
    printf("Can't allocate hiredis_context\n");
    return -1;
  }
  context->redis_context = 0;
  context->redis_context = NULL;
  context->redis_send_list = NULL;
  context->redis_receive_list = NULL;
  context->redis_send_list = strdup("csl");
  context->redis_receive_list = strdup("crl");
  *plugin_context = context;
  return 0;
}

void hiredis_plugin_network_disconnect(void *plugin_context) {
  hiredis_context *context = (hiredis_context *) plugin_context;
  redisFree(context->redis_context);
  context->redis_context = NULL;
  context->status = 0;
}

void hiredis_plugin_network_deinit(void **plugin_context) {
  hiredis_context *context = (hiredis_context *) plugin_context;
  free(context->redis_send_list);
  free(context->redis_receive_list);
  free(context);
  *plugin_context = NULL;
  context->status = -1;
}

int hiredis_plugin_network_connect(const uint8_t* address,
                                   uint16_t address_len,
                                   uint16_t network_max_device_address_length,
                                   void *plugin_context) {
  hiredis_context *context = (hiredis_context *) plugin_context;
  if (context->status != 0) {
    return -1;
  }
  if (network_max_device_address_length < 6) {
    return -1;
  }

  char ipv4_str[] = {"255.255.255.255"};
  uint16_t ipv4_str_length = strlen(ipv4_str) + 1;
  memset(ipv4_str, 0, ipv4_str_length);
  uint16_t port = 0;
  if (get_ipv4_string_and_port_from_device_address(ipv4_str,
                                                   ipv4_str_length,
                                                   &port,
                                                   address,
                                                   address_len)) {
    return -1;
  }

  context->redis_context = redisConnect(ipv4_str, port);
  if (context->redis_context == NULL || context->redis_context->err) {
#ifdef WITH_PLUGIN_ERROR_MSG
    if (context->redis_context) {
      fprintf(stderr, "Error: %s\n", context->redis_context->errstr);
    } else {
      fprintf(stderr, "Can't allocate redis context\n");
    }
#endif
    return -1;
  }
  return 0;
}

int hiredis_plugin_network_send(const uint8_t *to,
                                uint16_t to_len,
                                uint16_t network_max_device_address_length,
                                const uint8_t *data,
                                uint16_t data_len,
                                uint16_t network_max_message_length,
                                int timeout_ms,
                                void *plugin_context) {
  hiredis_context *context = (hiredis_context *) plugin_context;

  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }

  if (redisSetTimeout(context->redis_context, interval)) {
#ifdef WITH_PLUGIN_ERROR_MSG
    fprintf(stderr, "Error: %s\n", context->redis_context->errstr);
#endif
    return -1;
  }

  char *json_device_address = generate_json_from_device_address(to, to_len);
  if (json_device_address == NULL) {
#ifdef WITH_PLUGIN_ERROR_MSG
    printf("Can't create json_device_address\n");
#endif
    return -1;
  }

  char *json_data = generate_json_from_data(data, data_len);
  if (json_data == NULL) {
    free_json_device_address(json_device_address);
#ifdef WITH_PLUGIN_ERROR_MSG
    printf("Can't create json_data\n");
#endif
    return -1;
  }

  redisReply *reply = redisCommand(context->redis_context,
                                   "LPUSH %s {%s,%s}",
                                   context->redis_send_list,
                                   json_device_address,
                                   json_data);
  free_json_device_address(json_device_address);
  free_json_data(json_data);

  if (reply == NULL) {
#ifdef WITH_PLUGIN_ERROR_MSG
    if (context->redis_context) {
      printf("Error: %s\n", context->redis_context->errstr);
    } else {
      printf("Can't allocate redis reply\n");
    }
#endif
    freeReplyObject(reply);
    return -1;
  }

  freeReplyObject(reply);

  if (reply->type == REDIS_REPLY_ERROR) {
#ifdef WITH_PLUGIN_ERROR_MSG
    fprintf(stderr, "Error: %s\n", context->redis_context->errstr);
#endif
    return -1;
  }

  return data_len;
}

int hiredis_plugin_network_receive(uint8_t *from,
                                   uint16_t *from_len,
                                   uint16_t network_max_device_address_length,
                                   uint8_t *data,
                                   uint16_t *data_len,
                                   uint16_t network_max_data_length,
                                   int timeout_ms,
                                   void *plugin_context) {
  hiredis_context *context = (hiredis_context *) plugin_context;

  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0)) {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }

  if (redisSetTimeout(context->redis_context, interval)) {
#ifdef WITH_PLUGIN_ERROR_MSG
    fprintf(stderr, "Error: %s\n", context->redis_context->errstr);
#endif
    return -1;
  }

  redisReply *reply = redisCommand(context->redis_context, "RPOP %s", context->redis_receive_list);

  if (reply == NULL) {
#ifdef WITH_PLUGIN_ERROR_MSG
    if (context->redis_context) {
      printf("Error: %s\n", context->redis_context->errstr);
    } else {
      printf("Can't allocate redis reply\n");
    }
#endif
    freeReplyObject(reply);
    return -1;
  }

  if (reply->type == REDIS_REPLY_NIL) {
    // nothing received
    freeReplyObject(reply);
    return 0;
  }
  if (reply->type != REDIS_REPLY_STRING) {
#ifdef WITH_PLUGIN_ERROR_MSG
    printf("Can't redis reply is no string\n");
#endif
    freeReplyObject(reply);
    return -1;
  }

  if (parse_device_address_from_json(from,
                                     from_len,
                                     network_max_device_address_length,
                                     reply->str,
                                     reply->len)) {
#ifdef WITH_PLUGIN_ERROR_MSG
    printf("Can't parse device_address from json\n");
#endif
    freeReplyObject(reply);
    return 0;
  }

  if (parse_data_from_json(data,
                           data_len,
                           network_max_data_length,
                           reply->str,
                           reply->len)) {
#ifdef WITH_PLUGIN_ERROR_MSG
    printf("Can't parse data from json\n");
#endif
    freeReplyObject(reply);
    return 0;
  }

  freeReplyObject(reply);
  return 1;
}
