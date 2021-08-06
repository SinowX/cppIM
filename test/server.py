import socket

HOST = '127.0.0.1'
PORT = 5000
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen()

conn,addr = s.accept()

data = b'asdjioqwjeiqmwkldmsasg'
print(data.__sizeof__())

conn.send(data)
conn.close()
s.close()