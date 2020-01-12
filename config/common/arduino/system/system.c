//
// Created by SomeDude on 01.05.2019.
//
#include "system.h"
#include "system_logging.h"
#include <assert.h>
#include <platform/platform_compatibility.h>
#include <string.h>
#include <network/shared/ip/IpHelper.h>
#include <network/shared/ip/IpHelperLogging.h>
#include <network/arduino/shared/ip/ArduinoIpAddressHelper.hpp>
#include <cstdlib>
#include <config/common/arduino/eeprom/forwarder/forwarder_eeprom_loader.h>
#include <config/common/config_command_helper.h>

int connect_wifi(const char *ssid, const char *password, uint32_t timeout_ms, const MqttSnLogger *logger) {
    if (strlen(ssid) == 0) {
        // WiFi not configured
        print_wifi_not_configured(logger);
        return -1;
    }
#ifdef WITH_DEBUG_LOGGING
    print_wifi_connecting_to(logger, ssid, password);
#else
    log_wifi_connecting_to(logger, ssid, NULL);
#endif
    if (WiFi.status() == WL_CONNECTED) {
        WiFi.disconnect();
    }

    digitalWrite(LED_BUILTIN, HIGH);
    uint32_t started = millis();

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

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
    print_wifi_connected(logger);
    return 0;
}

int parse_arduino_serial_line(char *line, size_t *line_pos, size_t line_max) {
    if (Serial.available() > 0) {
        char c          = Serial.read();
        line[*line_pos] = c;
        if (*line_pos + 1 > line_max) {
            return 2;
        }
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

int arduino_serial_eval_process(EEPROM_cfg *ecfg, void *fcfg, const char *executable_name, const MqttSnLogger *logger, char *line,
                                size_t line_pos,
                                int32_t (*forwarder_config_file_process_command_callback)(void *cfg, const MqttSnLogger *logger, int argc,
                                                                                          char **argv),
                                void (*config_print_usage)(const MqttSnLogger *logger)) {
#ifdef WITH_EEPROM
    // process eeprom commands
    int ecfg_rc = process_eeprom_config_str(ecfg, logger, line, line_pos);
    if (ecfg_rc != 1) {
        return 3;
    }
#endif

#ifdef WITH_RESTART
    int system_rc = process_system_config_str(logger, line, line_pos);
    if (system_rc == 0 || system_rc == 2) {
        return 4;
    }
#endif

    if (fcfg == NULL) {
        return 5;
    }

    int fcfg_rc = eeprom_save_forwarder_config_line(ecfg, fcfg, executable_name, logger, line, line_pos,
                                                    forwarder_config_file_process_command_callback, config_print_usage);
    if (fcfg_rc == MQTT_SN_PARSE_SUCCESS) {
        print_system_restarting(logger);
        eeprom_config_cleanup(&ecfg);
        ESP.restart();
    }
    return fcfg_rc;
}

int convert_hostname_port_to_device_address(const MqttSnLogger *logger, const char *hostname, int port, device_address *address,
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
        // FEATURE MDNS_RESOLVER unclear if works - is callled by does not resolve address
        WiFiUDP                udp;
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
    uint32_t  timeout_ms = 30000;
    // ESP32
    if (WiFi.hostByName(hostname, remote_addr)) {
        arduino_ipv4_and_port_to_device_address(&remote_addr, 0, dst);
        return EXIT_SUCCESS;
    }
    // ESP8266
    /*
  if (WiFi.hostByName(hostname, remote_addr, timeout_ms)) {
    arduino_ipv4_and_port_to_device_address(&remote_addr, 0, dst);
    return EXIT_SUCCESS;
  }
  */
    return EXIT_FAILURE;
}

int EndsWith(const char *str, const char *suffix) {
    // from: https://stackoverflow.com/questions/744766/how-to-compare-ends-of-strings-in-c
    if (!str || !suffix) {
        return 0;
    }
    size_t lenstr    = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr) {
        return 0;
    }
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}
int EndsWithLocal(const char *str) {
    return EndsWith(str, ".local");
}
int StartsWith(const char *pre, const char *str) {
    // from: https://stackoverflow.com/questions/4770985/how-to-check-if-a-string-starts-with-another-string-in-c
    size_t lenpre = strlen(pre), lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}

int process_system_config_str(const MqttSnLogger *logger, char *ecfg_line, size_t ecfg_line_len) {
    assert(logger != NULL);

    const char *argv_0 = "ecfg_line";

    if (ecfg_line == NULL) {
        return -1;
    }
    if (ecfg_line_len == 0) {
        return -1;
    }

    if (ecfg_line_len > EEPROM_LINE_SIZE) {
        return -1;
    }
    if (strlen(ecfg_line) + 1 != ecfg_line_len) {
        return -1;
    }
    if (strlen(ecfg_line) == 0) {
        return -1;
    }

    int tk_count = 0;
    {
        char line_copy[EEPROM_LINE_SIZE];
        memcpy(line_copy, ecfg_line, ecfg_line_len);
        // estimates argc
        for (char *tk = strtok(line_copy, " "); tk != NULL; tk = strtok(NULL, " ")) {
            tk_count++;
        }
    }

    char *argv[EEPROM_LINE_SIZE];
    int   argc = 0;
    char  line_copy[EEPROM_LINE_SIZE];
    {
        memcpy(line_copy, ecfg_line, ecfg_line_len);
        argv[argc++] = (char *)argv_0;
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
    print_argc_argv(logger, argc, argv);
#endif

    // process
    int rc = process_system_config_token(logger, argc, argv);
    return rc;
}

int validate_system_config_token(const MqttSnLogger *logger, int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-sr") || !strcmp(argv[i], "--system_restart")) {
            i++;
        } else if (!strcmp(argv[i], "-sh") || !strcmp(argv[i], "--system_help")) {
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
            print_system_restarting(logger);
            ESP.restart();
        }
    }

    return 0;
}
