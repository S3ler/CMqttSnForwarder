//
// Created by SomeDude on 23.07.2019.
//

#include <memory>
#include <vector>
#include <test/MqttNetworkBuilder/MqttNetworkBuilder.h>
#include <test/MqttSnClient/MqttSnClientAction.h>
#include <test/MqttSnClient/MqttSnClientTestContainerInterface.h>
#include <test/MqttSnNetworkBuilder/MqttSnNetworkBuilder.h>
#include <logging/MqttSnLogging.h>

void configureMqttClients(int32_t rounds, const vector<std::shared_ptr<MqttClientTestContainerInterface>> &clients);
void configureMqttSnClients(int32_t rounds, const vector<std::shared_ptr<MqttSnClientTestContainerInterface>> &clients);

int main() {
    std::cout << "start" << std::endl << std::flush;

    int32_t mqttBrokerCount            = 1;
    int32_t mqttClientCount            = 0;
    int32_t mqttSnGatewayCount         = 1;
    int32_t mqttSnClientCount          = 1;
    int32_t mqttSnForwarderCount       = 0;
    int32_t mqttSnForwarderClientCount = 0;
    int32_t rounds                     = 1;

    auto mqttNetworkBuilder = std::make_shared<MqttNetworkBuilder>();

    for (int32_t i = 0; i < mqttBrokerCount; i++) {
        mqttNetworkBuilder->getMqttBroker();
    }

    for (int32_t i = 0; i < mqttClientCount; i++) {
        mqttNetworkBuilder->getMqttClient();
    }

    auto mqttSnNetworkBuilder = std::make_shared<MqttSnNetworkBuilder>(mqttNetworkBuilder.get());
    for (int32_t i = 0; i < mqttSnGatewayCount; i++) {
        mqttSnNetworkBuilder->getMqttSnGateway();
    }
    for (int32_t i = 0; i < mqttSnClientCount; i++) {
        mqttSnNetworkBuilder->getMqttSnClient();
    }

    for (int32_t i = 0; i < mqttSnForwarderCount; i++) {
        mqttSnNetworkBuilder->getMqttSnForwarder();
    }

    for (int32_t i = 0; i < mqttSnForwarderClientCount; i++) {
        mqttSnNetworkBuilder->getForwardedMqttSnClient();
    }

    if (!mqttNetworkBuilder->startMqttBrokers()) {
        throw std::exception();
    }
    if (!mqttSnNetworkBuilder->startMqttSnGateways()) {
        throw std::exception();
    }
    if (!mqttSnNetworkBuilder->startMqttSnForwarders()) {
        throw std::exception();
    }
    if (!mqttNetworkBuilder->startMqttClients()) {
        throw std::exception();
    }
    if (!mqttSnNetworkBuilder->startMqttSnClients()) {
        throw std::exception();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    uint64_t setup_ts_s;
    get_timestamp_s(&setup_ts_s);

    std::cout << "setup" << std::endl << std::flush;
    configureMqttClients(rounds, mqttNetworkBuilder->getMqttClients());
    configureMqttSnClients(rounds, mqttSnNetworkBuilder->getMqttSnClients());

    mqttNetworkBuilder->execMqttClientsActionSequenceAsync();
    mqttSnNetworkBuilder->execMqttSnClientsActionSequenceAsync();

    //while(true){
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    //}
    std::tuple<int, int> mqttClientsRunningFinished   = mqttNetworkBuilder->getMqttClientsActionsSequenceExecutingFinished();
    std::tuple<int, int> mqttSnClientsRunningFinished = mqttSnNetworkBuilder->getMqttSnClientsActionsSequenceExecutingFinished();
    while (std::get<0>(mqttClientsRunningFinished) != 0 || std::get<0>(mqttSnClientsRunningFinished) != 0) {
        std::cout << "Mqtt Clients running:    " << std::to_string(std::get<0>(mqttClientsRunningFinished))
                  << " finished: " << std::to_string(std::get<1>(mqttClientsRunningFinished)) << std::endl;
        std::cout << "Mqtt-Sn Clients running: " << std::to_string(std::get<0>(mqttSnClientsRunningFinished))
                  << " finished: " << std::to_string(std::get<1>(mqttSnClientsRunningFinished)) << std::endl;
        std::cout << std::flush;
        uint64_t current_ts_s;
        get_timestamp_s(&current_ts_s);
        std::cout << "Current Test duration: " << std::to_string(current_ts_s - setup_ts_s) << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        mqttClientsRunningFinished   = mqttNetworkBuilder->getMqttClientsActionsSequenceExecutingFinished();
        mqttSnClientsRunningFinished = mqttSnNetworkBuilder->getMqttSnClientsActionsSequenceExecutingFinished();
    }

    std::cout << "Mqtt Clients running:    " << std::to_string(std::get<0>(mqttClientsRunningFinished))
              << " finished: " << std::to_string(std::get<1>(mqttClientsRunningFinished)) << std::endl;
    std::cout << "Mqtt-Sn Clients running: " << std::to_string(std::get<0>(mqttSnClientsRunningFinished))
              << " finished: " << std::to_string(std::get<1>(mqttSnClientsRunningFinished)) << std::endl;
    std::cout << std::flush;
    std::cout << "end" << std::endl << std::flush;

    std::cout << "teardown" << std::endl << std::flush;

    std::cout << "end" << std::endl << std::flush;

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::flush;

    uint64_t current_ts_s;
    get_timestamp_s(&current_ts_s);
    std::cout << std::endl;
    std::cout << "Total Test duration: " << std::to_string(current_ts_s - setup_ts_s) << " s" << std::endl;

    std::cout << MqttClientActionResult::GetCsvColumns() << std::endl;
    for (const auto &client : mqttNetworkBuilder->getMqttClients()) {
        for (const auto &action_result : client->GetResult()) {
            std::cout << action_result << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << MqttSnClientActionResult::GetCsvColumns() << std::endl;
    for (const auto &client : mqttSnNetworkBuilder->getMqttSnClients()) {
        for (const auto &action_result : client->GetResult()) {
            std::cout << action_result << std::endl;
        }
    }
    std::cout << std::endl;
    std::cout << std::endl;

    mqttNetworkBuilder->stopMqttClients();
    mqttSnNetworkBuilder->stopMqttSnClients();

    mqttSnNetworkBuilder->stopMqttSnForwarders();
    mqttSnNetworkBuilder->stopMqttSnGateways();
    mqttNetworkBuilder->stopMqttBrokers();

    return 0;
}
void configureMqttClients(int32_t rounds, const vector<std::shared_ptr<MqttClientTestContainerInterface>> &clients) {
    for (auto &client : clients) {
        auto connect_action = client->getConnectAction();
        client->addAction(connect_action);
    }

    for (auto &client : clients) {
        auto subscribe_action = client->getSubscribeAction(client->configuration.clientId, 0);
        client->addAction(subscribe_action);
    }

    for (auto &client : clients) {
        std::vector<uint8_t> payload        = client->getPayload(10);
        auto publish_receive_publish_action = client->getPublishReceivePublishAction(client->configuration.clientId, payload, 1, false);
        client->addAction(publish_receive_publish_action);
    }

    for (auto &client : clients) {
        auto unsubscribe_action = client->getUnsubscribeAction(client->configuration.clientId);
        client->addAction(unsubscribe_action);
    }

    for (auto &client : clients) {
        auto delay_action = client->getDelayAction(1);
        client->addAction(delay_action);
    }

    for (auto &client : clients) {
        std::vector<uint8_t> payload        = client->getPayload(20);
        auto                 publish_action = client->getPublishAction(client->configuration.clientId, payload, 2, false);
        client->addAction(publish_action);
    }

    for (auto &client : clients) {
        auto disconnect_action = client->getDisconnectAction();
        client->addAction(disconnect_action);
    }

    for (auto &client : clients) {
        std::shared_ptr<MqttClientActionSequence> action_sequence = client->generateActionSequence(rounds);
    }
}
void configureMqttSnClients(int32_t rounds, const vector<std::shared_ptr<MqttSnClientTestContainerInterface>> &clients) {
    /*
    for (auto &client : clients) {
        std::vector<uint8_t> payload        = client->getPayload(20);
        auto                 publish_action = client->getPublishAction(MqttSnClientPublishTopicIdType::PREDEFINED, -1, 50, false, payload);
        client->addAction(publish_action);
    }
    // autoConnectAction
    for (auto &client : clients) {
        auto autoConnectAction = client->getAutoConnectAction();
        client->addAction(autoConnectAction);
    }
    for (auto &client : clients) {
        auto disconnect_action = client->getDisconnectAction();
        client->addAction(disconnect_action);
    }
    // connectAction
    for (auto &client : clients) {
        auto connectAction = client->getConnectAction();
        client->addAction(connectAction);
    }
    for (auto &client : clients) {
        auto disconnect_action = client->getDisconnectAction();
        client->addAction(disconnect_action);
    }

    // advertiseConnectAction
    for (auto &client : clients) {
        auto advertiseConnectAction = client->getAdvertiseConnectAction();
        client->addAction(advertiseConnectAction);
    }
    for (auto &client : clients) {
        auto disconnect_action = client->getDisconnectAction();
        client->addAction(disconnect_action);
    }
    // searchGatewayConnectAction
    for (auto &client : clients) {
        auto searchGatewayConnectAction = client->getSearchGatewayConnectAction();
        client->addAction(searchGatewayConnectAction);
    }
    for (auto &client : clients) {
        auto disconnect_action = client->getDisconnectAction();
        client->addAction(disconnect_action);
    }

    // directConnectAction
    for (auto &client : clients) {
        auto directConnectAction = client->getDirectConnectAction();
        client->addAction(directConnectAction);
    }
    for (auto &client : clients) {
        auto disconnect_action = client->getDisconnectAction();
        client->addAction(disconnect_action);
    }
         */

    for (auto &client : clients) {
        auto directConnectAction = client->getDirectConnectAction(client->configuration.brokerAddress, client->configuration.clientId, 60,
                                                                  client->configuration.cleanSession);
        client->addAction(directConnectAction);
    }
    /* // TODO from here

    for (auto &client : clients) {
        std::string registered_first    = "registered/first";
        auto        registration_action = client->getRegistrationAction(registered_first);
        client->addAction(registration_action);
    }

    for (auto &client : clients) {
        std::string registered_second   = "registered/second";
        auto        registration_action = client->getRegistrationAction(registered_second);
        client->addAction(registration_action);
    }
    for (auto &client : clients) {
        std::string registered_third    = "registered/third";
        auto        registration_action = client->getRegistrationAction(registered_third);
        client->addAction(registration_action);
    }
    for (auto &client : clients) {
        std::string registered_third    = "registered/third";
        auto        registration_action = client->getRegistrationAction(registered_third);
        client->addAction(registration_action);
    }

    for (auto &client : clients) {
        std::string subscribed_first    = "subscribed/topicname/first";
        auto        subscription_action = client->getSubscriptionAction(subscribed_first, 0);
        client->addAction(subscription_action);
    }
    for (auto &client : clients) {
        std::string subscribed_first    = "subscribed/topicname/second";
        auto        subscription_action = client->getSubscriptionAction(subscribed_first, 1);
        client->addAction(subscription_action);
    }
    for (auto &client : clients) {
        std::string subscribed_first    = "subscribed/topicname/third";
        auto        subscription_action = client->getSubscriptionAction(subscribed_first, 2);
        client->addAction(subscription_action);
    }
    for (auto &client : clients) {
        std::string registered_first    = "registered/first";
        auto        subscription_action = client->getSubscriptionAction(registered_first, 0);
        client->addAction(subscription_action);
    }

    for (auto &client : clients) {
        auto subscription_action = client->getSubscriptionAction(MqttSnClientPublishTopicIdType::PREDEFINED, 50, 0);
        client->addAction(subscription_action);
    }
    for (auto &client : clients) {
        auto subscription_action = client->getSubscriptionAction(MqttSnClientPublishTopicIdType::PREDEFINED, 50, 1);
        client->addAction(subscription_action);
    }
    for (auto &client : clients) {
        auto subscription_action = client->getSubscriptionAction(MqttSnClientPublishTopicIdType::PREDEFINED, 50, 2);
        client->addAction(subscription_action);
    }

    for (auto &client : clients) {
        std::string          registered_first = "registered/first";
        std::vector<uint8_t> payload          = client->getPayload(20);
        auto                 publish_action   = client->getPublishAction(registered_first, 0, false, payload);
        client->addAction(publish_action);
    }
    for (auto &client : clients) {
        std::string          registered_first = "registered/second";
        std::vector<uint8_t> payload          = client->getPayload(20);
        auto                 publish_action   = client->getPublishAction(registered_first, 1, false, payload);
        client->addAction(publish_action);
    }
    for (auto &client : clients) {
        std::string          registered_first = "registered/third";
        std::vector<uint8_t> payload          = client->getPayload(20);
        auto                 publish_action   = client->getPublishAction(registered_first, 2, false, payload);
        client->addAction(publish_action);
    }
    for (auto &client : clients) {
        std::vector<uint8_t> payload        = client->getPayload(20);
        auto                 publish_action = client->getPublishAction(MqttSnClientPublishTopicIdType::PREDEFINED, 0, 50, false, payload);
        client->addAction(publish_action);
    }
    for (auto &client : clients) {
        std::vector<uint8_t> payload        = client->getPayload(20);
        auto                 publish_action = client->getPublishAction(MqttSnClientPublishTopicIdType::PREDEFINED, 1, 50, false, payload);
        client->addAction(publish_action);
    }
     */ // TODO from here

    for (auto &client : clients) {
        std::vector<uint8_t> payload        = client->getPayload(20);
        auto                 publish_action = client->getPublishAction(MqttSnClientPublishTopicIdType::PREDEFINED, 2, 50, false, payload);
        client->addAction(publish_action);
    }



    /*

    for (auto &client : clients) {
        std::vector<uint8_t> payload        = client->getPayload(20);
        auto                 publish_action = client->getPublishAction(MqttSnClientPublishTopicIdType::TOPIC_ID, 0, 1, false, payload);
        client->addAction(publish_action);
    }

for (auto &client : clients) {
    std::vector<uint8_t> payload        = client->getPayload(20);
    auto                 publish_action = client->getPublishAction(MqttSnClientPublishTopicIdType::TOPIC_ID, 1, 4, false, payload);
    client->addAction(publish_action);
}
for (auto &client : clients) {
    std::vector<uint8_t> payload        = client->getPayload(20);
    auto                 publish_action = client->getPublishAction(MqttSnClientPublishTopicIdType::TOPIC_ID, 2, 5, false, payload);
    client->addAction(publish_action);
}
*/
    /* // TODO from here

for (auto &client : clients) {
    std::vector<uint8_t> payload = client->getPayload(20);

    auto publishReceivePublishAction =
      client->getPublishReceivePublishAction(MqttSnClientPublishTopicIdType::PREDEFINED, 50, 0, false, payload);
    client->addAction(publishReceivePublishAction);
}

for (auto &client : clients) {
std::vector<uint8_t> payload = client->getPayload(20);

auto publishReceivePublishAction =
  client->getPublishReceivePublishAction(MqttSnClientPublishTopicIdType::PREDEFINED, 50, 1, false, payload);
client->addAction(publishReceivePublishAction);
}
for (auto &client : clients) {
    std::vector<uint8_t> payload = client->getPayload(20);

    auto publishReceivePublishAction =
      client->getPublishReceivePublishAction(MqttSnClientPublishTopicIdType::PREDEFINED, 50, 2, false, payload);
    client->addAction(publishReceivePublishAction);
}
  */ // TODO from here

    /* // TODO from here
    for (auto &client : clients) {
        std::string          subscription_topic = "subscribed/topicname/first";
        std::vector<uint8_t> payload            = client->getPayload(20);

        auto publishReceivePublishAction = client->getPublishReceivePublishAction(subscription_topic, 0, false, payload);
        client->addAction(publishReceivePublishAction);
    }
    for (auto &client : clients) {
        std::string          subscription_topic = "subscribed/topicname/second";
        std::vector<uint8_t> payload            = client->getPayload(20);

        auto publishReceivePublishAction = client->getPublishReceivePublishAction(subscription_topic, 1, false, payload);
        client->addAction(publishReceivePublishAction);
    }
    for (auto &client : clients) {
        std::string          subscription_topic = "subscribed/topicname/third";
        std::vector<uint8_t> payload            = client->getPayload(20);

        auto publishReceivePublishAction = client->getPublishReceivePublishAction(subscription_topic, 2, false, payload);
        client->addAction(publishReceivePublishAction);
    }
    */ // TODO from here

    /**/
    /*
    for (auto &client : clients) {
        std::vector<uint8_t> payload = client->getPayload(20);

        auto publishReceivePublishAction =
          client->getPublishReceivePublishAction(MqttSnClientPublishTopicIdType::TOPIC_ID, 1, 0, false, payload);
        client->addAction(publishReceivePublishAction);
    }
    for (auto &client : clients) {
        std::vector<uint8_t> payload = client->getPayload(20);

        auto publishReceivePublishAction =
          client->getPublishReceivePublishAction(MqttSnClientPublishTopicIdType::TOPIC_ID, 4, 1, false, payload);
        client->addAction(publishReceivePublishAction);
    }
    for (auto &client : clients) {
        std::vector<uint8_t> payload = client->getPayload(20);

        auto publishReceivePublishAction =
          client->getPublishReceivePublishAction(MqttSnClientPublishTopicIdType::TOPIC_ID, 5, 2, false, payload);
        client->addAction(publishReceivePublishAction);
    }

    */
    // TODO does not work:
    /*
    for (auto &client : clients) {
        std::string          subscription_topic = "subscribed/name/first";
        std::vector<uint8_t> payload            = client->getPayload(20);

        auto publishReceivePublishAction = client->getPublishReceivePublishAction(subscription_topic, -1, false, payload);
        client->addAction(publishReceivePublishAction);
    }
     */

    /**/
    // TODO nicht unterstützt von Paho
    /*
    for (auto &client : clients) {
        std::string subscribed_first    = "subscribed/topicid/first";
        auto        subscription_action = client->getSubscriptionAction(MqttSnClientPublishTopicIdType::TOPIC_ID, 2, 2);
        client->addAction(subscription_action);
    }
     */
    /*
    for (auto &client : clients) {
        auto sleep_action = client->getSleepAction(60);
        client->addAction(sleep_action);
    }

    for (auto &client : clients) {
        auto sleep_action = client->getDelayAction(30);
        client->addAction(sleep_action);
    }

    for (auto &client : clients) {
        auto sleep_action = client->getWakeUpAction();
        client->addAction(sleep_action);
    }
    */

    /* // TODO from here
    for (auto &client : clients) {
        auto sleep_action = client->getReconnectAction();
        client->addAction(sleep_action);
    }
    for (auto &client : clients) {
        auto sleep_action = client->getPingAction();
        client->addAction(sleep_action);
    }
    */ // TODO from here
    /*
    for (auto &client : clients) {
        std::string subscribed_first      = "subscribed/topicname/first";
        auto        unsubscription_action = client->getUnsubscriptionAction(subscribed_first);
        client->addAction(unsubscription_action);
    }

    for (auto &client : clients) {
        std::string subscribed_first      = "subscribed/topicid/first";
        auto        unsubscription_action = client->getUnsubscriptionAction(MqttSnClientPublishTopicIdType::PREDEFINED, 50);
        client->addAction(unsubscription_action);
    }
    */
    // TODO nicht unterstützt von Paho?
    /*
    for (auto &client : clients) {
        std::string subscribed_first      = "subscribed/topicid/first";
        auto        unsubscription_action = client->getUnsubscriptionAction(MqttSnClientPublishTopicIdType::TOPIC_ID, 1);
        client->addAction(unsubscription_action);
    }
    */
    for (auto &client : clients) {
        auto disconnect_action = client->getDisconnectAction();
        client->addAction(disconnect_action);
    }
    for (auto &client : clients) {
        std::shared_ptr<MqttSnClientActionSequence> action_sequence = client->generateActionSequence(rounds);
    }
}
