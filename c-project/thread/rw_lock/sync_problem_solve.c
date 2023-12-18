#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX 50

int num;

// 創建讀寫鎖
// 讀寫鎖有兩種模式，分別是對讀或寫進行鎖定
// 若是讀鎖，單純進行讀操作的所有thread都可以進入到臨界區間，可以平行執行( Parallel )
// 若是寫鎖，就會跟互斥鎖相同，一次只能允許一個thread進入到臨界區間
pthread_rwlock_t rwlock;

void *read_num( void *arg ) {
    for ( int i = 0; i < MAX; i++ ) {

        pthread_rwlock_rdlock( &rwlock );
        // 要保護的臨界區間的開始
        // 只有執行讀操作，所以用讀鎖就可以   
        printf( "read Thread id = %lu, number = %d\n", pthread_self(), num );
        // pthread_rwlock_unlock( &rwlock );

        
        usleep( rand() % 5 );
    } // for

    
    return NULL;
} // void




void *write_num( void *arg ) {
    for ( int i = 0; i < MAX; i++ ) {
        pthread_rwlock_wrlock( &rwlock );
        // 要保護的臨界區間的開始
        // 有涉及到讀和寫的操作，所以要用寫鎖   
        int cur = num;
        cur++;
        num = cur;
        printf( "Thread id = %lu, number = %d\n", pthread_self(), num );
        // 要保護的臨界區間的結束
        pthread_rwlock_unlock( &rwlock );

        usleep( 5 );
    } // for

    
    return NULL;
} // void

int main( int argc, char *argv ) {
    pthread_t p1[5], p2[3];

    // 初始化
    // 第二個參數為屬性，用來指定不同thread間爭奪臨界區間的方式
    pthread_rwlock_init( &rwlock, NULL );


    // 用於讀的thread
    for ( int i = 0; i < 5; i++ ) {
        pthread_create( &p1[i], NULL, read_num, NULL );
    } // for

    // 用於寫的thread
    for ( int i = 0; i < 3; i++ ) {
        pthread_create( &p2[i], NULL, write_num, NULL );
    } // for


    // 用於讀的thread
    for ( int i = 0; i < 5; i++ ) {
        pthread_join( p1[i], NULL );
    } // for

    // 用於寫的thread
    for ( int i = 0; i < 3; i++ ) {
        pthread_join( p2[i], NULL );
    } // for

    // 銷毀
    pthread_rwlock_destroy( &rwlock );

    
    return 0;
} // int