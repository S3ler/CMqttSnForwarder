//
// Created by SomeDude on 14.08.2019.
//
#include <memory>
#include <vector>
#include <test/MqttNetworkBuilder/MqttNetworkBuilder.h>
#include <test/MqttSnClient/MqttSnClientAction.h>
#include <test/MqttSnClient/MqttSnClientConnectAction.h>
#include <csignal>
#include <test/MqttSnGateway/MqttSnGatewayTestContainerInterface.h>
#include <test/MqttSnNetworkBuilder/MqttSnNetworkBuilder.h>

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int _) {
    (void)_;
    keep_running = 0;
}

int main() {
    signal(SIGINT, sig_handler);

    int32_t mqttBrokerCount    = 1;
    int32_t mqttSnGatewayCount = 1;

    std::vector<std::shared_ptr<MqttBrokerTestContainerInterface>>    mqttBrokers;
    std::vector<std::shared_ptr<MqttSnGatewayTestContainerInterface>> mqttSnGateways;

    auto mqttNetworkBuilder = std::make_shared<MqttNetworkBuilder>();
    for (int32_t i = 0; i < mqttBrokerCount; i++) {
        mqttBrokers.push_back(mqttNetworkBuilder->getMqttBroker());
    }
    auto mqttSnNetworkBuilder = std::make_shared<MqttSnNetworkBuilder>(mqttNetworkBuilder.get());

    auto gw1 = mqttSnNetworkBuilder->getMqttSnGateway();

    auto gw2 = mqttSnNetworkBuilder->getMqttSnGateway(
      mqttSnNetworkBuilder->getMqttSnGatewayConfiguration(gw1->configuration.interfaceBroadcastAddress));
    auto gw3 = mqttSnNetworkBuilder->getMqttSnGateway(
      mqttSnNetworkBuilder->getMqttSnGatewayConfiguration(gw1->configuration.interfaceBroadcastAddress));
    auto gw4 = mqttSnNetworkBuilder->getMqttSnGateway(
      mqttSnNetworkBuilder->getMqttSnGatewayConfiguration(gw1->configuration.interfaceBroadcastAddress));
    auto gw5 = mqttSnNetworkBuilder->getMqttSnGateway(
      mqttSnNetworkBuilder->getMqttSnGatewayConfiguration(gw1->configuration.interfaceBroadcastAddress));
    auto gw6 = mqttSnNetworkBuilder->getMqttSnGateway(
      mqttSnNetworkBuilder->getMqttSnGatewayConfiguration(gw1->configuration.interfaceBroadcastAddress));

    mqttSnGateways.push_back(gw1);
    mqttSnGateways.push_back(gw2);
    mqttSnGateways.push_back(gw3);
    mqttSnGateways.push_back(gw4);
    mqttSnGateways.push_back(gw5);
    mqttSnGateways.push_back(gw6);

    for (int32_t i = 0; i < mqttSnGatewayCount; i++) {
        auto gw2 = mqttSnNetworkBuilder->getMqttSnGateway();
        mqttSnGateways.push_back(gw2);
    }
    for (auto &broker : mqttBrokers) {
        if (!broker->start_broker()) {
            throw std::exception();
        }
    }
    for (auto &gateway : mqttSnGateways) {
        if (!gateway->start()) {
            throw std::exception();
        }
    }

    std::cout << "started" << std::endl << std::flush;
    while (keep_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    for (auto &mqtt_sn_gateway : mqttSnGateways) {
        mqtt_sn_gateway->stop();
    }
    bool isMqttSnGwRunning = true;
    while (isMqttSnGwRunning) {
        isMqttSnGwRunning = false;
        for (auto &mqtt_sn_gateway : mqttSnGateways) {
            if (mqtt_sn_gateway->isRunning()) {
                isMqttSnGwRunning = true;
                break;
            }
        }
    }

    for (auto &broker : mqttBrokers) {
        broker->stop_broker();
        while (broker->isRunning()) {
        }
    }

    return 0;
}