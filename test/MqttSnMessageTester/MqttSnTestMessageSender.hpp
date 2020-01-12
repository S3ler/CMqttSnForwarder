#ifndef MQTTSNGATEWAY_CLIENTFAKE_H
#define MQTTSNGATEWAY_CLIENTFAKE_H

#include <stdio.h>
#include "MqttSnMessageTester.hpp"
#include <string.h>
#include <network/MqttSnGatewayNetworkInterface.h>
#include <network/MqttSnClientNetworkInterface.h>
#include <ringbuffer/MqttSnFixedSizeRingBuffer.h>
#include <atomic>
#include <thread>
#include "MqttSnTestMessageReceiverInterface.hpp"

class MqttSnMessageTester;

class MqttSnTestMessageSender {
 private:
  bool send(const uint8_t *msg, uint16_t msg_length);
  MqttSnMessageTester *tester = nullptr;
 public:
  void setTester(MqttSnMessageTester *tester);
  // TODO void send_advertise();

  void send_searchgw(uint8_t radius);

  // TODO void send_gwinfo();

  void send_connect(const char *client_id, uint16_t duration, bool clean_session, bool will);

  // TODO void send_connack();

  // TODO void send_willtopicreq();

  void send_willtopic(const char *willtopic, uint8_t qos, bool retain);

  // TODO void send_willmsgreq();

  void send_willmsg(const uint8_t *willmsg, uint8_t willmsg_len);

  void send_register(uint16_t topic_id, uint16_t msg_id, const char *topic_name);

  // TODO void send_regack();

  void send_publish(bool dup, int8_t qos, bool retain, bool short_topic, uint16_t topic_id,
                    uint16_t msg_id, const uint8_t *data, uint8_t data_length);

  void send_puback(uint8_t topic_id, uint16_t msg_id, return_code_test return_code);

  void send_pubrec(uint16_t msg_id);

  void send_pubrel(uint16_t msg_id);

  void send_pubcomp(uint16_t msg_id);

  void send_subscribe(bool dup, uint8_t qos, bool retain, bool will, bool clean_session,
                      topic_id_type_test topic_id_type,
                      uint16_t msg_id, const char *topic_name, uint16_t topic_id);

  // TODO void send_suback();

  void send_unsubscribe(topic_id_type_test topic_id_type, uint16_t msg_id, const char *topic_name, uint16_t topic_id);

  // TODO void send_unsuback();

  void send_disconnect();

  void send_disconnect(uint8_t length);

  void send_disconnect(uint16_t duration, uint8_t length);

  // TODO void send_willtopicupd();

  // TODO send_willmsgupd();

  // TODO send_willmsgupd();

  // TODO send_willmsgrsp();

  void send_pingreq();

  void send_pingreq(const char *client_id);

  void send_pingreq(const char *client_id, uint8_t length);

  // TODO send_forwarder_encapsulation

  // TODO send_any_message()

};

#endif //MQTTSNGATEWAY_CLIENTFAKE_H
