//
// Created by SomeDude on 08.07.2019.
//

#include "MqttSnMultipleForwarderTests.h"

using std::to_string;

void MqttSnMultipleForwarderTests::SetUp() {

  // TODO initialize broker

  for (size_t i = 1; i <= 1; ++i) {
    string gw_identifier = "gateway" + to_string(i);
    string gw_cmd = "-db -cp 2000" + to_string(i) + " --gateway_id " + to_string(i);
    std::shared_ptr<MqttSnGatewayTestContainer> gw_prt(new MqttSnGatewayTestContainer(gw_identifier, gw_cmd));
    ASSERT_EQ(gw_prt->initialize(), 0);
    gateways.push_back(gw_prt);
  }

  for (size_t i = 1; i <= 5; ++i) {
    string fw_identifier = "forwarder" + to_string(i);
    string fw_cmd = "-db -cp 3000" + to_string(i) + " -gp 4000" + to_string(i);
    std::shared_ptr<MqttSnForwarderTestContainer> fw_ptr(new MqttSnForwarderTestContainer(fw_identifier, fw_cmd));
    ASSERT_EQ(fw_ptr->initialize(), 0);
    forwarders.push_back(fw_ptr);
  }

  for (size_t i = 1; i <= 0; ++i) {
    string client_identifier = "client" + to_string(i);
    string client_cmd = "-db -gp 1000" + to_string(i) + " -p 20001";
    std::shared_ptr<MqttSnClientTestContainer> client_ptr(new MqttSnClientTestContainer(client_identifier, client_cmd));
    ASSERT_EQ(client_ptr->initialize(), 0);
    clients.push_back(client_ptr);
  }

  for (auto &gateway : gateways) {
    ASSERT_EQ(gateway->start(), 0);
  }

  for (auto &forwarder : forwarders) {
    ASSERT_EQ(forwarder->start(), 0);
  }

  for (auto &client : clients) {
    ASSERT_EQ(client->start(), 0);
  }
  // initialize gateway
  // intialize gateway
  // initialize forwarder (does not matter)
  // start client

}
void MqttSnMultipleForwarderTests::TearDown() {

  // stop clients
  for (auto &client : clients) {
    client->stop();
  }
  bool all_clients_stopped = false;
  while (!all_clients_stopped) {
    all_clients_stopped = true;
    for (auto &client : clients) {
      if (client->isRunning()) {
        all_clients_stopped = false;
        std::this_thread::sleep_for(std::chrono::microseconds(400));
        break;
      }
    }
  }

  // stop forwarder
  for (auto &forwarder : forwarders) {
    forwarder->stop();
  }
  bool all_forwarders_stopped = false;
  while (!all_forwarders_stopped) {
    all_forwarders_stopped = true;
    for (auto &forwarder : forwarders) {
      if (forwarder->isRunning()) {
        all_forwarders_stopped = false;
        std::this_thread::sleep_for(std::chrono::microseconds(400));
        break;
      }
    }
  }

  // stop gateway
  for (auto &gateway : gateways) {
    gateway->stop();
  }
  bool all_gateways_stopped = false;
  while (!all_gateways_stopped) {
    all_gateways_stopped = true;
    for (auto &gateway : gateways) {
      if (gateway->isRunning()) {
        all_gateways_stopped = false;
        std::this_thread::sleep_for(std::chrono::microseconds(400));
        break;
      }
    }
  }

}

TEST_F(MqttSnMultipleForwarderTests, gen_parse_test) {
  while(true) {
    std::this_thread::sleep_for(std::chrono::seconds(30));
  }
}
// test:
// connect to gateway
// stay connected for 10 minutes
