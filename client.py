import socket               # Import socket module
from time import sleep
s = socket.socket()         # Create a socket object
s.connect(('localhost', 8000))

sendText = "GET / HTTP/1.1\r\nHost: DRINOOOOSAN\r\nHello from client\r\n\r\n"
s.sendall(sendText.encode())
msg = s.recv(len(sendText))
print(msg.decode())
print("-"* 40)

secondSentText = "Ah Hi server\r\n\r\n"
s.sendall(secondSentText.encode())
msg = s.recv(len(secondSentText))
print(msg.decode())



s.close()                     # Close the socket when done