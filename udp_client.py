#!/usr/bin/python
import socket, sys, thread
import tty, termios

IP = "127.0.0.1"
PORT = 8008
RECV_BUFFSIZE = 1024
running = 0
message = ""

def getch ():
    old_settings = termios.tcgetattr(0)
    new_settings = old_settings[:]
    new_settings[3] &= ~termios.ICANON
    new_settings[3] = new_settings[3] & ~termios.ECHO
    try:
        termios.tcsetattr(0, termios.TCSANOW, new_settings)
        ch = sys.stdin.read(1)
        #Turn off key echo
        fd = sys.stdin.fileno()
        tty.setcbreak(fd)
    finally:
        termios.tcsetattr(0, termios.TCSANOW, old_settings)
    return ch

#Thread to receive the UDP responses
def recv_thread ():
    global running
    while running:
        recv_data = sock.recvfrom(RECV_BUFFSIZE)
        print "\nReceived: ", recv_data[0], "\n"

#Thread to send the UDP messages
def send_thread ():
    global running, message
    while running:
        readkey = getch ()

        if readkey == "=":
            message = "03"
            print "Volume up"
        elif readkey == "-":
            message = "04"
            print "Volume down"
        elif readkey == "v":
            vol = raw_input ("Enter in volume (0-ff): ")
            message = "05"
            message = message + vol
        elif readkey == "s":
            sound = raw_input ("Enter in sound code (0-ff): ")
            message = "00"
            message = message + sound
        elif readkey == "m":
            music = raw_input ("Enter in music code (0-ff): ")
            message = "01"
            message = message + music
        elif readkey == "1":
            message = "0000"
        elif readkey == "2":
            message = "0001"
        elif readkey == "3":
            message = "0002"
        elif readkey == "4":
            message = "0003"
        elif readkey == "5":
            message = "0004"
        elif readkey == "p":
            message = "fe"
        elif readkey == "q":
            running = 0;

        if len(message) > 0:
            sock.sendto(message, (IP, PORT))
        message = ""

running = 1
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
print "Simple UDP test client"
print "======================"
print "\n+/- Volume up/down"
print "s send sound"
print "m send music"
print "v set volume"
print "p ping!"
print "q quit"

thread.start_new_thread (recv_thread, ())
send_thread ()
