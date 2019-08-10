//
// Created by SomeDude on 31.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTARSMBCTESTCONTAINER_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTARSMBCTESTCONTAINER_H_

#include <client/interactive/linux/starter/MqttSnCmdBuffer.h>
#include <client/MqttSnClient.h>
#include "MqttSnClientTestContainerInterface.h"
#include "MqttSnClientTestContainerConfiguration.h"

class MqttSnClientArsmbCTestContainer : public MqttSnClientTestContainerInterface {
 private:
  std::atomic_bool running{false};
  std::atomic_bool stopped{false};

  MqttSnClient *client = nullptr;

  MqttSnLogger cfg_logger;
  MqttSnLogger *logger = &cfg_logger;
  interactive_client_config cfg;
  mqtt_sn_interactive_client interactive_client;
  mqtt_sn_interactive_client_cmd_buffer cmd_buffer;

 public:
  MqttSnClientArsmbCTestContainer(const MqttSnClientTestContainerConfiguration &configuration);

  bool StartBackgroundHandler() override;
  void StopBackgroundHandler() override;
  bool IsBackgroundHandlerRunning() override;

  MqttSnClientActionResult connect(MqttSnClientDirectConnectAction &connect_action) override;
  MqttSnClientActionResult connect(MqttSnClientAdvertiseConnectAction &connect_action) override;
  MqttSnClientActionResult connect(MqttSnClientSearchGwConnectAction &connect_action) override;
  MqttSnClientActionResult disconnect() override;
  MqttSnClientActionResultType GetActionResultType(MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE type);
  void loop();
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTARSMBCTESTCONTAINER_H_
