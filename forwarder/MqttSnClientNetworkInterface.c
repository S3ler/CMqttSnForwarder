//
// Created by bele on 28.01.19.
//

#include "MqttSnClientNetworkInterface.h"

int ClientNetworkInit(MqttSnClientNetworkInterface *n, void *context,
                      int (*client_network_init)(struct MqttSnClientNetworkInterface *, void *context)) {
    n->status = 0;
    n->client_network_init = client_network_init;
    n->client_network_init(n, context);
}

int ClientNetworkConnect(MqttSnClientNetworkInterface *n, void *context) {
    if (n->client_network_connect(n, context) == 0) {
        n->status = 1;
        return 0;
    } else {
        n->status = -1;
    }
    return n->status;
}

void ClientNetworkDisconnect(MqttSnClientNetworkInterface *n, void *context) {
    n->client_network_disconnect(n, context);
    n->status = -1;
}
