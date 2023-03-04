#ifndef RSAKEYHANDLER_H_
#define RSAKEYHANDLER_H_
#include <iostream>
#include "rsa.h"
#include "protocol.h"

static constexpr size_t NUMBER_OF_BITS = 1024;

class RsaKeyHandler{
private:
    static constexpr size_t KEYSIZE = PUBLIC_KEY_SIZE;
    CryptoPP::AutoSeededRandomPool range;
	CryptoPP::RSA::PublicKey       public_key;
    CryptoPP::RSA::PrivateKey      private_Key;

public:
    RsaKeyHandler();
    RsaKeyHandler(std::string &key);
    ~RsaKeyHandler();
    std::string RsaKeyHandler::get_private_key();
    std::string RsaKeyHandler::get_public_key();
    std::string RsaKeyHandler::encrypt(const uint8_t* plain, size_t length);
    std::string RsaKeyHandler::decrypt(const uint8_t* cipher, size_t length);
};
#endif
