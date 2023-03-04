#include "RsaKeyHandler.h"
#include "Protocol.h"

RequestResponseHandler::RsaKeyHandler(): {
    private_key.Initialize(range,NUMBER_OF_BITS);
    
}

RequestResponseHandler::~RequestResponseHandler(){
}

