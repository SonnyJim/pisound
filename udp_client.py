#!/usr/bin/python
import socket, sys


IP = "127.0.0.1"
PORT = 8008

if len(sys.argv) > 1:
    MESSAGE = str(sys.argv[1])
else:
    print 'Please specify a message on the command line'
    exit ()

print 'Sending message:', MESSAGE
msg = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
msg.sendto(MESSAGE, (IP, PORT))
