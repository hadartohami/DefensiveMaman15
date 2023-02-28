import struct

SERVER_VERSION = 3
CLIENT_ID_SIZE = 16
HEADER_SIZE = 7
CLIENT_NAME_SIZE = 255
CLIENT_PUBLIC_KEY_SIZE = 160
CLIENT_CONTENT_SIZE = 4
CLIENT_FILE_NAME = 255


class RequestHandler:
    def __init__(self):
        self.header_size = CLIENT_ID_SIZE + HEADER_SIZE
        self.payload_data = None
        self.client_id = b""
        self.version = SERVER_VERSION
        self.code = None
        self.payload_size = None

    def handler_headers(self, data):
        try:
            self.client_id = struct.unpack(
                f"<{CLIENT_ID_SIZE}s", data[:CLIENT_ID_SIZE])[0]
            header_data = data[CLIENT_ID_SIZE:CLIENT_ID_SIZE + HEADER_SIZE]
            self.version, self.code, self.payload_size = struct.unpack(
                "<BHL", header_data)
            # nameData = data[self.header_size:self.header_size]
            # self.payload_data = str(struct.unpack("<BHL", nameData)[0].partition(b'\0')[0].decode('utf-8'))
            return True
        except Exception as e:
            return False

    def print_headers(self):
        print(self.client_id, self.code, self.payload_size)
