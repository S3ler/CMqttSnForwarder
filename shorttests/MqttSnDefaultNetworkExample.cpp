#include <chrono>
#include <thread>
#include <test/MqttNetworkBuilder/MqttNetworkBuilder.h>
#include <test/MqttSnClient/MqttSnClientTestContainerInterface.h>
#include <test/MqttSnNetworkBuilder/MqttSnNetworkBuilder.h>
//
// Created by SomeDude on 17.09.2019.
//

using std::vector;

void configureMqttSnClients(int32_t rounds, const vector<std::shared_ptr<MqttSnClientTestContainerInterface>> &clients);


int main() {
    int32_t mqttBrokerCount            = 1;
    int32_t mqttClientCount            = 1;
    int32_t mqttSnGatewayCount         = 1;
    int32_t mqttSnClientCount          = 0;
    int32_t mqttSnForwarderCount       = 1;
    int32_t mqttSnForwarderClientCount = 1;
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

    for (int32_t i = 0; i < mqttSnForwarderCount; i++) {
        mqttSnNetworkBuilder->getMqttSnForwarder();
    }
    for (int32_t i = 0; i < mqttSnForwarderClientCount; i++) {
        mqttSnNetworkBuilder->getForwardedMqttSnClient();
    }

    for (int32_t i = 0; i < mqttSnClientCount; i++) {
        mqttSnNetworkBuilder->getMqttSnClient();
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

    for (auto &client : mqttNetworkBuilder->getMqttClients()) {
        auto connect_action = client->getConnectAction();
        client->addAction(connect_action);
    }

    for (auto &client : mqttNetworkBuilder->getMqttClients()) {
        auto subscribe_action = client->getSubscribeAction(client->configuration.clientId, 1);
        client->addAction(subscribe_action);
    }
    for (auto &client : mqttNetworkBuilder->getMqttClients()) {
        std::vector<uint8_t> payload        = client->getPayload(10);
        auto publish_receive_publish_action = client->getPublishReceivePublishAction(client->configuration.clientId, payload, 1, false);
        client->addAction(publish_receive_publish_action);
    }
    for (auto &client : mqttNetworkBuilder->getMqttClients()) {
        auto disconnect_action = client->getDisconnectAction();
        client->addAction(disconnect_action);
    }

    configureMqttSnClients(rounds, mqttSnNetworkBuilder->getMqttSnClients());
    /*
    for (auto &client : mqttSnNetworkBuilder->getMqttSnClients()) {
        //auto directConnectAction = client->getDirectConnectAction(client->configuration.brokerAddress, client->configuration.clientId, 15,
        //                                                          client->configuration.cleanSession);
        auto directConnectAction = client->getDirectConnectAction();
        client->addAction(directConnectAction);
    }
    for (auto &client : mqttSnNetworkBuilder->getMqttSnClients()) {
        auto register_action = client->getRegistrationAction(client->configuration.clientId);
        client->addAction(register_action);
    }
    for (auto &client : mqttSnNetworkBuilder->getMqttSnClients()) {
        auto register_action = client->getSubscriptionAction(client->configuration.clientId, 0);
        client->addAction(register_action);
    }
    for (auto &client : mqttSnNetworkBuilder->getMqttSnClients()) {
        auto delay_action = client->getDelayAction(1);
        client->addAction(delay_action);
    }
    for (auto &client : mqttSnNetworkBuilder->getMqttSnClients()) {
        std::vector<uint8_t> payload        = client->getPayload(4);
        auto publish_receive_publish_action = client->getPublishReceivePublishAction(client->configuration.clientId, 0, false, payload);
        client->addAction(publish_receive_publish_action);
    }
    for (auto &client : mqttSnNetworkBuilder->getMqttSnClients()) {
        auto delay_action = client->getDelayAction(1);
        client->addAction(delay_action);
    }
    for (auto &client : mqttSnNetworkBuilder->getMqttSnClients()) {
        auto disconnect_action = client->getDisconnectAction();
        client->addAction(disconnect_action);
    }
    for (auto &client : mqttSnNetworkBuilder->getMqttSnClients()) {
        std::shared_ptr<MqttSnClientActionSequence> action_sequence = client->generateActionSequence(rounds);
    }
    */
    for (auto &client : mqttNetworkBuilder->getMqttClients()) {
        if (!client->execActionSequence_async()) {
            throw std::exception();
        }
    }
    for (auto &client : mqttSnNetworkBuilder->getMqttSnClients()) {
        if (!client->execActionSequence_async()) {
            throw std::exception();
        }
    }

    std::cout << std::endl;
    std::cout << std::endl;
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

    mqttNetworkBuilder->stopMqttClients();
    mqttSnNetworkBuilder->stopMqttSnClients();

    mqttSnNetworkBuilder->stopMqttSnForwarders();
    mqttSnNetworkBuilder->stopMqttSnGateways();

    mqttNetworkBuilder->stopMqttBrokers();

    return 0;
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

    for (auto &client : clients) {
        auto sleep_action = client->getReconnectAction();
        client->addAction(sleep_action);
    }
    for (auto &client : clients) {
        auto sleep_action = client->getPingAction();
        client->addAction(sleep_action);
    }
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
