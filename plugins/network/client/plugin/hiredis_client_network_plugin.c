//
// Created by SomeDude on 15.04.2019.
//

#include "hiredis_client_network_plugin.h"
#include "../../shared/plugin/plugin_json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *client_plugin_get_short_network_protocol_name() {
  return "redis";
}
uint16_t client_plugin_get_maximum_message_length() {
  return 1024;
}

int client_plugin_network_init(const client_plugin_config *cfg, void **plugin_context) {
  hiredis_client_context *context = calloc(1, sizeof(hiredis_client_context));
  if (context == NULL) {
    printf("Can't allocate redis_client_context\n");
    return -1;
  }
  context->redis_contest = 0;
  context->redis_contest = NULL;
  context->redis_send_list = strdup("sl");
  context->redis_receive_list = strdup("rl");
  *plugin_context = context;
  return 0;
}

int client_plugin_network_deinit(const client_plugin_config *cfg, void **plugin_context) {
  hiredis_client_context *context = (hiredis_client_context *) *plugin_context;
  free(context->redis_send_list);
  free(context->redis_receive_list);
  free(context);
  *plugin_context = NULL;
  context->status = -1;
  return 0;
}

int client_plugin_network_connect(const client_plugin_config *cfg, void *plugin_context) {
  hiredis_client_context *context = (hiredis_client_context *) plugin_context;
  if (context->status < 1) {
    return -1;
  }
  if (cfg->client_plugin_device_address_length < 6) {
    return -1;
  }
  const char *max_ipv4_str = "255.255.255.255";
  uint16_t max_ipv4_str_length = sizeof(max_ipv4_str);
  char ip_str[max_ipv4_str_length];
  memset(ip_str, 0, max_ipv4_str_length);

  snprintf(ip_str, max_ipv4_str_length, "%d.%d.%d.%d",
           cfg->forwarder_client_network_address->bytes[0],
           cfg->forwarder_client_network_address->bytes[1],
           cfg->forwarder_client_network_address->bytes[2],
           cfg->forwarder_client_network_address->bytes[3]);

  // TODO conver device address to string
  context->redis_contest = redisConnect("127.0.0.1", 6379);
  if (context->redis_contest == NULL || context->redis_contest->err) {
    if (context->redis_contest) {
      printf("Error: %s\n", context->redis_contest->errstr);
    } else {
      printf("Can't allocate redis context\n");
    }
    // free context is done in deinit
    return -1;
  }
  return 0;
}

void client_plugin_network_disconnect(const client_plugin_config *cfg, void *plugin_context) {
  hiredis_client_context *context = (hiredis_client_context *) plugin_context;
  redisFree(context->redis_contest);
  context->status = -1;
}

int client_plugin_network_send(const client_plugin_message *send_message,
                               int timeout_ms,
                               const client_plugin_config *cfg,
                               void *plugin_context) {
  hiredis_client_context *context = (hiredis_client_context *) plugin_context;

  char *json_device_address =
      generate_json_from_device_address(send_message->address.bytes, send_message->address.length);
  if (json_device_address == NULL) {
    printf("Can't create json_device_address\n");
    return -1;
  }

  char *json_data = generate_json_from_data(send_message->data, *send_message->data_length);
  if (json_data == NULL) {
    free(json_device_address);
    printf("Can't create json_data\n");
    return -1;
  }

  redisReply *reply = redisC

  mmand(context->redis_contest,
        "LPUSH %s {%s,%s}",
        context->redis_send_list,
        json_device_address,
        json_data);

  free(json_device_address);
  free(json_data);

  if (reply == NULL) {
    freeReplyObject(reply);
    printf("Error reply\n");
    // TODO errorhandling of reply
    return -1;
  }
  // TODO check if reply: (integer) 1
  // printf("LPUSH: %s\n", reply->str);
  freeReplyObject(reply);

  return *send_message->data_length;
}

int client_plugin_network_receive(client_plugin_message *rec_message,
                                  int timeout_ms,
                                  const client_plugin_config *cfg,
                                  void *plugin_context) {

  hiredis_client_context *context = (hiredis_client_context *) plugin_context;
  redisReply *reply = redisCommand(context->redis_contest, "RPOP %s", context->redis_receive_list);
  if (reply == NULL) {
    if (context->redis_contest) {
      printf("Error: %s\n", context->redis_contest->errstr);
    } else {
      printf("Can't allocate redis context\n");
    }
    freeReplyObject(reply);
    return -1;
  }
  if (reply->type == REDIS_REPLY_NIL) {
    // nothing received
    freeReplyObject(reply);
    return 0;
  }
  if (reply->type != REDIS_REPLY_STRING) {
    // this should never happen
    freeReplyObject(reply);
    return -1;
  }

  uint16_t address_length = 0;
  if (parse_device_address_from_json(rec_message->address.bytes,
                                     &address_length,
                                     cfg->client_plugin_device_address_length,
                                     reply->str,
                                     reply->len)) {
    freeReplyObject(reply);
    return 0;
  }
  rec_message->address.length = address_length;

  if (parse_data_from_json(rec_message->data,
                           rec_message->data_length,
                           cfg->forwarder_maximum_message_length,
                           reply->str,
                           reply->len)) {
    freeReplyObject(reply);
    return 0;
  }

  freeReplyObject(reply);

  return 1;
}
