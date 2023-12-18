#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main( int argc, char *argv[] ) {
    // raise只能將signal發送給自己，相當於kill( getpid(), sig )


    // 這時的raise( 9 )相當於執行_exit()，而不是exit()
    // 因為第一行的printf沒有\n，所以資料會先被寫入到buffer當中，不會傳給linux kernel
    // 然而執行_exit會直接關閉process，並不會執行終止處理程序，所以第一行不會傳給linux kernel
    // 若執行exit()，會執行相關的終止處理程序，所以會將printf的buffer內的資料傳給linux kernel
    printf( "raise before" );
    raise( 9 );
    printf( "raise after\n" );

    
    return 0;
} // int