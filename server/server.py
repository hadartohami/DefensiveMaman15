import selectors
import socket
from requestResponseHandler import *
import struct 

SERVER_DB = "server.db"
PACKET_SIZE = 1024


class Server:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.sel = selectors.DefaultSelector()
        self.database = Database(SERVER_DB)
        self.requestResponseHandler = RequestResponseHandler(self.database)
        

    def initilize_database(self):
        self.database.connect()
        self.database.initialize()
    

    def start(self):
        try:
            self.initilize_database()
            sock = socket.socket()
            sock.bind((self.host, int(self.port)))
            sock.listen()
            sock.setblocking(False)
            self.sel.register(sock, selectors.EVENT_READ, self.accept)
            print("Server is listening on port: {}".format(self.port))
            while True:
                events = self.sel.select()
                for key, mask in events:
                    callback = key.data
                    callback(key.fileobj, mask)
        except Exception as e:
            print(e)

    def accept(self, sock, mask):
        conn, addr = sock.accept()  # Should be ready
        print('accepted', conn, 'from', addr)
        conn.setblocking(False)
        self.sel.register(conn, selectors.EVENT_READ, self.read)

    def read(self, conn, mask):
        data = conn.recv(PACKET_SIZE)  # Should be ready
        if data:
            if self.requestResponseHandler.handle_request_headers(data):
                self.requestResponseHandler.print_request_headers() # need to remove
                if self.requestResponseHandler.request_headers.code == REGISTRATION_REQUEST_CODE:
                    if self.requestResponseHandler.handle_registration_request(data):
                        registration_response = self.requestResponseHandler.get_registration_response()
                        self.write(conn, registration_response)
                    else:
                        print("Couldn't handle registration request")
                if self.requestResponseHandler.request_headers.code == SEND_PUBLIC_KEY_REQUEST_CODE:
                    if self.requestResponseHandler.handle_send_public_key_request(data):
                        print("got public key")
                        print(self.requestResponseHandler.public_key)
                        #send_public_key_response = self.requestResponseHandler.get_send_AES_key_response()
                        #self.write(conn, registration_response)
                    else:
                        print("Couldn't handle send public key request")
            else:
                 print("Couldn't parse headers")
        else:
            print('closing', conn)
            self.sel.unregister(conn)
        conn.close()

        while True:
            events = self.sel.select()
            for key, mask in events:
                callback = key.data
                callback(key.fileobj, mask)

    def write(self, conn, data):
        size = len(data)
        sent = 0
        while sent < size:
            leftover = size - sent
            if leftover > PACKET_SIZE:
                leftover = PACKET_SIZE
            to_send = data[sent:sent + leftover]
            if len(to_send) < PACKET_SIZE:
                to_send += bytearray(PACKET_SIZE - len(to_send))
            try:
                conn.send(to_send)
                sent += len(to_send)
            except Exception as e:
                print("Failed to send data, Got error: {} ".format(e))
                return False
        return True