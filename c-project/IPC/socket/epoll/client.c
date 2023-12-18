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

#define PORT 9999

struct data {
    char buff[1024];
    char name[1024];
};

void* working( void *arg ) {
    // char buff[1024];
    int fd = *( int * )arg;
    struct data msg;
    while ( 1 ) {
        // 收資料
        memset( msg.buff, 0, sizeof( msg.buff ) );
        memset( msg.name, 0, sizeof( msg.name ) );
        int len = recv( fd, &msg, sizeof( msg ), 0 );
        if ( len > 0 ) {
            printf( "%s say: %s\n", msg.name, msg.buff );
            // send( fd, buff, len, 0 );
        } // if
        else if ( len == 0 ) {
            printf( "server offline...\n" );
            break;
        } // else if
        else {
            perror( "recv" );
            break;
        } // else

    } // while

} // void
int main() {

    // 創建溝通用的socket
    int fd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( fd == -1 ) {
        perror( "error" );
        return -1;
    } // if

    // 連接server的IP port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons( PORT );
    inet_pton( AF_INET, "172.17.0.2", &saddr.sin_addr.s_addr );
    // saddr.sin_addr.s_addr = "INADDR_ANY";

    int ret = connect( fd, ( struct sockaddr* )&saddr, sizeof( saddr ) );
    if ( ret == -1 ) {
        perror( "connect" );
        return -1;
    } // if


    // 創建thread去持續地向server收資料
    // 這樣才能第一時間收到其他client的訊息
    pthread_t tid;
    pthread_create( &tid, NULL, working, &fd );
    pthread_detach( tid );

    
    // communication
    struct data msg;
    printf( "Enter your name:" );
    scanf( "%s", msg.name );
    while ( 1 ) {
        // 發資料
        scanf( "%s", msg.buff );
        // fgets( msg.buff, 1024, stdin );
        if ( strcmp( msg.buff, "quit" ) == 0 ) {
            break;
        } // if
            


        ret = send( fd, &msg,  sizeof( msg ), 0 );
        // if ( ret == -1 ) {
        //     perror( "send error" );
        //     close( fd );
        //     exit( 1 );
        // } // if
        sleep( 1 );
    } // while

    close( fd );
    return 0;
} // int