#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <fcntl.h>

int main() {
    
    int fd = open( "./myfifo", O_WRONLY );
    if ( fd == -1 ) {
        perror( "open myfifo error" );
        return -1;
    } // if

    printf( "open myfifo success\n" );

    char buff[1024];
    while ( strcmp( buff, "quit" ) != 0 ) {
        printf( "Enter message:" );
        scanf( "%s", buff );
        write( fd, buff, sizeof( buff ) );
    } // while


    printf( "first process close\n" );
    return 0;
} // int