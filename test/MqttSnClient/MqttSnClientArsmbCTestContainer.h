//
// Created by SomeDude on 31.07.2019.
//

#ifndef CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTARSMBCTESTCONTAINER_H_
#define CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTARSMBCTESTCONTAINER_H_

#include <client/MqttSnClientMethod.h>
#include <client/interactive/config/interactive_client_config.h>
#include <client/interactive/starter/mqtt_sn_client_interactive.h>
#include <config/common/confg_command_buffer.h>
#include <mutex>
#include "MqttSnClientPublishAction.h"
#include "MqttSnClientTestContainerConfiguration.h"
#include "MqttSnClientTestContainerInterface.h"

class MqttSnClientArsmbCTestContainer : public MqttSnClientTestContainerInterface {
 private:
  std::atomic_bool running{false};
  std::atomic_bool stopped{true};
  std::mutex runnerMutex;
  std::thread runner;

  MqttSnLogger cfg_logger;
  MqttSnLogger *logger = &cfg_logger;
  interactive_client_config_container cfg;
  mqtt_sn_interactive_client interactive_client;
  mqtt_sn_config_command_buffer cmd_buffer;

 public:
  MqttSnClientArsmbCTestContainer(const MqttSnClientTestContainerConfiguration &configuration);

  bool StartBackgroundHandler() override;
  void StopBackgroundHandler() override;
  bool IsBackgroundHandlerRunning() override;
  void loop();

  MqttSnClientActionResult connect(MqttSnClientConnectAction& connect_action) override;
  MqttSnClientActionResult connect(MqttSnClientDirectConnectAction &connect_action) override;
  MqttSnClientActionResult connect(MqttSnClientAdvertiseConnectAction &connect_action) override;
  MqttSnClientActionResult connect(MqttSnClientSearchGatewayConnectAction &connect_action) override;
  MqttSnClientActionResult disconnect() override;

  MqttSnClientActionResult publish(MqttSnClientPublishAction &publishAction) override;
  MqttSnClientActionResult reconnect(MqttSnClientReconnectAction& reconnectAction) override;
  MqttSnClientActionResult registration(MqttSnClientRegistrationAction& registrationAction) override;
  MqttSnClientActionResult subscribe(MqttSnClientSubscribeAction& subscribeAction) override;
  MqttSnClientActionResult unsubscribe(MqttSnClientUnsubscribeAction& unsubscribeAction) override;
  MqttSnClientActionResult publishReceivePublish(MqttSnClientPublishReceivePublishAction& publishReceivePublishAction) override;
  MqttSnClientActionResult sleep(MqttSnClientSleepAction& sleepAction) override;
  MqttSnClientActionResult wakeup(MqttSnClientWakeupAction& wakeupAction) override;
  MqttSnClientActionResult ping(MqttSnClientPingAction& pingAction) override;
  MqttSnClientActionResult delay(MqttSnClientDelayAction& delayAction) override;

 private:
  std::string getMqttSnClientConnectActionToCmdString(MqttSnClientConnectAction &connect_action);
  std::string getMqttSnClientDirectConnectActionToCmdString(MqttSnClientDirectConnectAction& connect_action);
  std::string MqttSnClientAdvertiseConnectActionToCmdString(MqttSnClientAdvertiseConnectAction& action);
  std::string MqttSnClientSearchGwConnectActionToCmdString(MqttSnClientSearchGatewayConnectAction& action);
  std::string getMqttSnClientTestContainerInterfaceToCmdString(const MqttSnClientTestContainerConfiguration& configuration);
  std::string getMqttSnClientPublishActionToCmdString(MqttSnClientPublishAction& action);
  std::string getMqttSnClientRegisterActionToCmdString(MqttSnClientRegistrationAction& registrationAction);
  std::string getMqttSnClientSubscribeActionToCmdString(MqttSnClientSubscribeAction& subscribeAction);
  std::string getMqttSnClientUnsubscribeActionToCmdString(MqttSnClientUnsubscribeAction& unsubscribeAction);
  std::string getMqttSnClientSleepActionToCmdString(MqttSnClientSleepAction& sleepAction);
  std::string getMqttSnClientReceivePublishActionToCmdString(MqttSnClientPublishReceivePublishAction& action);

  void clearCmdPublishList();
  void clearCmdRegisterList();
  void clearCmdSubscribeList();
  void clearCmdUnsubscribeList();
  bool parseCommandIntoConfiguration(std::string command);
  std::string getTrueOrFalse(bool b);
  std::string getHexString(const std::vector<uint8_t>& v, std::string d);

  MqttSnClientActionResult getMqttSnClientInteractiveCmdResultToMqttSnClientActionResult(mqtt_sn_client_interactive_cmd_result rc);
  MqttSnClientActionType getMqttSnClientInteractiveCmdActionTypeToMqttSnClientActionType(MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE action_type);
  MqttSnClientActionResultType getMqttSnClientInteractiveCmdActionResultTypeToMqttSnClientActionResultType(MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE action_result);
};

#endif //CMQTTSNFORWARDER_TEST_MQTTSNCLIENT_MQTTSNCLIENTARSMBCTESTCONTAINER_H_
