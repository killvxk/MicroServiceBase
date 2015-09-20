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
#include "..\Utility\Crypto\SM3.h"
#include "..\Utility\Crypto\AES256.h"
#include "..\Utility\Compression\lz4.h"
#include <time.h>

// Crypto properties.
uint8_t  IPC_Packet_Manager::Encryptionkey[32]{};
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
    sm3((const unsigned char *)&SharedSecret, sizeof(long double), Encryptionkey);
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
bool IPC_Packet_Manager::EncryptPayload(uint8_t PacketHeader[6], void *Plaintext, void *Ciphertext, uint32_t BufferLength)
{
    // Generate the IV from the packet header.
    uint8_t InitialVector[32];
    sm3((const unsigned char *)&PacketHeader, 6, InitialVector);

    // Encrypt.
    ByteArray Encrypted;
    Aes256 aes(Encryptionkey, InitialVector);
    aes.encrypt_start(BufferLength, Encrypted);
    aes.encrypt_continue((const unsigned char *)Plaintext, BufferLength, Encrypted);
    aes.encrypt_end(Encrypted);

    // Copy to the pre-allocated buffer.
    if(Encrypted.size())
        memcpy(Ciphertext, Encrypted.data(), BufferLength);
    return Encrypted.size();
}
bool IPC_Packet_Manager::DecryptPayload(uint8_t PacketHeader[6], void *Ciphertext, void *Plaintext, uint32_t BufferLength)
{
    // Generate the IV from the packet header.
    uint8_t InitialVector[32];
    sm3((const unsigned char *)&PacketHeader, 6, InitialVector);

    // Decrypt.
    ByteArray Decrypted;
    Aes256 aes(Encryptionkey, InitialVector);
    aes.decrypt_start(BufferLength);
    aes.decrypt_continue((const unsigned char *)Ciphertext, BufferLength, Decrypted);
    aes.decrypt_end(Decrypted);

    // Copy to the pre-allocated buffer.
    if (Decrypted.size())
        memcpy(Plaintext, Decrypted.data(), BufferLength);
    return Decrypted.size();
}
bool IPC_Packet_Manager::DeflatePayload(void *Plainbuffer, uint32_t Plainlength, void **Compressedbuffer, uint32_t *Compressedlength)
{
    // Compression rate should be about 2.1 so the buffer is a little large.
    *Compressedbuffer = malloc(Plainlength);
    *Compressedlength = LZ4_compress_fast((const char *)Plainbuffer, (char *)*Compressedbuffer, Plainlength, Plainlength, 1);
    return *Compressedlength != 0;
}
bool IPC_Packet_Manager::InflatePayload(void *Compressedbuffer, uint32_t Compressedlength, void **Plainbuffer, uint32_t *Plainlength)
{
    int InflationResult = 0;

    // Compression rate should be about 2.1 so the buffer is a little large.
    *Plainbuffer = malloc(Compressedlength * 3);
    InflationResult = LZ4_decompress_safe((const char *)Compressedbuffer, (char *)*Plainbuffer, Compressedlength, Compressedlength * 3);

    if (InflationResult < 0)
    {
        free(*Plainbuffer);
        return false;
    }
    else
    {
        *Plainlength = InflationResult;
        return true;
    }
}
