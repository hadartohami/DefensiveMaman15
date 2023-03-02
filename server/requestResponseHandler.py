import struct
from database import Database
from client import Client
from datetime import datetime
import uuid

SERVER_VERSION = 3
CLIENT_ID_SIZE = 16
HEADER_SIZE_WITHOUT_CLIENT_ID = 7
CLIENT_NAME_SIZE = 255
CLIENT_PUBLIC_KEY_SIZE = 160
CLIENT_CONTENT_SIZE = 4
CLIENT_FILE_NAME = 255
DEFAULT_VALUE = 0


REGISTRATION_REQUEST_CODE = 1100



REGISTRATION_RESPONSE_CODE = 2100
REGISTRATION_RESPONSE_ERROR_CODE = 2101


class RequestHeaders:
    def __init__(self):
        self.version = SERVER_VERSION
        self.header_size = CLIENT_ID_SIZE + HEADER_SIZE_WITHOUT_CLIENT_ID
        self.code = None
        self.payload_size : DEFAULT_VALUE
        

class ResponseHeaders:
    def __init__(self):
        self.version = SERVER_VERSION
        self.header_size = HEADER_SIZE_WITHOUT_CLIENT_ID
        self.code = None
        self.client_id = b""
        self.payload_size = DEFAULT_VALUE

class RequestResponseHandler:
    def __init__(self, database):
        self.database = database
        self.client_name = b""
        self.request_headers = RequestHeaders()
        self.response_headers = ResponseHeaders()

    def handle_request_headers(self, data):
        try:
            self.request_headers.client_id = struct.unpack(f"<{CLIENT_ID_SIZE}s", data[:CLIENT_ID_SIZE])[0]
            header_data = data[CLIENT_ID_SIZE:CLIENT_ID_SIZE + HEADER_SIZE_WITHOUT_CLIENT_ID]
            self.request_headers.version, self.request_headers.code, self.request_headers.payload_size = struct.unpack("<BHL", header_data)
            return True
        except Exception as e:
            return False

    def print_request_headers(self):
        print("Client ID: {}\nPayload Size: {}\nCode: {}\nVersion: {}".format(self.request_headers.client_id, self.request_headers.payload_size, self.request_headers.code, self.request_headers.version))

    def handle_registration_request(self, data):
        try:
            name_data = data[self.request_headers.header_size:self.request_headers.header_size + CLIENT_NAME_SIZE]
            self.client_name = str(struct.unpack(f"<{CLIENT_NAME_SIZE}s", name_data)[0].partition(b'\0')[0].decode('utf-8'))
            return True
        except Exception as e:
            return False
    
    def get_registration_response(self):
        self.response_headers.payload_size = CLIENT_ID_SIZE
        try:
            if self.client_name:
                if self.database.check_if_client_exist(self.client_name) is False:
                    clnt = Client(uuid.uuid4().hex, self.client_name, "", str(datetime.now()), "")
                    client_id = clnt.id
                    self.response_headers.client_id = client_id
                    self.database.insert_client(clnt)
                    data += struct.pack(f"<{CLIENT_ID_SIZE}s", self.response_headers.client_id)
                    self.response_headers.code = REGISTRATION_RESPONSE_CODE
                    data = self.pack_reponse_headers()
                    return data
                else:
                    raise Exception("Client with name: {} already exist".format(self.client_name))
        except Exception as e:
            self.response_headers.code = REGISTRATION_RESPONSE_ERROR_CODE
            data = self.pack_reponse_headers()
            return data

    def pack_reponse_headers(self):
        return struct.pack("<BHL", self.response_headers.version, self.response_headers.code, self.response_headers.payload_size)



