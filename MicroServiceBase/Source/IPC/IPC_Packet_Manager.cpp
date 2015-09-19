/*
    Initial author: (https://github.com/)Convery
    Started: 2015-09-19
    License: LGPL 3.0
    Notes:
        Payload extraction from the packets.
        Operation on the payload.
        Very weak keyexchange.
*/

#include "IPC_Packet.h"
#include "IPC_Packet_Manager.h"
#include <time.h>

// Crypto properties.
uint8_t  IPC_Packet_Manager::Encryptionkey[32];
uint8_t  IPC_Packet_Manager::Exponent;
float    IPC_Packet_Manager::Secret;
uint16_t IPC_Packet_Manager::Base;

// Create the crypto properties.
uint8_t IPC_Packet_Manager::CreateBase()
{
    srand(time(NULL));
    Base = rand();
    return Base;
}
float IPC_Packet_Manager::CreateSecret()
{
    Secret = powf(Base, Exponent);
    return Secret;
}
void IPC_Packet_Manager::CreateSharedbase(uint8_t PeerBase)
{
    Base *= PeerBase;
}
void IPC_Packet_Manager::CreateCryptokey(double PeerSecret)
{
    long double SharedSecret = powl(PeerSecret, Exponent);
    
    // TODO: Hashing of the shared secret.
}

// Fetch the header properties.
uint16_t IPC_Packet_Manager::GetPayloadSize(uint8_t PacketHeader[6])
{
    return ((IPC_RequestHeader *)PacketHeader)->Packet_Length - 4;
}
uint16_t IPC_Packet_Manager::GetTransactionID(uint8_t PacketHeader[6])
{
    return ((IPC_RequestHeader *)PacketHeader)->TransactionID;
}
uint16_t IPC_Packet_Manager::GetResultCode(uint8_t PacketHeader[6])
{
    return ((IPC_ResponseHeader *)PacketHeader)->Result;
}
uint8_t  IPC_Packet_Manager::GetServiceID(uint8_t PacketHeader[6])
{
    return ((IPC_RequestHeader *)PacketHeader)->Request_ServiceID;
}
uint8_t  IPC_Packet_Manager::GetTaskID(uint8_t PacketHeader[6])
{
    return ((IPC_RequestHeader *)PacketHeader)->Request_TaskID;
}
bool     IPC_Packet_Manager::GetOption1(uint8_t PacketHeader[6])
{
    return ((IPC_RequestHeader *)PacketHeader)->Option_Encrypted;
}
bool     IPC_Packet_Manager::GetOption2(uint8_t PacketHeader[6])
{
    return ((IPC_RequestHeader *)PacketHeader)->Option_Compressed;
}
bool     IPC_Packet_Manager::GetOption3(uint8_t PacketHeader[6])
{
    return ((IPC_RequestHeader *)PacketHeader)->Option_Reserved1;
}
bool     IPC_Packet_Manager::GetOption4(uint8_t PacketHeader[6]) 
{
    return ((IPC_RequestHeader *)PacketHeader)->Option_Reserved2;
}

// Work on the payload.
bool EncryptPayload(uint8_t PacketHeader[6], void *Plaintext, void *Ciphertext, uint32_t BufferLength)
{
    return false;
}
bool DecryptPayload(uint8_t PacketHeader[6], void *Ciphertext, void *Plaintext, uint32_t BufferLength)
{
    return false;
}
bool DeflatePayload(void *Plainbuffer, uint32_t Plainlength, void **Compressedbuffer, uint32_t *Compressedlength)
{
    return false;
}
bool InflatePayload(void *Compressedbuffer, uint32_t Compressedlength, void **Plainbuffer, uint32_t *Plainlength)
{
    return false;
}
