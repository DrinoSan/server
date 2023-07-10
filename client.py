import socket               # Import socket module
from time import sleep
s = socket.socket()         # Create a socket object
s.connect(('localhost', 8000))

s.sendall("Hello from client\r\n\r\n".encode())
msg = s.recv(21)
print(msg.decode())
print("-"* 40)

s.sendall("Ah Hi server\r\n\r\n".encode())
msg = s.recv(16)
print(msg.decode())



s.close()                     # Close the socket when done