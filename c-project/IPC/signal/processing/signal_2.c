#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
void myfun( int signum ) {
    int i = 0;
    while ( i < 10 ) {
        printf( "process signal signum = %d\n", signum );
        sleep( 1 );
        i++;
    } // while

    return;
} // void
int main( int argc, char *argv[] ) {
    // void ( *signal( int signum, void( *handler )( int ) ) )( int )
    // signal函數有兩個參數，第一個是signal值，第二個是另一個函數指標handler
    // handler有兩種值，第一種是SIG_IGN，表示忽略該signal
    // 第二個是SIG_DFL，表示使用系統default的方式
    // 第三個是自訂的處理方式
    

    // 這次是使用忽略的方式
    signal( 14, SIG_IGN );
    // 也可以使用default的方式，根據不同的signal，default的方式都不同，大部分是終止process
    signal( 14, SIG_DFL );



    alarm( 5 );
    int i = 1;
    while ( i < 10 ) {
        printf( "process process things = %d\n", i );
        i++;
        sleep( 1 );
    } // while

    return 0;
} // int