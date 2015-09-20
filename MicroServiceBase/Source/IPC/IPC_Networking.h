/*
    Initial author: (https://github.com/)Convery
    Started: 2015-09-21
    License: LGPL 3.0
    Notes:
        Polls the connections either in threads or manually.
*/

#pragma once
#include "IPC_Packet.h"
#include "..\STDInclude.h"
#include <vector>

// Socket or handle for a connection.
struct IPC_Connection
{
    void *Socket{ nullptr };
    bool Connected{ false };
    bool Authenticated{ false };
};

// General callbacks to handle packets.
using IPC_Callback = bool(__stdcall *)(IPC_Packet &Packet);

// Use the IPC_Connection as a socket.
class IPC_Networking
{
public:
    // Connections we poll.
    static IPC_Connection Routerserver;
    static IPC_Connection Webserver;

    // Connection modifiers.
    static bool Connect(const char *Hostname, uint16_t Port, IPC_Connection &Connection);
    static bool Disconnect(IPC_Connection &Connection);

    // Poll the connections for data.
    static bool Blocking_Recv(IPC_Connection &Connection, IPC_Packet &Packet);                                                      // Waits for a packet and returns.
    static bool Blocking_Recv_Callback(IPC_Connection &Connection, IPC_Callback Callback);                                          // Waits for a packet and calls the callback.
    static void Blocking_Multiple_Recv(IPC_Connection &Connection, uint32_t PacketCount, std::vector<IPC_Packet> &PacketArray);     // Waits for n packets and returns.
    static void Blocking_Multiple_Callback(IPC_Connection &Connection, uint32_t PacketCount, std::vector<IPC_Callback> &Callbacks); // Waits for n packets and calls the callbacks in order until one returns true.
    static void Blocking_Recv_Foreach(IPC_Connection &Connection, uint32_t MaxPackets, std::vector<IPC_Callback> &Callbacks);       // For each packet, call the callbacks in order until one returns true.
    static void Async_Recv_Callback(IPC_Connection &Connection, IPC_Callback Callback);                                             // Waits for a packet and calls the callback.

    // Send data to a connection.
    static bool Blocking_Send(IPC_Connection &Connection, void *Buffer, uint32_t Length);   // Waits until the socket is available and sends the data.
    static void Async_Send(IPC_Connection &Connection, void *Buffer, uint32_t Length);      // Waits until the socket is available and sends the data.
};
