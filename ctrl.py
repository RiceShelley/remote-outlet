#!/usr/bin/python3
import socket
import sys

while True:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    sock.connect(("192.168.1.27", 21))
    print(sock.recv(100).decode())
    cmd = input("CMD: ")
    print("Sent: '" + cmd + "'")
    sock.send(cmd.encode())
    sock.close()
