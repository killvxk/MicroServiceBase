/*
    Initial author: (https://github.com/)Convery
    Started: 2015-09-19
    License: LGPL 3.0
    Notes:
        IPC packet definitions.
*/

#pragma once

struct IPC_RequestHeader
{
    // The total length of the packet - 16bit.
    unsigned int Packet_Length : 16;

    // Identifier for the response.
    unsigned int TransactionID : 16;

    // Details from the routing server.
    unsigned int Request_ServiceID : 7;
    unsigned int Request_TaskID : 5;
    
    // Payload options.
    unsigned int Option_Encrypted : 1;
    unsigned int Option_Compressed : 1;
    unsigned int Option_Reserved1 : 1;
    unsigned int Option_Reserved2 : 1;
};
struct IPC_ResponseHeader
{
    // The total length of the packet - 16bit.
    unsigned int Packet_Length : 16;

    // Identifier for the response.
    unsigned int TransactionID : 16;

    // Error code or info.
    unsigned int Result : 12;

    // Payload options.
    unsigned int Option_Encrypted : 1;
    unsigned int Option_Compressed : 1;
    unsigned int Option_Reserved1 : 1;
    unsigned int Option_Reserved2 : 1;
};

struct IPC_Packet
{
    union
    {
        IPC_RequestHeader Request;
        IPC_ResponseHeader Response;
    } Header;

    unsigned int PayloadLength;
    void *PayloadData;
};
