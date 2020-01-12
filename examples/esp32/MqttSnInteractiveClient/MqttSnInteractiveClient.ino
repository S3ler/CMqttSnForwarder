#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include <SSD1306.h>
#include <MqttSnClientArduino.hpp>

#ifdef WITH_EEPROM
EEPROM_cfg *eeprom_cfg = NULL;
#else
#define STASSID "..."
#define STAPSK  "..."
const char* ssid     = STASSID;
const char* password = STAPSK;
#endif
MqttSnLogger fcfg_logger = {0};

SSD1306 display(0x3c, 21, 22);
ssd1306status display_status = {0};
MqttSnLogger screen_logger = {0};
forwarder_config fcfg = {0};

#ifdef WITH_ARDUINO_GATEWAY_NETWORK_UDP
WiFiUDP gatewayWiFiUdp;
WiFiUDP gatewayBCWiFiUdp;
WiFiUDP *gatewayUdp = &gatewayWiFiUdp;
WiFiUDP *gatewayBCUdp = &gatewayBCWiFiUdp;
#endif

size_t line_pos = 0;

mqtt_sn_interactive_client interactive_client = { 0 };
interactive_client_config_container cfg;
mqtt_sn_config_command_buffer cmd_buffer;

int updateEEPROM();

void configureModeTimeout(int timeout) {
  if (timeout > 0) {
    Serial.print("Restarting in ");
    Serial.print(timeout, DEC);
    Serial.println(" seconds.");
  }
  int started = millis();
  digitalWrite(LED_BUILTIN, LOW);
  while (true) {
    yield();
    if (Serial.available() > 0) {
      started = millis();
      if (parse_arduino_serial_line(cmd_buffer.buffer, &line_pos, sizeof(cmd_buffer.buffer))) {
        updateEEPROM();
        mqtt_sn_config_command_buffer_reset(&cmd_buffer);
        line_pos = 0;

        started = millis();
        if (timeout > 0) {
          Serial.print("Restarting in ");
          Serial.print(timeout, DEC);
          Serial.println(" seconds.");
        }
      }
    }
    if (timeout > 0) {
      if (Serial.available() <= 0) {
        int current = millis();
        if ((current - started) > (timeout * 1000)) {
          Serial.println("Restarting ...");
          eeprom_config_cleanup(&eeprom_cfg);
          log_flush(&fcfg_logger);
          delay(1000);
          ESP.restart();
        }
      }
    }
  }
}
void configureMode() {
  configureModeTimeout(0);
}
#ifndef MQTT_SN_CLIENT_ARDUINO_IDENTIFIER
#define MQTT_SN_CLIENT_ARDUINO_IDENTIFIER "MqttSnArduinoClient"
#endif
char* client_identifier = MQTT_SN_CLIENT_ARDUINO_IDENTIFIER;
void setup() {

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  Serial.println();
  yield();

  if (MqttSnLoggerInit(&fcfg_logger, LOG_LEVEL_DEBUG, arduino_serial_log_init)) {
    Serial.println("Error: Could not initialize MqttSnLogger.");
    return;
  }
  if (client_identifier) {
    fcfg_logger.log_identifier = client_identifier;
  }
  interactive_client_config_init(&cfg);
  mqtt_sn_config_command_buffer_init(&cmd_buffer);

#ifdef WITH_EEPROM
  eeprom_config_init(&eeprom_cfg);
  int eeprom_fcfg_rc = eeprom_load_forwarder_config(eeprom_cfg, NULL, NULL, &fcfg_logger, NULL, NULL);
  if (eeprom_fcfg_rc == -2) {
    configureModeTimeout(30);
  }
  yield();
  int wifi_rc = connect_wifi(eeprom_cfg->WiFi_cfg.ssid, eeprom_cfg->WiFi_cfg.password, 20000, &fcfg_logger);
  if (wifi_rc != 0) {
    configureModeTimeout(30);
  }
#else
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.print("  ");
  WiFi.begin(ssid, password);
#endif
}
int32_t client_loop = 0;
void updateConfig();
void loop() {
  if (client_loop) {
    MQTT_SN_CLIENT_LOOP_RETURN_CODE loop_rc = MqttSnClientLoop(&interactive_client.client);
    if (loop_rc == MQTT_SN_CLIENT_LOOP_RETURN_ERROR) {
      client_loop = 0;
    }
  }
  if (Serial.available() > 0) {
    if (parse_arduino_serial_line(cmd_buffer.buffer, &line_pos, sizeof(cmd_buffer.buffer))) {
      if (!updateEEPROM()) {
        return;
      }

      yield();
      cmd_buffer.buffer[strlen(cmd_buffer.buffer) - 1] = 0; // cut '\n'
      // check for help
      if (mqtt_sn_client_interactive_is_help(cmd_buffer.buffer)) {
        mqtt_sn_client_interactive_print_usage(&fcfg_logger);
        mqtt_sn_config_command_buffer_reset(&cmd_buffer);
        line_pos = 0;
        return;
      }

      // handle interactive commands
      MQTT_SN_CLIENT_INTERACTIVE_ACTION_TYPE command_token;
      if ((command_token = mqtt_sn_client_interactive_parse_action(cmd_buffer.buffer, strlen(cmd_buffer.buffer))) !=
          MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_NO_COMMAND) {
        // TODO logger initialiasierung
        mqtt_sn_client_interactive_cmd_result exec_rc;
        exec_rc = mqtt_sn_cmd_buffer_execute_command(&interactive_client, command_token, &cfg, &fcfg_logger);

        print_mqtt_sn_client_interactive_cmd_result(&fcfg_logger, &exec_rc);
        if (command_token == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_INIT_CLIENT &&
            exec_rc.action_result == MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS) {
          client_loop = 1;
        }
        if (command_token == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_CONNECT_GATEWAY &&
            exec_rc.action_result == MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS) {
          client_loop = 0;
        }
        if (command_token == MQTT_SN_CLIENT_INTERACTIVE_COMMAND_TOKEN_DEINIT_CLIENT &&
            exec_rc.action_result == MQTT_SN_CLIENT_INTERACTIVE_ACTION_RESULT_TYPE_SUCCESS) {
          client_loop = 0;
        }

        mqtt_sn_config_command_buffer_reset(&cmd_buffer);
        line_pos = 0;
        return;
      }


      updateConfig();

    }
  }
}
int updateEEPROM() {
  mqtt_sn_config_command_buffer cmd_buffer_cp = mqtt_sn_config_command_buffer_cp(&cmd_buffer);
  if (mqtt_sn_config_command_buffer_parse_string(&cmd_buffer_cp) < 0) {
    log_str(&fcfg_logger, PSTR("Error tokenizing.\n"));
    line_pos = 0;
    return 0;
  }
  int eeprom_rc = process_eeprom_config_token(eeprom_cfg, &fcfg_logger, cmd_buffer_cp.argc, cmd_buffer_cp.argv);
  if (eeprom_rc == 0 || eeprom_rc == 2) {
    if (eeprom_rc == 0) {
      eeprom_config_set(eeprom_cfg, NULL, 0);
    }
    mqtt_sn_config_command_buffer_reset(&cmd_buffer);
    line_pos = 0;
    return 0;
  }
  int system_rc = process_system_config_token(&fcfg_logger, cmd_buffer_cp.argc, cmd_buffer_cp.argv);
  if (system_rc == 0 || system_rc == 2) {
    mqtt_sn_config_command_buffer_reset(&cmd_buffer);
    line_pos = 0;
    return 0;
  }
  return 1;
}
void updateConfig() {

  // update configuration
  // create a copy for simulating
  mqtt_sn_config_command_buffer cmd_buffer_cp = mqtt_sn_config_command_buffer_cp(&cmd_buffer);
  if (mqtt_sn_config_command_buffer_parse_string(&cmd_buffer_cp) < 0) {
    log_str(&fcfg_logger, PSTR("Error tokenizing.\n"));
    line_pos = 0;
    return;
  }
  int rc;
  if ((rc = mqtt_sn_cmd_buffer_interactive_client_simulate(&cfg, &fcfg_logger, cmd_buffer_cp.argc, cmd_buffer_cp.argv)) < 0) {
    // no valid configuration - no update
    mqtt_sn_config_command_buffer_reset(&cmd_buffer);
    line_pos = 0;
    return;
  }

  if (rc != MQTT_SN_PARSE_CONFIG_SUCCESS) {
    if (rc == MQTT_SN_PARSE_CONFIG_HELP) {
      interactive_client_config_print_usage(&fcfg_logger);
      mqtt_sn_config_command_buffer_reset(&cmd_buffer);
      line_pos = 0;
      return;
    } else {
      interactive_client_config_print_see_usage(&fcfg_logger);
      mqtt_sn_config_command_buffer_reset(&cmd_buffer);
      line_pos = 0;
      return;
    }
  }

  // simulation successful now write real values into configuration
  if (mqtt_sn_config_command_buffer_parse_string(&cmd_buffer) < 0) {
    log_str(&fcfg_logger, PSTR("Error tokenizing.\n"));
    line_pos = 0;
    return;
  }

  if (cmd_buffer.argc == 1) {
    mqtt_sn_config_command_buffer_reset(&cmd_buffer);
    line_pos = 0;
    return;
  }

  interactive_client_config_process_line(&cfg, &fcfg_logger, cmd_buffer.argc, cmd_buffer.argv);
  interactive_client_config_copy_to_buffer(&cfg);

  mqtt_sn_config_command_buffer_reset(&cmd_buffer);
  interactive_client_config_print_updated_config(&fcfg_logger, 1);
  line_pos = 0;

}
