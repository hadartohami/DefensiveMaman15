import os
from client import Client
from server import Server

FILEPATH = "port.info"
DEFAULT_PORT = 1234
HOST = "127.0.0.1"


def get_port():
    if os.path.exists(FILEPATH):
        with open(FILEPATH) as file_object:
            content = file_object.read()
            return content.split()[0]
    return DEFAULT_PORT


def main():
    port = get_port()
    server = Server(HOST, port)

    # clients = d.get_clients()
    # print(clients)
    server.start()


if __name__ == '__main__':
    main()
