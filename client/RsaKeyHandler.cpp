#include "RsaKeyHandler.h"
#include "Protocol.h"

RsaKeyHandler::RsaKeyHandler(std::string &key){
	CryptoPP::StringSource prv_key(key, true);
	_privateKey.Load(prv_key);
}

RsaKeyHandler::RsaKeyHandler(): {
    private_key.Initialize(range,NUMBER_OF_BITS);
    CryptoPP::StringSource prv_key(key, true);
	_privateKey.Load(prv_key);
}

std::string RsaKeyHandler::get_private_key(){
    std::string prv_key;
	CryptoPP::StringSink pvk(prv_key);
	_privateKey.Save(pvk);
	return prv_key;
}

std::string RsaKeyHandler::get_public_key(){
	const CryptoPP::RSAFunction this->public_key((this->private_Key));
	std::string pub_key;
	CryptoPP::StringSink pubk(pub_key);
	publicKey.Save(pubk);
	return pub_key;
}


std::string RsaKeyHandler::encrypt(const uint8_t* plain, size_t length)
{
	std::string cipher;
	CryptoPP::RSAES_OAEP_SHA_Encryptor e(this->public_key);
	CryptoPP::StringSource ss(plain, length, true, new CryptoPP::PK_EncryptorFilter(range, e, new CryptoPP::StringSink(cipher)));
	return cipher;
}


std::string RsaKeyHandler::decrypt(const uint8_t* cipher, size_t length)
{
	std::string decrypted;
	CryptoPP::RSAES_OAEP_SHA_Decryptor d(this->private_Key);
	CryptoPP::StringSource ss_cipher((cipher), length, true, new CryptoPP::PK_DecryptorFilter(range, d, new CryptoPP::StringSink(decrypted)));
	return decrypted;
}