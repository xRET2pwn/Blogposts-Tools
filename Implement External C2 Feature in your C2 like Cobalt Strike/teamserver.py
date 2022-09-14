import socket, struct
from time import sleep

print("""How to Implement External C2 in your C2 like Cobalt Strike..
Author: @RET2_pwn
>>                      [[[Teamserver]]]
>> This python script is not my real teamserver of my C2 framwork.
""")
def send_msg(sock, msg):

    msglength = struct.pack('<I', len(msg)) 
    sock.sendall(msglength)
    sock.sendall(msg.encode())


def recv_msg(sock):

    raw_msglen = recvall(sock, 4)
    if not raw_msglen:
        return None
    msglen = struct.unpack('<I', raw_msglen)[0]

    return recvall(sock, msglen)


def recvall(sock, n):

    data = bytearray()
    while len(data) < n:
        packet = sock.recv(n - len(data))
        if not packet:
            return None
        data.extend(packet)
    return data


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind(("127.0.0.1", 1337))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print(f"Connected by {addr}")
        while True:
            command = input("> ")
            send_msg(conn, command) 
            data = recv_msg(conn)
            print("[*] Received Data: %s" % data.decode())
            