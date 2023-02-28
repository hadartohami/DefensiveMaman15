import sqlite3
import logging
from client import Client

CLIENTS_TABLE = "clients"
FILES_TABLE = "files"


class Database:
    def __init__(self, name):
        self.name = name

    def connect(self):
        conn = sqlite3.connect(self.name)  # doesn't raise exception.
        return conn

    def executescript(self, script):
        conn = self.connect()
        try:
            conn.executescript(script)
            conn.commit()
        except:
            pass  # table might exist already
        conn.close()

    def execute(self, query, args, commit=False, get_last_row=False):
        """
        This function gets query with args and execute it
        Return: results from DB
        """
        results = None
        conn = self.connect()
        try:
            cur = conn.cursor()
            cur.execute(query, args)
            if commit:
                conn.commit()
                results = True
            else:
                results = cur.fetchall()
            if get_last_row:
                results = cur.lastrowid
        except Exception as e:
            logging.exception(f'database execute: {e}')
        conn.close()
        return results

    def initialize(self):
        # create Clients table
        self.executescript(f"""
            CREATE TABLE {CLIENTS_TABLE}(
              ID CHAR(16) NOT NULL PRIMARY KEY,
              Name CHAR(255) NOT NULL,
              PublicKey CHAR(160) NOT NULL,
              LastSeen DATE,
              AESKey CHAR(16)
            );
            """)

        # create files table
        self.executescript(f"""
            CREATE TABLE {FILES_TABLE}(
              ID CHAR(16) NOT NULL PRIMARY KEY,
              FileName CHAR(255) NOT NULL,
              PathName CHAR(255) NOT NULL,
              Verified BOOLEAN,
              FOREIGN KEY(ID) REFERENCES {CLIENTS_TABLE}(ID),
            );
            """)

    def insert_client(self, client):
        if not type(client) is Client:
            return False
        return self.execute(f"INSERT INTO {CLIENTS_TABLE} VALUES (?, ?, ?, ?, ?)",
                            [client.id, client.name, client.public_key, client.last_seen, client.AES_key], True)

    def check_if_client_exist(self, name):
        client = self.execute(
            f"SELECT Name FROM {CLIENTS_TABLE} WHERE Name = ?", [name])
        if not client:
            return False
        return True

    def get_clients(self):
        return self.execute(f"SELECT ID, Name FROM {CLIENTS_TABLE}", [])
