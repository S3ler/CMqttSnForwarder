#include <MqttSnArduino.h>
//#include <logging/arduino/Serial/ArduinoSerial.cpp>
//#include <logging/arduino/serial/ArduinoSerial.hpp>
//#include <logging/arduino/SD/ArduinoSD.cpp>
//#include <logging/arduino/SD/ArduinoSD.hpp>
//#include <logging/arduino/SSD1306/ArduinoSSD1306.cpp>
//#include <logging/arduino/SSD1306/ArduinoSSD1306.hpp>
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include <SSD1306.h>

#ifdef WITH_EEPROM
EEPROM_cfg *eeprom_cfg = NULL;
#else
#define COMMAND "-verbose"
#endif
MqttSnLogger cfg_logger = {0};

SSD1306 display(0x3c, 21, 22);
ssd1306status display_status = {0};
MqttSnLogger screen_logger = {0};

char line[EEPROM_LINE_SIZE] = {0};
size_t line_pos = 0;
size_t line_max = EEPROM_LINE_SIZE;

const char* executable_name = "MqttSnLogger";
mqtt_sn_logger_config mslcfg;
int32_t log_mqtt_sn_logger_example_input_too_long(const MqttSnLogger *logger, uint64_t line_max) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
    return 0;
  }
  log_msg_start(logger);
  log_str(logger, PSTR(" input too long - maximum character are"));
  log_uint64(logger, line_max);
  log_flush(logger);
  return log_status(logger);
}

int32_t log_mqtt_sn_logger_log_level(const MqttSnLogger *logger, uint64_t loop_counter) {
  if (is_logger_not_available(logger) || shall_not_be_logged(logger, LOG_LEVEL_VERBOSE)) {
    return 0;
  }

  log_msg_start(logger);
  log_str(logger, PSTR("log_level: "));
#ifdef WITH_DEBUG_LOGGING
  if (logger->log_level == LOG_LEVEL_DEBUG) {
    log_str(logger, PSTR("LOG_LEVEL_DEBUG"));
  } else
#endif
    if (logger->log_level == LOG_LEVEL_DEFAULT) {
      log_str(logger, PSTR("LOG_LEVEL_DEFAULT"));
    } else if (logger->log_level == LOG_LEVEL_VERBOSE) {
      log_str(logger, PSTR("LOG_LEVEL_VERBOSE"));
    } else {
      log_str(logger, PSTR("UNKNOWN"));
    }
  log_str(logger, PSTR(" - "));
  log_uint64(logger, loop_counter);

  log_flush(logger);
  return log_status(logger);
}

int32_t mqtt_sn_logger_config_file_process_command_callback(void *cfg, const MqttSnLogger *logger, int argc, char **argv) {
  mqtt_sn_logger_config *mslcfg = (mqtt_sn_logger_config *) cfg;

    if (contains_config_argument_help(argc, argv)) {
        return MQTT_SN_PARSE_CONFIG_HELP;
    }
  int32_t mslcfg_rc = mqtt_sn_logger_config_process_args(mslcfg, logger, argc, argv);
  if (mslcfg_rc == MQTT_SN_PARSE_CONFIG_FAILURE) {
    return MQTT_SN_PARSE_CONFIG_FAILURE;
  }
  if (mslcfg_rc != (argc - 1)) {
    for (int i = 1; i < argc; i++) {
      if (!is_mqtt_sn_logger_config_command(argv[i], &i)) {
        print_unknown_option(logger, argv[i]);
        return MQTT_SN_PARSE_CONFIG_FAILURE;
      }
    }
  }
  return MQTT_SN_PARSE_CONFIG_SUCCESS;
}
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
      if (parse_arduino_serial_line(line, &line_pos, line_max)) {
        arduino_serial_eval_process(eeprom_cfg, &mslcfg, executable_name, &cfg_logger, line, line_pos, mqtt_sn_logger_config_file_process_command_callback, mqtt_sn_logger_config_print_usage);
        memset(line, 0, sizeof(line));
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
          log_flush(&cfg_logger);
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
void setup() {
  mqtt_sn_logger_config_init(&mslcfg);

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  Serial.println();
  yield();

  if (MqttSnLoggerInit(&screen_logger, LOG_LEVEL_DEBUG, ssd1306_screen_log_init)) {
    Serial.println("Error: Could not initialize MqttSnLogger.");
    return;
  }
  Serial.println("Use '--help' to see usage.");

#ifdef WITH_EEPROM
  eeprom_config_init(&eeprom_cfg);
  int eeprom_fcfg_rc  = eeprom_load_forwarder_config(eeprom_cfg, &mslcfg, executable_name , &cfg_logger, mqtt_sn_logger_config_file_process_command_callback, mqtt_sn_logger_config_print_usage);
  if (eeprom_fcfg_rc == -2) {
    configureModeTimeout(30);
  }
  yield();
#endif
  if (MqttSnLoggerInit(&cfg_logger, LOG_LEVEL_DEBUG, arduino_serial_log_init)) {
    Serial.println("Error: Could not initialize MqttSnLogger.");
    return;
  }
}
uint64_t loop_counter = 0;
void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  log_mqtt_sn_logger_log_level(&cfg_logger, loop_counter);
  if (Serial.available() > 0) {
    int32_t parse_rc = parse_arduino_serial_line(line, &line_pos, line_max);
    if (parse_rc == 1) {
      arduino_serial_eval_process(eeprom_cfg, &mslcfg, executable_name, &cfg_logger, line, line_pos, mqtt_sn_logger_config_file_process_command_callback, mqtt_sn_logger_config_print_usage);
      memset(line, 0, sizeof(line));
      line_pos = 0;
    } else if (parse_rc == 2) {
      log_mqtt_sn_logger_example_input_too_long(&cfg_logger, line_max);
      memset(line, 0, sizeof(line));
      line_pos = 0;
    }
  }
  delay(2000);
  loop_counter += 1;
  digitalWrite(LED_BUILTIN, HIGH);
}