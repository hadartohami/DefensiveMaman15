#include <iostream>
#include <map>

typedef uint8_t version_t;
typedef uint16_t code_t;
typedef uint32_t csize_t;

constexpr version_t CLIENT_VERSION = 3;
constexpr size_t CLIENT_ID_SIZE = 16;
constexpr size_t CLIENT_NAME_SIZE = 255;
constexpr size_t PUBLIC_KEY_SIZE = 160;  // defined in protocol. 1024 bits.
constexpr size_t SYMMETRIC_KEY_SIZE = 16;   // defined in protocol.  128 bits.
constexpr code_t REGISTRATION_CODE = 1000;

struct RequestHeader {
    std::string client_id;
    const version_t version;
    const code_t code;
    csize_t payload_size;

    RequestHeader(const code_t request_code) : version(CLIENT_VERSION), code(request_code), payload_size(0) {}

    RequestHeader(const std::string &id, const code_t request_code) : client_id(id), version(CLIENT_VERSION),
                                                                       code(request_code),
                                                                       payload_size(0) {}
};

struct RegistrationRequest {
    RequestHeader header(code_t);
    struct {
        std::string client_name;
    }payload;
};
