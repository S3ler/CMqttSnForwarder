//
// Created by SomeDude on 22.07.2019.
//

#include "MqttClientTestContainerConnectOptions.h"
#include "../../../../../../.CLion2019.1/system/.remote/homeserver-NUC6CAYH_22/3e948abc-604f-4868-a00f-a633497b3769/usr/include/c++/7/string"
#include "../../../../../../.CLion2019.1/system/.remote/homeserver-NUC6CAYH_22/3e948abc-604f-4868-a00f-a633497b3769/usr/include/c++/7/cstdint"
#include "../../../../../../.CLion2019.1/system/.remote/homeserver-NUC6CAYH_22/3e948abc-604f-4868-a00f-a633497b3769/usr/lib/gcc/x86_64-linux-gnu/7/include/stdint-gcc.h"
MqttClientTestContainerConnectOptions::MqttClientTestContainerConnectOptions(const std::string &broker_uri,
                                                                             const std::string &client_id,
                                                                             const std::string &client_password,
                                                                             const int32_t &keep_alive_interval,
                                                                             const constbool &clean_session,
                                                                             const std::string &will_topic,
                                                                             const std::string &will_message,
                                                                             const int32_t &will_qos)
    : brokerURI(broker_uri),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveIntervall(keep_alive_interval),
      cleanSession(clean_session),
      willTopic(will_topic),
      willMessage(will_message),
      willQoS(will_qos) {}
