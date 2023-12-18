#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main( int argc, char *argv[] ) {
    // raise只能將signal發送給自己，相當於kill( getpid(), sig )


    pid_t pid;
    pid = fork();

    // parent process
    if ( pid > 0 ) {
        sleep( 5 );
        printf( "%d\n", pid );
        
        // pid為child proces的pid，WNOHANG表示為non block模式
        if ( waitpid( pid, NULL, WNOHANG ) == 0 ) {
            // 將child process終止
            kill( pid, 9 );
        } // if

        // // 將waitpid設為block模式，就會導致parent process一直處於睡眠狀態
        // // 因為child process使用了raise( SIGTSTP )將process暫停
        // // 所以waitpid會一直等待child process的終止而導致處於睡眠狀態
        // if ( waitpid( pid, NULL, 0 ) == 0 ) {
        //     // 將child process終止
        //     kill( pid, 9 );
        // } // if

        // 回收child process的資源，就不會產生Z狀態的殭屍process
        wait( NULL );

        while( 1 );
    } // if
    // child process
    else if ( pid == 0 ) {
        printf( "raise function before\n" );
        // 這個signal相當於按下了ctrl Z，能暫停process
        raise( SIGTSTP );
        printf( "raise function after\n" );
        exit( 0 );
    } // else
    
    return 0;
} // int