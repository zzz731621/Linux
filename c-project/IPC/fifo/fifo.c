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

int main() {
    

    for ( int i = 0; i < 5; i++ ) {
       printf( "this is first process i=%d!\n", i );
        usleep( 100 );
    } // for

    while ( 1 );
    return 0;
} // int