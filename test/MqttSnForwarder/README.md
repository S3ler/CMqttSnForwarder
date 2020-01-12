The MqttSnForwarder is Integration tested.
We reuse the MockMqttSnClientNetwork, MockMqttSnGatewayNetwork and the MqttSnFixedSizeRingBuffer to test the different functions.

MockClientNetwork -- ClientNetworkInterfaceTests
MockGatewayNetwork -- GatewayNetworkInterfaceTests

ClientNetworkImplementationTests - ClientNetworkTcpImplementation + MockTcpClient
GatewayNetworkImplementationTests - GatewayNetworkTcpImplementation + MockTcpGateway
ClientNetworkImplementationTests - ClientNetworkUdpImplementation + MockUdpClient
GatewayNetworkImplementationTests - GatewayNetworkUdpImplementation + MockUdpGateway

MockClientNetwork -- ClientNetworkInterfaceTests -- Forwarder -- GatewayNetworkInterfaceTests -- MockGatewayNetwork

ClientNetworkTcpImplementation -- ClientNetworkInterfaceTests -- Forwarder -- GatewayNetworkInterface -- GatewayNetworkTcpImplementation
ClientNetworkUdpImplementation -- ClientNetworkInterfaceTests -- Forwarder -- GatewayNetworkInterface -- GatewayNetworkUdpImplementation

ClientNetworkTcpImplementation -- ClientNetworkInterfaceTests -- Forwarder -- GatewayNetworkInterface -- GatewayNetworkUdpImplementation
ClientNetworkUdpImplementation -- ClientNetworkInterfaceTests -- Forwarder -- GatewayNetworkInterface -- GatewayNetworkTcpImplementation



/ send by gateway => received by forwarder
// send by forwarder => received by gateway
// send by gateway => received by forwarder => send back to gateway
// send by forwarder => received by gateway => send back to forwarder

// Forwarder nutzt die MqttSnClientNetworkInterface
// Forwarder nutzt MqttSnGatewayNetworkInterface

// Forwarder nutzt RingBuffer (hier vielleicht eine Heap-Allocation-Based Queue noch implementieren und austauschbar gestalten

// Behaviour-Black-Box Test
// function testing

// Behaviour-Tests for Interface (if the interface behaves correctly corresponding to the contract)

// Wenn ich den MqttSnRingBuffer mocke, so kann ich über das Mock Object das zeug Testen

// Unit testing for: RingBuffer, MqttSnForwarder,