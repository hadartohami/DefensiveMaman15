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
    ~RsaKeyHandler();
};
#endif
