import selectors
import socket
from requestHandler import *
import struct 

PACKET_SIZE = 1024


class Server:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.sel = selectors.DefaultSelector()
        self.requestHandler = RequestHandler()

    def start(self):
        try:
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
            self.requestHandler.handler_headers(data)
            self.requestHandler.print_headers()
            if self.requestHandler.handle_registration_request(data):
                print("Client Name: {}".format(self.requestHandler.client_name))
            #self.write(conn, "testing")
            # get_headers = self.requestHandler.handler_headers(data)
            # if get_headers:
            #     self.requestHandler.print_headers()
            #     #print('echoing', repr(data), 'to', conn)
            # else:
            #     print("Couldn't parse headers")
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