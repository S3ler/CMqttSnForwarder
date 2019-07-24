//
// Created by SomeDude on 24.07.2019.
//

#include <arpa/inet.h>
#include <memory.h>
#include <netdb.h>
#include "MqttSnClientConnectAction.h"
std::vector<uint8_t> MqttSnClientConnectAction::GetBrokerAddressFromUriAndPort(const std::string &URI,
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

std::vector<uint8_t> MqttSnClientConnectAction::GetPortVectorFromInt32(const int32_t brokerPort) {
  if (brokerPort < -1 || brokerPort > UINT16_MAX) {
    throw std::exception();
  }

  std::vector<uint8_t> brokerPortVector;
  if (brokerPort > -1) {
    uint16_t brokerPortNetworkByteOrder = htons((uint16_t) brokerPort);
    brokerPortVector.push_back((uint8_t) brokerPortNetworkByteOrder & 0xff);
    brokerPortVector.push_back((uint8_t) brokerPortNetworkByteOrder >> 8);
  }
  return brokerPortVector;
}
std::vector<uint8_t> MqttSnClientConnectAction::GetBrokerAddressFromString(const std::string &addressString) {
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
std::vector<uint8_t> MqttSnClientConnectAction::GetBrokerAddressFromUri(const  std::string &URI) {
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
MqttSnClientConnectAction::MqttSnClientConnectAction(const MqttSnClientProtocolType a_protocol,
                                                     const std::vector<uint8_t> &broker_address,
                                                     const std::string &client_id,
                                                     const std::string &client_password,
                                                     const int32_t keep_alive_interval,
                                                     const bool clean_session)
    : MqttSnClientAction(MqttSnClientActionType::CONNECT),
      protocol(a_protocol),
      brokerAddress(broker_address),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}
MqttSnClientConnectAction::MqttSnClientConnectAction(const MqttSnClientProtocolType a_protocol,
                                                     const std::string &URI,
                                                     const int32_t brokerPort,
                                                     const std::string &client_id,
                                                     const std::string &client_password,
                                                     const int32_t keep_alive_interval,
                                                     const bool clean_session)
    : MqttSnClientAction(MqttSnClientActionType::CONNECT),
      protocol(a_protocol),
      brokerAddress(GetBrokerAddressFromUriAndPort(URI, brokerPort)),
      clientId(client_id),
      clientPassword(client_password),
      keepAliveInterval(keep_alive_interval),
      cleanSession(clean_session) {}
