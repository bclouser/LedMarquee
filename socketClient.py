import socket

clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientsocket.connect(('192.168.1.12', 1336))
clientsocket.send('The Cake is a lie')
