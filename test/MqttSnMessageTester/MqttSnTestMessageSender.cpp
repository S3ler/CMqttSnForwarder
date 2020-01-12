#include <iostream>
#include <thread>
#include "MqttSnTestMessageSender.hpp"
#include "../../platform/MqttSnMessageData.h"

void MqttSnTestMessageSender::send_searchgw(uint8_t radius) {
  test_searchgw msg(radius);
  if (send((const uint8_t *) &msg, msg.length)) {
    std::cout << "sendto()Radius" << std::endl;
  }
}

void MqttSnTestMessageSender::send_connect(const char *client_id, uint16_t duration, bool clean_session, bool will) {
  test_connect msg(will, clean_session, PROTOCOL_ID, duration, client_id);
  if (send((const uint8_t *) &msg, msg.length)) {
    std::cout << "sendto()Publish" << std::endl;
  }

}

void MqttSnTestMessageSender::send_willtopic(const char *willtopic, uint8_t qos, bool retain) {
  test_willtopic msg((char *) willtopic, qos, retain);
  if (send((const uint8_t *) &msg, msg.length)) {
    std::cout << "sendto()Willtopic" << std::endl;
  }
}

void MqttSnTestMessageSender::send_willmsg(const uint8_t *willmsg, uint8_t willmsg_len) {
  test_willmsg msg(willmsg, willmsg_len);
  if (send((const uint8_t *) &msg, msg.length)) {
    std::cout << "sendto()Willtopic" << std::endl;
  }
}

void MqttSnTestMessageSender::send_publish(bool dup, int8_t qos, bool retain, bool short_topic, uint16_t topic_id,
                                           uint16_t msg_id, const uint8_t *data, uint8_t data_length) {
  test_publish msg(dup, qos, retain, short_topic, topic_id, msg_id, data, data_length);
  if (send((const uint8_t *) &msg, msg.length)) {
    std::cout << "sendto()Publish" << std::endl;
  }

}

void MqttSnTestMessageSender::send_puback(uint8_t topic_id, uint16_t msg_id, return_code_test return_code) {
  test_puback msg(topic_id, msg_id, return_code);
  if (send((const uint8_t *) &msg, msg.length)) {
    std::cout << "sendto()Puback" << std::endl;
  }
}

void MqttSnTestMessageSender::send_pubrec(uint16_t msg_id) {
  test_pubrec msg(msg_id);
  if (send((const uint8_t *) &msg, msg.length)) {
    std::cout << "sendto()Puback" << std::endl;
  }
}

void MqttSnTestMessageSender::send_pubrel(uint16_t msg_id) {
  test_pubrel msg(msg_id);
  if (send((const uint8_t *) &msg, msg.length)) {
    std::cout << "sendto()Puback" << std::endl;
  }
}

void MqttSnTestMessageSender::send_pubcomp(uint16_t msg_id) {
  test_pubcomp msg(msg_id);
  if (send((const uint8_t *) &msg, msg.length)) {
    std::cout << "sendto()Puback" << std::endl;
  }
}

void MqttSnTestMessageSender::send_register(uint16_t topic_id, uint16_t msg_id, const char *topic_name) {
  test_register msg(topic_id, msg_id, topic_name);
  if (send((const uint8_t *) &msg, msg.length)) {
    std::cout << "sendto()Register" << std::endl;
  }
}

void MqttSnTestMessageSender::send_subscribe(bool dup, uint8_t qos, bool retain, bool will, bool clean_session,
                                             topic_id_type_test topic_id_type,
                                             uint16_t msg_id, const char *topic_name, uint16_t topic_id) {
  test_subscribe msg(dup, qos, retain, will, clean_session, topic_id_type, msg_id, topic_name, topic_id);
  if (send((const uint8_t *) &msg, msg.length)) {
    std::cout << "sendto()Subscribe" << std::endl;
  }
}

void MqttSnTestMessageSender::send_unsubscribe(topic_id_type_test topic_id_type,
                                               uint16_t msg_id,
                                               const char *topic_name,
                                               uint16_t topic_id) {
  test_unsubscribe msg(topic_id_type, msg_id, topic_name, topic_id);
  if (send((const uint8_t *) &msg, msg.length)) {
    std::cout << "sendto()Unubscribe" << std::endl;
  }
}

void MqttSnTestMessageSender::send_disconnect() {
  this->send_disconnect(2);
}

void MqttSnTestMessageSender::send_disconnect(uint8_t length) {
  test_disconnect msg;
  msg.length = length;
  if (send((const uint8_t *) &msg, msg.length)) {
    std::cout << "sendto()Disconnect" << std::endl;
  }
}

void MqttSnTestMessageSender::send_disconnect(uint16_t duration, uint8_t length) {
  test_disconnect msg(duration);
  msg.length = length;

  if (send((const uint8_t *) &msg, msg.length)) {
    std::cout << "sendto()Disconnect" << std::endl;
  }
}

void MqttSnTestMessageSender::send_pingreq() {
  test_pingreq msg;
  if (send((const uint8_t *) &msg, msg.length)) {
    std::cout << "sendto()Pingreq" << std::endl;
  }
}

void MqttSnTestMessageSender::send_pingreq(const char *client_id) {
  test_pingreq_wakeup msg(client_id);
  if (send((const uint8_t *) &msg, msg.length)) {
    std::cout << "sendto()Pingreq" << std::endl;
  }
}

void MqttSnTestMessageSender::/**/send_pingreq(const char *client_id, uint8_t length) {
  test_pingreq_wakeup msg(client_id);
  msg.length = 2 + length;
  if (send((const uint8_t *) &msg, msg.length)) {
    std::cout << "sendto()Pingreq" << std::endl;
  }
}
bool MqttSnTestMessageSender::send(const uint8_t *msg, uint16_t msg_length) {
  if (tester == nullptr) {
    return false;
  }
  return !tester->send(msg, msg_length);
}
void MqttSnTestMessageSender::setTester(MqttSnMessageTester *tester) {
  MqttSnTestMessageSender::tester = tester;
}





