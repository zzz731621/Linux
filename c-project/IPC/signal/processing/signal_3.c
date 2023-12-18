#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void myfun( int signum ) {
    int i = 0;
    while ( i < 5 ) {
        printf( "receive signum = %d, i = %d\n", signum, i++ );
        sleep( 1 );
    } // while
    return ;
} // void
void myfun1( int signum ) {
    printf( "receive signum = %d\n", signum );
    wait( NULL );
    return ;
} // void


int main( int argc, char *argv[] ) {
    // raise只能將signal發送給自己，相當於kill( getpid(), sig )

    pid_t pid;
    pid = fork();

    // parent process
    if ( pid > 0 ) {
        signal( 10, myfun );

        // 利用child process執行的exit( 1 )來得知child process已經執行完畢
        // 所以可以使用自訂方式來處理SIGCHLD signal( 17號 )
        // 來將child process的資源回收，避免產生殭屍process
        signal( 17, myfun1 );

        int i = 0;
        while ( 1 ) {
            printf( "parent process things, i = %d\n", i++ );
            sleep( 1 );
        } // while
    } // if
    // child process
    else if ( pid == 0 ) {
        sleep( 5 );
        kill( getppid(), 10 );
        sleep( 5 );

        // 相當於向parent process發送SIGCHLD signal
        // 所以可以在parent中使用自訂的signal處理方式 
        exit( 1 );
    } // else
    
    return 0;
} // int