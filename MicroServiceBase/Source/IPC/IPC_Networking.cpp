/*
    Initial author: (https://github.com/)Convery
    Started: 2015-09-21
    License: LGPL 3.0
    Notes:
        Polls the connections either in threads or manually.
*/

#include "IPC_Networking.h"
#include <mutex>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")

// Connections we poll.
IPC_Connection IPC_Networking::Routerserver;
IPC_Connection IPC_Networking::Webserver;

// Initialize winsock.
static bool Initialized = false;
bool VerifyWinsock()
{
    if (!Initialized)
    {
        WSADATA wsaData;
        int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        const char *ErrorString;

        if (Result != 0)
        {
            switch (Result)
            {
            case WSASYSNOTREADY:
                ErrorString = "WSA system is not ready for networking.";
                break;

            case WSAVERNOTSUPPORTED:
                ErrorString = "WSA system does not support version (2.2).";
                break;

            case WSAEINPROGRESS:
                ErrorString = "WSA is busy with a blocking socket.";
                break;

            case WSAEPROCLIM:
                ErrorString = "Too many processes are using WSA right now.";
                break;

            case WSAEFAULT:
                ErrorString = "Invalid argument data.";
                break;

            default:
                ErrorString = va("Unknown error %d.", Result);
            }

            nDebugPrint("WSA failed for reason: %s", ErrorString);
        }
        else
        {
            Initialized = true;
        }
    }

    return Initialized;
}
void KillWinsock()
{
    if (IPC_Networking::Routerserver.Socket != nullptr)
        closesocket((SOCKET)IPC_Networking::Routerserver.Socket);
    if (IPC_Networking::Webserver.Socket != nullptr)
        closesocket((SOCKET)IPC_Networking::Webserver.Socket);

    WSACleanup();
}

// Connection modifiers.
bool IPC_Networking::Connect(const char *Hostname, uint16_t Port, IPC_Connection &Connection)
{
    // Verify that the WSA system is up and running.
    if (!VerifyWinsock())
        return false;

    // Resolve the IPaddress for the hostname.
    addrinfo *IPAddress = nullptr;
    if (getaddrinfo(Hostname, NULL, NULL, &IPAddress) != 0)
    {
        nDebugPrint("Failed to resolve the address for %s, exiting", Hostname);
        Connection.Socket = nullptr;
        return false;
    }

    // For each of the results, try to connect.
    for (auto Iterator = IPAddress; Iterator != NULL; Iterator = Iterator->ai_next)
    {
        // Create the socket for this connection.
        Connection.Socket = (void *)socket(Iterator->ai_family, Iterator->ai_socktype, Iterator->ai_protocol);
        if (Connection.Socket == (void *)INVALID_SOCKET)
        {
            Connection.Socket = nullptr;
            continue;
        }

        // Attempt to connect.
        register int Error = 0;
        Error = connect((SOCKET)Connection.Socket, Iterator->ai_addr, Iterator->ai_addrlen);
        if (Error == SOCKET_ERROR)
        {
            closesocket((SOCKET)Connection.Socket);
            Connection.Socket = nullptr;
            continue;
        }

        // Connected to the server.
        Connection.Connected = true;
        break;
    }

    // Cleanup.
    freeaddrinfo(IPAddress);

    // Check if we connected.
    if (!Connection.Connected)
    {
        nDebugPrint("Unable to connect to %s.", Hostname);
        return false;
    }
    
    return true;
}
bool IPC_Networking::Disconnect(IPC_Connection &Connection)
{
    Connection.Connected = false;
    return shutdown((SOCKET)Connection.Socket, SD_BOTH) == 0;
}







