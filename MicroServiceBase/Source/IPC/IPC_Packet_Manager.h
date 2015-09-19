/*
    Initial author: (https://github.com/)Convery
    Started: 2015-09-19
    License: LGPL 3.0
    Notes:
        Payload extraction from the packets.
        Operation on the payload.
        Very weak keyexchange.
*/

#pragma once
#include "..\STDInclude.h"
#include <vector>

class IPC_Packet_Manager
{
    // Crypto properties.
    static uint8_t Encryptionkey[32];
    static uint8_t Base;
    static uint8_t Exponent;

public:
    // Fetch the header properties.
    static uint16_t GetPayloadSize(uint8_t PacketHeader[6]);
    static uint16_t GetTransactionID(uint8_t PacketHeader[6]);
    static uint16_t GetResultCode(uint8_t PacketHeader[6]);
    static uint8_t  GetServiceID(uint8_t PacketHeader[6]);
    static uint8_t  GetTaskID(uint8_t PacketHeader[6]);
    static bool     GetOption1(uint8_t PacketHeader[6]);   // Encrypted.
    static bool     GetOption2(uint8_t PacketHeader[6]);   // Compressed.
    static bool     GetOption3(uint8_t PacketHeader[6]);   // Reserved1.
    static bool     GetOption4(uint8_t PacketHeader[6]);   // Reserved2.

    // Work on the payload.
    static bool EncryptPayload(uint8_t PacketHeader[6], void *Plaintext, void *Ciphertext, uint32_t BufferLength);
    static bool DecryptPayload(uint8_t PacketHeader[6], void *Ciphertext, void *Plaintext, uint32_t BufferLength);
    static bool DeflatePayload(void *Plainbuffer, uint32_t Plainlength, void **Compressedbuffer, uint32_t *Compressedlength);
    static bool InflatePayload(void *Compressedbuffer, uint32_t Compressedlength, void **Plainbuffer, uint32_t *Plainlength);

    // Create the crypto properties.
    static void CreateNewSecret();
    static void CreateSharedbase(uint8_t PeerBase);
    static void CreateCryptokey(uint8_t PeerExponent);
    static uint16_t GetSecret();
};
