#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main( int argc, char *argv[] ) {
    // alarm只能發送一種signal( SIGALRM )，和raise一樣只能發送給當前process，且linux kernel會定時一段時間才發送signal


    printf( "alarm before\n" );
    // 會在5秒之後發送signal，收到signal後的default處理方式為終止process
    alarm( 5 );
    printf( "alarm after\n" );
    
    // 大概會在第5次時終止，所以只會印出i = 1 ~ 4
    // 使用這個方式使process存在一段時間，因為process存在才能收到signal
    for ( int i = 0; i < 20; i++ ) {
        sleep( 1 );
        printf( "process things i = %d\n", i + 1 );
    } // for

    return 0;
} // int