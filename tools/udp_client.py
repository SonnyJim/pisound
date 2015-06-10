#!/usr/bin/python
import socket, sys, thread, struct
import tty, termios

#RIP = "192.168.1.80"
RIP = "127.0.0.1"

IP = "192.168.1.76"
#IP = "127.0.0.1"
PORT = 8008
RPORT = 8009
RECV_BUFFSIZE = 1024
running = 0
data = ""

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
    sockr.bind ((RIP, RPORT))
    while running:
        recv_data = sockr.recvfrom(RECV_BUFFSIZE)
        print "\nReceived: ", recv_data[0], "\n"

#Thread to send the UDP messages
def send_thread ():
    global running, data
    while running:
        readkey = getch ()

        if readkey == "=":
            data = struct.pack("B", 3)
            print "Volume up"
        elif readkey == "-":
            data = struct.pack("B", 4)
            print "Volume down"
        elif readkey == "v":
            vol = raw_input ("Enter in volume (0-127): ")
            data += struct.pack("B", 0x05)
            data += struct.pack("B", int(vol))
        elif readkey == "s":
            sound = raw_input ("Enter in sound code (0-127): ")
            data = struct.pack ("B", 0x00)
            data += struct.pack ("B", int(sound))
        elif readkey == "m":
            music = raw_input ("Enter in music code (0-127): ")
            data = struct.pack ("B", 0x01)
            data += struct.pack ("B", int(music))
        elif readkey == "p":
            print "PING"
            data = struct.pack ("B", 0xfe)
        elif readkey == "Q":
            print "Shutting down pisound server"
            data = struct.pack ("B", 0xfd)
        elif readkey == "c":
            scene = raw_input ("Enter scene number: ")
            data = struct.pack ("B", 0xfa)
            data += struct.pack ("B", int(scene))
        elif readkey == "C":
            scene = raw_input ("Enter subscene number: ")
            data = struct.pack ("B", 0xfb)
            data += struct.pack ("B", int(scene))
        elif readkey == "t":
            trans = raw_input ("Enter transition effect: ")
            data = struct.pack ("B", 0xfc)
            data += struct.pack ("B", int(trans))
        elif readkey == "S":
            print "Not implemented"
        elif readkey == "P":
            player = raw_input ("Enter player number: ")
            data = struct.pack ("B", 0xe1)
            data += struct.pack ("B", int(player))
        elif readkey == "q":
            running = 0

        sock.sendto(data, (IP, PORT))
        data = ""

running = 1
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sockr = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
print "Simple UDP test client"
print "======================"
print "\n+/- Volume up/down"
print "s            Send sound code"
print "m            Send music code"
print "v            Set volume"
print "c            Change scene"
print "c            Change subscene"
print "t            Change transition effect"
print "S            Send score"
print "P            Change player"
print "p            Ping!"
print "M            Send custom message"
print "Q            Shutdown pisound"
print "q            Quit UDP client"

thread.start_new_thread (recv_thread, ())
send_thread ()
