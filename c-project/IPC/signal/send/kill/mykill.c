#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main( int argc, char *argv[] ) {


    // 要有兩個參數，第一個是signal的值( 1 ~ 64 )
    // 第二個是process的id
    if ( argc < 3 ) {
        printf( "please input parameter\n" );
        return -1;
    } // if

    int sig, pid;

    sig = atoi( argv[1] );
    pid = atoi( argv[2] );

    printf( "sig = %d, pid = %d\n", sig, pid );

    // 透過system call : kill函數來實現一般在CLI中使用的kill用法，例如 kill -9 XXXX
    // 向另一個process發送signal，共有64種signal，可以輸入kill -l查看
    kill( pid, sig );
    return 0;
} // int