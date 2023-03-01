import struct

SERVER_VERSION = 3
CLIENT_ID_SIZE = 16
HEADER_SIZE_WITHOUT_CLIENT_ID = 7
CLIENT_NAME_SIZE = 255
CLIENT_PUBLIC_KEY_SIZE = 160
CLIENT_CONTENT_SIZE = 4
CLIENT_FILE_NAME = 255


class RequestResponseHandler:
    def __init__(self):
        self.header_size = CLIENT_ID_SIZE + HEADER_SIZE_WITHOUT_CLIENT_ID
        self.client_id = b""
        self.version = SERVER_VERSION
        self.code = None
        self.payload_size = None
        self.payload_data = None
        self.client_name = b""

    def handler_headers(self, data):
        try:
            self.id = struct.unpack(f"<{CLIENT_ID_SIZE}s", data[:CLIENT_ID_SIZE])[0]
            header_data = data[CLIENT_ID_SIZE:CLIENT_ID_SIZE + HEADER_SIZE_WITHOUT_CLIENT_ID]
            self.version, self.code, self.payload_size = struct.unpack("<BHL", header_data)
            return True
        except Exception as e:
            return False

    def print_headers(self):
        print("Client ID: {}\nPayload Size: {}\nCode: {}\nVersion: {}".format(self.client_id,self.payload_size, self.code, self.version))

    def handle_registration_request(self, data):
        try:
            name_data = data[self.header_size:self.header_size + CLIENT_NAME_SIZE]
            self.client_name = str(struct.unpack(f"<{CLIENT_NAME_SIZE}s", name_data)[0].partition(b'\0')[0].decode('utf-8'))
            return True
        except Exception as e:
            return False
