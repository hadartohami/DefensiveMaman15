
class Client:
    def __init__(self, id, name, public_key, last_seen, AES_key):
        self.name = name
        self.id = bytes.fromhex(id)
        self.public_key = public_key
        self.last_seen = last_seen
        self.AES_key = AES_key
