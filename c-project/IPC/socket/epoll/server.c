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

int main( int argc, char *argv[] ) {
    // 創建socket
    int lfd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( lfd == -1 ) {
        perror( "socket error" );
        return -1;
    } // if

    // 設定ip和port
    struct sockaddr_in serv_addr;
    memset( &serv_addr, 0, sizeof( struct sockaddr_in ) );
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( PORT );
    serv_addr.sin_addr.s_addr = htonl( INADDR_ANY );


    // 設定port能重複使用
    int optval = 1;
    setsockopt( lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof( optval ) );

    // 綁定
    int ret = bind( lfd, ( struct sockaddr* )&serv_addr, sizeof( serv_addr ) );
    if ( ret == -1 ) {
        perror( "bind error" );
        return -1;
    } // if

    ret = listen( lfd, 64 );
    if ( ret == -1 ) {
        perror( "listen error" );
        return -1;
    } // if


    // 創建epoll
    int epfd = epoll_create( 1 );
    if ( epfd == -1 ) {
        perror( "epoll_create error" );
        return -1;
    } // if

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = lfd;
    epoll_ctl( epfd, EPOLL_CTL_ADD, lfd, &ev );

    
    // 初始化
    for ( int i = 0; i < MAX_CLIENT; i++ ) {
        memset( &infos[i].addr, 0, sizeof( infos[i].addr ) );
        infos[i].fd = -1;
    } // for
    
    
    struct epoll_event evs[1024];
    int size = sizeof( evs ) / sizeof( evs[0] );

    struct sockaddr_in caddr;
    int addrlen = sizeof( caddr );


    while ( 1 ) {
        int num = epoll_wait( epfd, evs, size, -1 );
        // printf( "%d\n", num );

        for ( int i = 0; i < num; i++ ) { 
            int fd = evs[i].data.fd;

            // 代表這次的事件是client端發送連接請求
            if ( fd == lfd ) {
                
                struct SockInfo *pinfo;
                for ( int j = 0; j < MAX_CLIENT; j++ ) {
                    if ( infos[j].fd == -1 ) {
                        pinfo = &infos[j];
                        break;
                    } // if
                } // for

                
                int cfd = accept( fd, NULL, NULL );
                pinfo->fd = cfd;
                if ( cfd == -1 ) {
                    perror( "accept" );
                    break;
                } // if

                
                // struct epoll_event ev;
                ev.events = EPOLLIN;
                ev.data.fd = cfd;
                epoll_ctl( epfd, EPOLL_CTL_ADD, cfd, &ev );
            } // if
            else {
                struct data msg;
                memset( msg.buff, 0, sizeof( msg.buff ) );
                memset( msg.name, 0, sizeof( msg.name ) );
                int len = recv( fd, &msg, sizeof( msg ), 0 );

                // 錯誤處理
                if ( len == -1 ) {
                    perror( "recv error" );
                    exit( 1 );
                } // if
                // client斷開連接
                else if ( len == 0 ) {
                    printf( "%s offline...\n", msg.name );
                    for ( int j = 0; j < MAX_CLIENT; j++ ) {
                        if ( infos[j].fd == fd ) {
                            infos[j].fd = -1;
                            break;
                        } // if
                    } // for

                    epoll_ctl( epfd, EPOLL_CTL_DEL, fd, NULL );
                    close( fd ); 
                    break;
                } // else if

                // 發送訊息給所有在線的client
                for ( int j = 0; j < MAX_CLIENT; j++ ) {
                    if ( infos[j].fd == -1 )
                        continue;

                    ret = send( infos[j].fd, &msg, sizeof( msg ), 0 );
                    if ( ret == -1 ) {
                        perror( "send error" );
                        exit( 1 );
                    } // if
                } // for
                

            } // else
        } // for
    } // while


    close( lfd );

} // int