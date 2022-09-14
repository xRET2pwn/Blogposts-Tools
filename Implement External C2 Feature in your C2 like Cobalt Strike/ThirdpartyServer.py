import socket, struct
from time import sleep


print("""How to Implement External C2 in your C2 like Cobalt Strike..
Author: @RET2_pwn
>>                    [[[Third-Party Server]]]
>> This python script is not my real Third-party server of my C2 framwork.
""")

def send_msg(sock, msg):


    msglength = struct.pack('<I', len(msg))
    print("[+] Sending Data length: ", msglength)
    sock.sendall(msglength+msg)


def recv_msg(sock):

    raw_msglen = recvall(sock, 4)

    print("[+] Receiving Data: %s" % raw_msglen)

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


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
    server.bind(("127.0.0.1", 31337)) # Start TCP Server to send and receive data between third-party server and client. 
    server.listen()
    conn, addr = server.accept()
    with conn:
        print(f"Connected by {addr}")
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as teamserver_connector:
            teamserver_connector.connect(("127.0.0.1", 1337)) # Intial connection between the teamserver when then third-party client connect with the third-party server.
            while True:
                print("="*40)

                data = recv_msg(teamserver_connector)

                print("[+] Receiving Data: %s" % data)

                send_msg(conn,data)  
                data = recv_msg(conn)
                
                print("[+] Receiving Data: %s" % data)

                send_msg(teamserver_connector,data)
