#include <Windows.h>
#include <iostream>

#define BUFFER_MAX_SIZE 1024 * 1024

void WriteData( HANDLE hNamedPipe, char* DATA, DWORD DataLength ) {

    DWORD wrote = 0;

    WriteFile(hNamedPipe, (void*)&DataLength, 4, &wrote, 0);
    FlushFileBuffers(hNamedPipe);

    WriteFile(hNamedPipe, DATA, DataLength, &wrote, 0);
    FlushFileBuffers(hNamedPipe);

    printf("[*] Wrote Data: %s ,Data Size: %i\n", DATA, wrote);

}

DWORD ReadData( HANDLE hNamedPipe, char* DATA ) {
    DWORD  temp = 0;

    int size = 0, total = 0;

    ReadFile( hNamedPipe, (char*)&size, 4, &temp, 0 );

    printf( "[*] Reading Data Size: %i\n", size );


    while ( size > total ) {

        ReadFile( hNamedPipe, DATA + total , size - total, &temp, 0 );
        total += temp;
        printf( "[*] Data: %s\n", DATA );
        printf( "[*] Total : %i\n", total );
        printf( "[*] Size : %i\n", size );

    }

    return size;
}

int main()
{

    HANDLE hPipe;
    char *buf = ( char* )malloc( BUFFER_MAX_SIZE );
    DWORD dwRead;


    hPipe = CreateNamedPipe( TEXT( "\\\\.\\pipe\\ExternalC2Myths" ),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,  
        1,
        1024 * 16,
        1024 * 16,
        NMPWAIT_USE_DEFAULT_WAIT,
        NULL );

    
    while ( hPipe != INVALID_HANDLE_VALUE )
    {
        printf( "[+] Named pipe successfully created.\n" );
        if ( ConnectNamedPipe(hPipe, NULL) != FALSE )   
        {
            printf( "[+] Third-Party client successfully connected.\n" );
            
            while ( TRUE )
            {
                printf( "===================================\n" );

                dwRead = ReadData( hPipe, buf );
                printf( "[*] Reading Data: %s\n", buf );


                printf( "[*] Writing Data.\n" );
                WriteData( hPipe, buf, dwRead + 1 );
            }
        }

    }
    DisconnectNamedPipe( hPipe );
    return 0;
}
