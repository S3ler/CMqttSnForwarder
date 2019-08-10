//
// Created by SomeDude on 30.07.2019.
//

#include "MqttSnURLParser.h"
#include <arpa/inet.h>
#include <netdb.h>

std::vector<uint8_t> MqttSnURLParser::GetBrokerAddressFromUriAndPort(const std::string &URI,
                                                                     const int32_t brokerPort) {
  // TODO maybe resolving mDNS
  if (brokerPort < -1 || brokerPort > UINT16_MAX) {
    throw std::exception();
  }

  try {
    std::vector<uint8_t> stringBrokerAddress = GetBrokerAddressFromString(URI);
    std::vector<uint8_t> brokerPortVector = GetPortVectorFromInt32(brokerPort);
    stringBrokerAddress.insert(stringBrokerAddress.end(), brokerPortVector.begin(), brokerPortVector.end());
    return stringBrokerAddress;
  } catch (std::exception exception) {}

  std::vector<uint8_t> uriBrokerAddress = GetBrokerAddressFromUri(URI);
  std::vector<uint8_t> brokerPortVector = GetPortVectorFromInt32(brokerPort);
  uriBrokerAddress.insert(uriBrokerAddress.end(), brokerPortVector.begin(), brokerPortVector.end());
  return uriBrokerAddress;
}

std::vector<uint8_t> MqttSnURLParser::GetPortVectorFromInt32(const int32_t brokerPort) {
  if (brokerPort < -1 || brokerPort > UINT16_MAX) {
    throw std::exception();
  }

  std::vector<uint8_t> brokerPortVector;
  if (brokerPort > -1) {
    uint16_t brokerPortNetworkByteOrder = htons((uint16_t) brokerPort);
    brokerPortVector.push_back((brokerPortNetworkByteOrder >> 8) & 0xff);
    brokerPortVector.push_back(brokerPortNetworkByteOrder & 0xff);
  }
  return brokerPortVector;
}
std::vector<uint8_t> MqttSnURLParser::GetBrokerAddressFromString(const std::string &addressString) {
  std::vector<uint8_t> result;

  char *cp_string = strdup(addressString.c_str());
  char *token = strtok(cp_string, ".");

  while (token != NULL) {
    char *end_prt;
    long int n = strtol(token, &end_prt, 10);
    if (errno == EOVERFLOW) {
      throw std::exception();
    }
    if (*end_prt != '\0') {
      // no conversion performed
      throw std::exception();
    }
    if (n > UINT8_MAX || n < 0) {
      throw std::exception();
    }

    result.push_back(n);
    token = strtok(NULL, ".");
  }
  free(cp_string);

  return result;
}
std::vector<uint8_t> MqttSnURLParser::GetBrokerAddressFromUri(const std::string &URI) {
  std::vector<uint8_t> result;

  struct addrinfo hints = {0};
  struct addrinfo *ainfo, *rp;
  int rc = 0;

  rc = getaddrinfo(URI.c_str(), NULL, &hints, &ainfo);
  if (rc) {
    throw std::exception();
  }
  // prefer ip v4 address
  for (rp = ainfo; rp != NULL; rp = rp->ai_next) {
    if (rp->ai_family == AF_INET) {
      struct sockaddr_in *addr_in = (struct sockaddr_in *) rp->ai_addr;
      char *s = (char *) malloc(INET_ADDRSTRLEN);
      inet_ntop(AF_INET, &(addr_in->sin_addr), s, INET_ADDRSTRLEN);
      std::string s_str(s);
      free(s);
      result = GetBrokerAddressFromString(s_str);
      freeaddrinfo(ainfo);
      return result;
    }
    if (rp->ai_family == AF_INET6) {
      struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *) rp->ai_addr;
      char *s = (char *) malloc(INET_ADDRSTRLEN);
      inet_ntop(AF_INET6, &(addr_in6->sin6_addr), s, INET6_ADDRSTRLEN);
      std::string s_str(s);
      free(s);
      result = GetBrokerAddressFromString(s_str);
      freeaddrinfo(ainfo);
      return result;
    }
  }

  freeaddrinfo(ainfo);
  throw std::exception();
}
bool MqttSnURLParser::isLocalhost(const std::vector<uint8_t> &vector) {
  if (vector.size() >= 4) {
    if (vector[0] == 127 && vector[1] == 0 && vector[2] == 0 && vector[3] == 1) {
      return true;
    }
  }
  return false;
}
std::string MqttSnURLParser::toString(const std::vector<uint8_t> &vector, uint32_t elements) {
  if (elements > vector.size()) {
    throw std::exception();
  }
  std::string result;
  for (uint32_t i = 0; i < elements; i++) {
    result += std::to_string(vector[i]);
    if (i + 1 < elements) {
      result += ".";
    }
  }
  return result;
}
uint16_t MqttSnURLParser::getPort(const std::vector<uint8_t> &vector) {
  if (vector.size() < 2) {
    throw std::exception();
  }
  uint16_t result = 0;
  result += vector[vector.size() - 1];
  result += (vector[vector.size() - 2] << 8);

  return ntohs(result);
}
