import os
import uuid
from database import Database
from client import Client
from server import Server

FILEPATH = "port.info"
DEFAULT_PORT = 1234
SERVER_DB = "server.db"
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

    d = Database(SERVER_DB)
    d.connect()
    d.initialize()
    name = "hadar"
    if d.check_if_client_exist(name) is False:
        c = Client(uuid.uuid4().hex, "hadar", "", "", "")
        d.insert_client(c)
    else:
        print("Client with name: {} already exist".format(name))

    # clients = d.get_clients()
    # print(clients)
    server.start()


if __name__ == '__main__':
    main()
