#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 9999
#define MAX_CLIENT 512

struct SockInfo {
    struct sockaddr_in addr;
    int fd;
};
struct data {
    char buff[1024];
    char name[1024];
};
struct SockInfo infos[MAX_CLIENT];


void *working( void *arg ) {

    struct SockInfo *pinfo = ( struct SockInfo* )arg;

    // 連接建立成功，印出client的IP和port
    char ip[32];
    printf( "client ip: %s, port: %d\n", inet_ntop( AF_INET, &pinfo->addr.sin_addr.s_addr, ip, sizeof( ip ) ), ntohs( pinfo->addr.sin_port ) );

    struct data msg;

    // communication
    while ( 1 ) {
        // 收資料
        // char buff[1024];

        // 接收目前thread負責的client所發出的訊息
        int len = recv( pinfo->fd, &msg, sizeof( msg ), 0 );

        if ( len > 0 ) {
            printf( "%s say: %s\n", msg.name, msg.buff );

            for ( int i = 0; i < 512; i++ ) {
                if ( infos->fd == -1 )
                    continue;
                
                send( infos[i].fd, &msg, sizeof( msg ), 0 );
            } // for
            
        } // if
        else if ( len == 0 ) {
            printf( "%s offline...\n", msg.name );
            break;
        } // else if
        else {
            perror( "recv" );
            break;
        } // else

    } // while

    close( pinfo->fd );
    pinfo->fd = -1;

    return NULL;
} // void

int main() {

    // 創建監聽用的socket
    int fd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( fd == -1 ) {
        perror( "error" );
        return -1;
    } // if

    // 綁定IP port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons( PORT );
    saddr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind( fd, ( struct sockaddr* )&saddr, sizeof( saddr ) );
    if ( ret == -1 ) {
        perror( "bind" );
        return -1;
    } // if


    // 監聽socket的指定port
    ret = listen( fd, 128 );
    if ( ret == -1 ) {
        perror( "listen" );
        return -1;
    } // if

    // 初始化infos
    int max = sizeof( infos ) / sizeof( infos[0] );
    for ( int i = 0; i < max; i++ ) {
        bzero( &infos[i], sizeof( infos[i] ) );
        infos[i].fd = -1;
    } // for

    // 阻塞並等待client連接
    struct sockaddr_in caddr;
    int addrlen = sizeof( caddr );
    while ( 1 ) {

        struct SockInfo *pinfo;
        for ( int i = 0; i < max; i++ ) {
            if ( infos[i].fd == -1 ) {
                pinfo = &infos[i];
                break;
            } // if
        } // for



        int cfd = accept( fd, ( struct sockaddr* )&pinfo->addr, &addrlen );
        *(pinfo)->fd = cfd;
        if ( cfd == -1 ) {
            perror( "accept" );
            break;
        } // if

        // 創建thread去處理client和server的溝通
        pthread_t tid;
        pthread_create( &tid, NULL, working, pinfo );
        pthread_detach( tid );
    } // while
    
    close( fd );
    return 0;
} // int