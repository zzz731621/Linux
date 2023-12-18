#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

struct test {
    int num;
    int age;
};

void *func( void *arg ) {
    for ( int i = 0; i < 5; i++ ) {
        printf( "sub thread i = %d\n", i );
    } // for

    printf( "sub thread i = %ld\n", pthread_self() );


    return NULL;
} // void
int main( int argc, char *argv ) {

    pthread_t tid;
    pthread_create( &tid, NULL, func, NULL );
    void *t;

    printf( "main thread i = %ld\n", pthread_self() );

    // 用來讓副thread自己回收系統資源
    // 因為如果使用join的話，回收時會處於block狀態
    // 在multi-thread下運行時，主thread會被block住導致無法進行其他作業
    // 例如socket通信的multi-thread方法就會透過detach讓副thread自行回收資源
    pthread_detach( tid );

    // 但是主thread若退出，副thread的資源也會一起回收
    // 所以還是要用exit來等待
    // 但在socket通信的multi-thread方法是透過while( 1 )來確保主thread的持續運行，所以不用加exit
    pthread_exit( NULL );

    
    return 0;
} // int