
#include "config.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "forwarder_config.h"

void forwarder_config_init(forwarder_config *fcfg) {
  memset(fcfg, 0, sizeof(*fcfg));
  fcfg->protocol_version = MQTT_SN_PROTOCOL_V1;

  fcfg->mqtt_sn_gateway_host = strdup("localhost");
  fcfg->mqtt_sn_gateway_port = 8888;
  fcfg->gateway_network_bind_port = 9999;
  fcfg->client_network_bind_port = 7777;
}

void forwarder_config_cleanup(forwarder_config *fcfg) {
  free(fcfg->mqtt_sn_gateway_host);
}

int process_forwarder_config_line(forwarder_config *fcfg, int argc, char *argv[]) {

  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--mqtt_sn_gateway_host")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no host specified.\n\n", argv[i]);
        return 1;
      } else {
        free(fcfg->mqtt_sn_gateway_host); // free default value
        fcfg->mqtt_sn_gateway_host = strdup(argv[i + 1]);
      }
      i++;
    } else if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--mqtt_sn_gateway_port")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no port specified.\n\n", argv[i]);
        return 1;
      } else {
        fcfg->mqtt_sn_gateway_port = atoi(argv[i + 1]);
        if (fcfg->mqtt_sn_gateway_port < 1 || fcfg->mqtt_sn_gateway_port > 65535) {
          fprintf(stderr, "Error: Invalid port given: %d\n", fcfg->mqtt_sn_gateway_port);
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-L") || !strcmp(argv[i], "--url")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no URL specified.\n\n", argv[i]);
        return 1;
      } else {
        char *url = argv[i + 1];

        if (!strncasecmp(url, "mqttsn://", 7 + 2)) {
          url += 7;
          fcfg->mqtt_sn_gateway_port = 8888;
        } else {
          fprintf(stderr, "Error: unsupported URL scheme.\n\n");
          return 1;
        }

        fcfg->mqtt_sn_gateway_host = url + 2;
        char *tmp = strchr(url, ':');
        if (tmp) {
          *tmp++ = 0;
          fcfg->mqtt_sn_gateway_port = atoi(tmp);
          if (fcfg->mqtt_sn_gateway_port < 1 || fcfg->mqtt_sn_gateway_port > 65535) {
            fprintf(stderr, "Error: Invalid port given: %d\n", fcfg->mqtt_sn_gateway_port);
            return 1;
          }
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-gA") || !strcmp(argv[i], "--gateway_network_bind_address")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no address specified.\n\n", argv[i]);
        return 1;
      } else {
        fcfg->gateway_network_bind_address = strdup(argv[i + 1]);
      }
      i++;
    } else if (!strcmp(argv[i], "-gp") || !strcmp(argv[i], "--gateway_network_bind_port")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no port specified.\n\n", argv[i]);
        return 1;
      } else {
        fcfg->gateway_network_bind_port = atoi(argv[i + 1]);
        if (fcfg->gateway_network_bind_port < 1 || fcfg->gateway_network_bind_port > 65535) {
          fprintf(stderr, "Error: Invalid port given: %d\n", fcfg->gateway_network_bind_port);
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-gL") || !strcmp(argv[i], "--gateway_network_url")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no URL specified.\n\n", argv[i]);
        return 1;
      } else {
        char *url = argv[i + 1];

        fcfg->gateway_network_bind_address = url;
        char *tmp = strchr(url, ':');
        if (tmp) {
          *tmp++ = 0;
          fcfg->gateway_network_bind_port = atoi(tmp);
          if (fcfg->gateway_network_bind_port < 1 || fcfg->gateway_network_bind_port > 65535) {
            fprintf(stderr, "Error: Invalid port given: %d\n", fcfg->gateway_network_bind_port);
            return 1;
          }
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-cA") || !strcmp(argv[i], "--client_network_bind_address")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no address specified.\n\n", argv[i]);
        return 1;
      } else {
        fcfg->client_network_bind_address = strdup(argv[i + 1]);
      }
      i++;
    } else if (!strcmp(argv[i], "-gp") || !strcmp(argv[i], "--client_network_bind_port")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no port specified.\n\n", argv[i]);
        return 1;
      } else {
        fcfg->client_network_bind_port = atoi(argv[i + 1]);
        if (fcfg->client_network_bind_port < 1 || fcfg->client_network_bind_port > 65535) {
          fprintf(stderr, "Error: Invalid port given: %d\n", fcfg->client_network_bind_port);
          return 1;
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-cL") || !strcmp(argv[i], "--client_network_url")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no URL specified.\n\n", argv[i]);
        return 1;
      } else {
        char *url = argv[i + 1];

        fcfg->client_network_bind_address = url;
        char *tmp = strchr(url, ':');
        if (tmp) {
          *tmp++ = 0;
          fcfg->client_network_bind_port = atoi(tmp);
          if (fcfg->client_network_bind_port < 1 || fcfg->client_network_bind_port > 65535) {
            fprintf(stderr, "Error: Invalid port given: %d\n", fcfg->client_network_bind_port);
            return 1;
          }
        }
      }
      i++;
    } else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")) {
      fcfg->debug = 1;
    } else if (!strcmp(argv[i], "--help")) {
      return 2;
    } else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--mqtt_sn_gateway_host")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no host specified.\n\n", argv[i]);
        return 1;
      } else {
        fcfg->mqtt_sn_gateway_host = strdup(argv[i + 1]);
      }
      i++;
    } else if (!strcmp(argv[i], "-V") || !strcmp(argv[i], "--protocol-version")) {
      if (i == argc - 1) {
        fprintf(stderr, "Error: %s argument given but no version specified.\n\n", argv[i]);
        return 1;
      } else {
        if (!strcmp(argv[i + 1], "mqttsnv1")) {
          fcfg->protocol_version = MQTT_SN_PROTOCOL_V1;
        } else {
          fprintf(stderr, "Error: Invalid protocol version argument given.\n\n");
          return 1;
        }
        i++;
      }
    } else if (!strcmp(argv[i], "--quiet")) {
      fcfg->quiet = 1;
    } else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose")) {
      fcfg->verbose = 1;
    } else {
      fprintf(stderr, "Error: Unknown option '%s'.\n", argv[i]);
      return 1;
    }
  }

  return 0;
}

void print_usage(void) {
  char *VERSION = "0";
  int major, minor, revision;
  major = 0;
  minor = 0;
  revision = 0;
  // mosquitto_lib_version(&major, &minor, &revision);
  printf("cmqttsnforwarder is a simple mqtt-sn forwarder that will forward udp mqtt-sn messages\n");
  printf("                 from a client network over a gateway network to a mqtt-sn gateway.\n");
  printf("cmqttsnforwarder version %s is %d.%d.%d.\n\n", VERSION, major, minor, revision);
  printf("Usage: cmqttsnforwarder {[[-h mqtt_sn_gateway_host] [-p mqtt_sn_gateway_port]] | -L URL}\n");
  printf("                        {[[-gA gateway_network_bind_address] [-gp gateway_network_bind_port]] | -gL URL}\n");
  printf("                        {[[-cA client_network_bind_address] [-cp client_network_bind_port]] | -cL URL}\n");
  printf("                        [-c config_file]\n");
  printf("                        [-v] [--quiet]\n");

  printf("       cmqttsnforwarder --help\n\n");
  printf(" -h : mqtt-sn gateway host to connect to. Defaults to localhost.\n");
  printf(" -p : mqtt-sn gateway network port to connect to. Defaults to 8888 for plain MQTT-SN.\n");
  printf(" -L : specify hostname, port as a URL in the form:\n");
  printf("      mqtt-sn(s)://host[:port]\n");

  printf(" -gA : bind the gateway network to the outgoing socket to this host/ip address.\n");
  printf("       Use to control which interface the network communicates over.\n");
  printf(" -gp : listening on the specific gateway network port. Defaults to 9999.\n");
  printf(" -gL : specify outgoing socket, port as a URL in the form: address[:port]\n");

  printf(" -cA : bind the client network to the outgoing socket to this host/ip address.\n");
  printf("       Use to control which interface the network communicates over.\n");
  printf(" -cp : listening on the specific client network port. Defaults to 7777.\n");
  printf(" -cL : specify outgoing socket, port as a URL in the form: address[:port]\n");

  printf(" -c : specify the forwarder config file.\n");
  printf(" -V : specify the version of the MQTT-SN protocol to use.\n");
  printf("      Can be mqttsnv1. Defaults to mqttsnv1.\n");
  printf(" -v : verbose mode - enable all logging types.\n");
  printf(" --quiet : don't print error messages.\n");
  printf(" --help : display this message.\n");

  printf("\nSee http://./ for more information.\n\n");
}
