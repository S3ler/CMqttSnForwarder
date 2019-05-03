//
// Created by SomeDude on 01.05.2019.
//
#include "system.h"
#include <forwarder/network/shared/shared/IpHelper.h>
#include <forwarder/logging/MqttSnForwarderLoggingBasic.h>
#include <forwarder/network/arduino/shared/ip/ArduinoIpAddressHelper.h>
#include <assert.h>

int connect_wifi(EEPROM_cfg *ecfg, const MqttSnLogger *logger, uint32_t timeout_ms) {
  if (strlen(ecfg->WiFi_cfg.ssid) == 0) {
    // WiFi not configured
    print_wifi_not_configured(logger);
    return -1;
  }
#ifdef WITH_DEBUG_LOGGING
  print_wifi_connecting_to(logger, ecfg->WiFi_cfg.ssid, ecfg->WiFi_cfg.password);
#else
  log_wifi_connecting_to(logger, ecfg->WiFi_cfg.ssid, NULL);
#endif
  if (WiFi.status() == WL_CONNECTED) {
    WiFi.disconnect();
  }

  digitalWrite(LED_BUILTIN, HIGH);
  uint32_t started = millis();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ecfg->WiFi_cfg.ssid, ecfg->WiFi_cfg.password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    log_str(logger, ".");
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);

    int current = millis();
    if ((current - started) > timeout_ms) {
      log_flush(logger);
      break;
    }
  }
  if (WiFi.status() != WL_CONNECTED) {
    print_wifi_could_not_connect(logger);
    return 1;
  }
  log_flush(logger);
  return 0;
}

int parse_arduino_serial_line(char *line, size_t *line_pos, size_t line_max) {
  if (Serial.available() > 0) {
    char c = Serial.read();
    line[*line_pos] = c;
    if (*line_pos + 1 == line_max) {
      *line_pos += 1;
      return 1;
    }
    if (line[*line_pos] == '\n') {
      *line_pos += 2;
      return 1;
    }
    *line_pos += 1;
  }
  return 0;
}

int arduino_serial_eval_process(EEPROM_cfg *ecfg, forwarder_config *fcfg, char *line, size_t line_pos) {
#ifdef WITH_EEPROM
  // process eeprom commands
  int ecfg_rc = process_eeprom_config_str(ecfg, fcfg->logger, line, line_pos);
  if (ecfg_rc != 1) {
    return 3;
  }
#endif

#ifdef WITH_RESTART
  int system_rc = process_system_config_str(fcfg->logger, line, line_pos);
  if (system_rc == 0 || system_rc == 2) {
    return 4;
  }
#endif

  int fcfg_rc = eeprom_save_forwarder_config_line(ecfg, fcfg, line, line_pos);
  if (fcfg_rc == 0) {
    log_system_restarting(fcfg->logger);
    eeprom_config_cleanup(&ecfg);
    ESP.restart();
  }
  return fcfg_rc;
}

int convert_hostname_port_to_device_address(const MqttSnLogger *logger,
                                            const char *hostname,
                                            int port,
                                            device_address *address,
                                            const char *address_name) {
  if (hostname == NULL) {
    memset(address, 0, sizeof(device_address));
  } else {
    if (convert_string_to_device_address(hostname, address)) {
      // no form of XXX.XXX.XXX.XXX
      if (get_device_address_from_hostname(hostname, address)) {
        print_cannot_convert_or_resolve_network_address(logger, hostname, address_name);
        return EXIT_FAILURE;
      } else {
        print_resolved_hostname_to(logger, hostname, address);
      }
    }
  }
  if (port < -1 || port > 65535) {
    print_invalid_port_given(logger, port);
    return EXIT_FAILURE;
  }
  if (port > -1) {
    add_port_to_device_address(port, address);
  }
  return EXIT_SUCCESS;
}

int get_device_address_from_hostname(const char *hostname, device_address *dst) {
  if (EndsWithLocal(hostname)) {
#ifdef MDNS_RESOLVER_h
    // resolve via mDNS
    // for mDNS resolver we use: https://github.com/madpilot/mDNSResolver
    WiFiUDP udp;
    mDNSResolver::Resolver resolver(udp);
    resolver.setLocalIP(WiFi.localIP());
    IPAddress ip = resolver.search(hostname);
    if (ip == INADDR_NONE) {
      return EXIT_FAILURE;
    }
    arduino_ipv4_and_port_to_device_address(&ip, 0, dst);
    return EXIT_SUCCESS;
#else
    return EXIT_FAILURE;
#endif
  }
  // resolve via DNS
  IPAddress remote_addr;
  uint32_t timeout_ms = 30000;
  if (WiFi.hostByName(hostname, remote_addr, timeout_ms)) {
    arduino_ipv4_and_port_to_device_address(&remote_addr, 0, dst);
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

int EndsWith(const char *str, const char *suffix) {
  // from: https://stackoverflow.com/questions/744766/how-to-compare-ends-of-strings-in-c
  if (!str || !suffix) {
    return 0;
  }
  size_t lenstr = strlen(str);
  size_t lensuffix = strlen(suffix);
  if (lensuffix > lenstr) {
    return 0;
  }
  return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}
int EndsWithLocal(const char *str) { return EndsWith(str, ".local"); }
int StartsWith(const char *pre, const char *str) {
  // from: https://stackoverflow.com/questions/4770985/how-to-check-if-a-string-starts-with-another-string-in-c
  size_t lenpre = strlen(pre),
      lenstr = strlen(str);
  return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}

int print_system_config_usage(const MqttSnLogger *logger) {
  log_str(logger, "EEPROM Usage:     [-sr system_restart]\n");
  log_str(logger, "                  --system_help\n");
  log_flush(logger);
  log_str(logger, " -sr : restarts the system.\n");
#ifdef WITH_EEPROM
  log_str(logger, "       EEPROM is not automatically saved.\n");
#endif
  log_str(logger, " --system_help : display this message.\n");
  log_flush(logger);
  return log_status(logger);
}

int process_system_config_str(const MqttSnLogger *logger, char *ecfg_line, size_t ecfg_line_len) {
  assert(logger != NULL);

  char *argv_0 = "ecfg_line";

  if (ecfg_line == NULL) {
    return -1;
  }
  if (ecfg_line_len == 0) {
    return -1;
  }

  if (strlen(ecfg_line) + 1 != ecfg_line_len) {
    return -1;
  }
  if (strlen(ecfg_line) == 0) {
    return -1;
  }

  size_t len = ecfg_line_len;
  int tk_count = 0;
  {
    char line_copy[len];
    memcpy(line_copy, ecfg_line, len);
    // estimates argc
    for (char *tk = strtok(line_copy, " "); tk != NULL; tk = strtok(NULL, " ")) {
      tk_count++;
    }
  }

  char *argv[tk_count];
  int argc = 0;
  char line_copy[len];
  {
    memcpy(line_copy, ecfg_line, len);
    argv[argc++] = argv_0;
    for (char *tk = strtok(line_copy, " "); tk != NULL; tk = strtok(NULL, " ")) {
      argv[argc++] = tk;
    }
  }

  // remove '\n' from tokens
  for (uint16_t i = 1; i < argc; i++) {
    if (argv[i][(strlen(argv[i]) - 1)] == '\n') {
      argv[i][(strlen(argv[i]) - 1)] = '\0';
    }
  }

#ifdef WITH_DEBUG_LOGGING
  log_str(logger, "argc: ");
  log_uint64(logger, argc);
  log_str(logger, " argv: ");
  for (uint16_t i = 0; i < argc; i++) {
    log_str(logger, argv[i]);
    if (i + 1 < argc) {
      log_str(logger, ", ");
    }
  }
  log_flush(logger);
#endif

  // process
  int rc = process_system_config_token(logger, argc, argv);
  return rc;
}

int validate_system_config_token(const MqttSnLogger *logger, int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-sr") || !strcmp(argv[i], "--system_restart")) {
      i++;
    } else if (!strcmp(argv[i], "--system_help")) {
      return 2;
    } else {
#ifdef WITH_DEBUG_LOGGING
      print_system_unknown_option(logger, argv[i]);
#endif
      return 1;
    }
  }
  return 0;
}
int process_system_config_token(const MqttSnLogger *logger, int argc, char *argv[]) {
  int validation_rc = validate_system_config_token(logger, argc, argv);
  if (validation_rc == 2) {
    print_system_config_usage(logger);
    return 2;
  }
  if (validation_rc != 0) {
    return validation_rc;
  }

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-sr") || !strcmp(argv[i], "--system_restart")) {
      log_system_restarting(logger);
      //eeprom_config_cleanup(&ecfg);
      ESP.restart();
    }
  }

  return 0;
}

#ifdef WITH_LOGGING
int log_system_restarting(const MqttSnLogger *logger) {
  log_str(logger, "Restarting system ...");
  log_flush(logger);
  return log_status(logger);
}
int print_system_unknown_option(const MqttSnLogger *logger, const char *unknown_option) {
  log_str(logger, "Error: Unknown system option ");
  log_str(logger, unknown_option);
  log_str(logger, ".");
  log_flush(logger);
  return log_status(logger);
}
int print_cannot_convert_or_resolve_network_address(const MqttSnLogger *logger,
                                                    const char *hostname,
                                                    const char *address_name) {
  log_str(logger, "Cannot convert or resolve ");
  log_str(logger, hostname);
  log_str(logger, " to ");
  log_str(logger, address_name);
  log_str(logger, " network address.");
  log_flush(logger);
  return log_status(logger);
}
int print_resolved_hostname_to(const MqttSnLogger *logger, const char *hostname, const device_address *address) {
  log_str(logger, "Resolved ");
  log_str(logger, hostname);
  log_str(logger, " to ");
  log_device_address(logger, address);
  log_str(logger, ".");
  log_flush(logger);
  return log_status(logger);
}
int print_wifi_could_not_connect(const MqttSnLogger *logger) {
  log_str(logger, "Could not connect to Wifi - configure with: --eeprom_wifi_name and --eeprom_wifi_password.");
  log_flush(logger);
  return log_status(logger);
}
int print_wifi_connecting_to(const MqttSnLogger *logger, const char *ssid, const char *password) {
  log_str(logger, "Connecting to WiFi: ");
  log_str(logger, ssid);
  log_str(logger, " , password: ");
#ifdef WITH_DEBUG_LOGGING
  log_str(logger, password);
#else
  log_str(logger, "****");
#endif
  log_str(logger, " ");
  return log_status(logger);
}
int print_wifi_not_configured(const MqttSnLogger *logger) {
  log_str(logger, "No WiFi Configuration found - configure with: --eeprom_wifi_name and --eeprom_wifi_password.");
  log_flush(logger);
  return log_status(logger);
}
int log_init_gateway_network_unknown_protocol(const MqttSnLogger *logger, const char *protocol) {
  return log_init_network_unknown_protocol(logger, "gateway", protocol);
}
int log_init_client_network_unknown_protocol(const MqttSnLogger *logger, const char *protocol) {
  return log_init_network_unknown_protocol(logger, "client", protocol);
}
int log_init_network_unknown_protocol(const MqttSnLogger *logger, const char *network_name, const char *protocol) {
  log_str(logger, "Error init ");
  log_str(logger, network_name);
  log_str(logger, " network unknown protocol: ");
  log_str(logger, protocol);
  log_flush(logger);
  return log_status(logger);
}
#endif

