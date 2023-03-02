#include <iostream>
#include <map>

typedef uint8_t version_t;
typedef uint16_t code_t;
typedef uint32_t csize_t;

constexpr version_t SERVER_VERSION = 3;
constexpr size_t CLIENT_ID_SIZE = 16;
constexpr size_t CLIENT_NAME_SIZE = 255;
constexpr size_t PUBLIC_KEY_SIZE = 160;  // defined in protocol. 1024 bits.
constexpr size_t SYMMETRIC_KEY_SIZE = 16;   // defined in protocol.  128 bits.
constexpr code_t REQUEST_REGISTRATION_CODE = 1100;
constexpr code_t REQUEST_SEND_PUBLIC_KEY_CODE = 1101;


#pragma pack(push, 1)

struct ClientID
{
	uint8_t uuid[CLIENT_ID_SIZE];
	ClientID() : uuid{ 0 } {}	
};

struct ClientName
{
	uint8_t name[CLIENT_NAME_SIZE];  
	ClientName() : name{ '\0' } {}
};

struct RequestHeader {
    ClientID client_id;
    const version_t version;
    const code_t code;
    csize_t payload_size;

    RequestHeader(const code_t request_code) : version(SERVER_VERSION), code(request_code), payload_size(0) {}
	RequestHeader(const ClientID& id, const code_t request_code) : client_id(id), version(SERVER_VERSION), code(request_code), payload_size(0) {}

};

struct ResponseHeader {
    const version_t version;
    const code_t code;
    csize_t payload_size;
    ResponseHeader() : version(0), code(0), payload_size(0) {}

};

struct RequestRegistration
{
	RequestHeader header;
	struct
	{
		ClientName client_name;
	}payload;
	RequestRegistration() : header(REQUEST_REGISTRATION_CODE) {}
};

struct ResponseRegistration
{
	ResponseHeader header;
    ClientID payload;
};

#pragma pack(pop)
