#!/usr/bin/python
import socket
import sys


IP = "127.0.0.1"
PORT = 8008
MESSAGE = str(sys.argv[1])

if len(MESSAGE) > 0:
    print 'Sending message:', MESSAGE
    msg = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    msg.sendto(MESSAGE, (IP, PORT))
else:
    print 'Please specify a message on the command line'
