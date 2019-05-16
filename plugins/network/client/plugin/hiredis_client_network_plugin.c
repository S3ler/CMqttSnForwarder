//
// Created by SomeDude on 15.04.2019.
//

#include "hiredis_client_network_plugin.h"
#include "../../shared/plugin/plugin_json.h"
#include "../../shared/plugin/plugin_device_address_converter.h"
#include "../../../../forwarder/network/client/plugin/client_network_plugin_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *client_plugin_get_short_network_protocol_name() {
  return "redis";
}

uint16_t client_plugin_get_maximum_message_length() {
  return 1024;
}

int client_plugin_network_initialize(const client_plugin_config *cfg, void **plugin_context) {
  hiredis_client_context *context = calloc(1, sizeof(hiredis_client_context));
  if (context == NULL) {
    printf("Can't allocate redis_client_context\n");
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

void client_plugin_network_deinitialize(const client_plugin_config *cfg, void **plugin_context) {
  hiredis_client_context *context = (hiredis_client_context *) *plugin_context;
  free(context->redis_send_list);
  free(context->redis_receive_list);
  free(*plugin_context);
  *plugin_context = NULL;
}

int client_plugin_network_connect(const client_plugin_config *cfg, void *plugin_context) {
  hiredis_client_context *context = (hiredis_client_context *) plugin_context;
  if (context->status != 0) {
    return -1;
  }
  if (cfg->client_plugin_device_address_length < 6) {
    return -1;
  }

  char ipv4_str[] = {"255.255.255.255"};
  uint16_t ipv4_str_length = strlen(ipv4_str) + 1;
  memset(ipv4_str, 0, ipv4_str_length);
  uint16_t port = 0;
  if (get_ipv4_string_and_port_from_device_address(ipv4_str,
                                                   ipv4_str_length,
                                                   &port,
                                                   cfg->forwarder_client_network_address->bytes,
                                                   cfg->forwarder_client_network_address->length)) {
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

void client_plugin_network_disconnect(const client_plugin_config *cfg, void *plugin_context) {
  hiredis_client_context *context = (hiredis_client_context *) plugin_context;
  redisFree(context->redis_context);
  context->redis_context = NULL;
  context->status = 0;
}

int client_plugin_network_send(const client_plugin_message *send_message,
                               int timeout_ms,
                               const client_plugin_config *cfg,
                               void *plugin_context) {
  hiredis_client_context *context = (hiredis_client_context *) plugin_context;

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

  char *json_device_address =
      generate_json_from_device_address(send_message->address.bytes, send_message->address.length);
  if (json_device_address == NULL) {
#ifdef WITH_PLUGIN_ERROR_MSG
    printf("Can't create json_device_address\n");
#endif
    return -1;
  }

  char *json_data = generate_json_from_data(send_message->data, *send_message->data_length);
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

  return *send_message->data_length;
}

int client_plugin_network_receive(client_plugin_message *rec_message,
                                  int timeout_ms,
                                  const client_plugin_config *cfg,
                                  void *plugin_context) {
  hiredis_client_context *context = (hiredis_client_context *) plugin_context;

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

  uint16_t address_length = 0;
  if (parse_device_address_from_json(rec_message->address.bytes,
                                     &address_length,
                                     cfg->client_plugin_device_address_length,
                                     reply->str,
                                     reply->len)) {
#ifdef WITH_PLUGIN_ERROR_MSG
    printf("Can't parse device_address from json\n");
#endif
    freeReplyObject(reply);
    return 0;
  }
  rec_message->address.length = address_length;

  if (parse_data_from_json(rec_message->data,
                           rec_message->data_length,
                           cfg->forwarder_maximum_message_length,
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
