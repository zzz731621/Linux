#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main( int argc, char *argv[] ) {
    // 因為一個process要接收signal的前提是該process存在
    // 所以可以用三種方式來保持他存在

    // 第一種使用pause
    pause();

    // 第二種使用sleep
    sleep( 5 );

    // 第三種使用while迴圈進入循環
    while( 1 );

    return 0;
} // int