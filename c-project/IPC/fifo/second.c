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
    
    int fd = open( "./myfifo", O_RDONLY );
    if ( fd == -1 ) {
        perror( "open myfifo error" );
        return -1;
    } // if

    printf( "open myfifo success\n" );

    char buff[1024];
    while ( strcmp( buff, "quit" ) != 0 ) {
        read( fd, buff, sizeof( buff ) );
        printf( "first process say:%s\n", buff );
    } // while


    printf( "second process close\n" );
    return 0;
} // int