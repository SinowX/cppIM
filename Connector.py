import socket
from Definition import *

class Connector():
    def __init__(self):
        self.sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        pass

    def connectServer(self):
        try:
            self.sock.connect((SERVER_HOST, SERVER_PORT))
        except InterruptedError:
            print("Cant Connect to Server")
        pass

    def sendMsg(self, b_msg):
        self.sock.send(b_msg)
        pass

    def recvMsg(self, size):
        buffer = self.sock.recv(size)
        return buffer
        # print(buffer)
    # def blockingRecv(self,size):
    #     self.sock.recv(si)




    def disconnect(self):
        self.sock.close()
        pass

# client = Connector()
# client.connectServer()
# client.recvMsg()
# client.disconnect()