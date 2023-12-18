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
    int fd[2];
    pid_t pid;
    char buff[1024];

    // 回傳的值不是fd，fd會直接傳到參數"fd"中
    int ret = pipe( fd );
    if ( ret == -1 ) {
        perror( "pipe error" );
        exit( 1 );
    } // if

    pid = fork();
    if ( pid == -1 ) {
        perror( "fork error" );
        exit( 1 );
    } // if



    // fork出的process
    if ( pid == 0 ) {
        read( fd[0], buff, 128 );
        printf( "child process:%s\n", buff );
    } // else if
    // 原本的process
    else {
        printf( "enter message:" );
        scanf( "%s", buff );


        printf( "parent process:%s\n", buff );
        sleep( 2 );
        
        write( fd[1], buff, sizeof( buff ) );
    } // else

    // 在linux kernel的pipe中的資料，讀完了就會清除
    // 若pipe中沒有資料，則會read會被block住
    



    close( fd[0] );
    close( fd[1] );
    return 0;
} // int