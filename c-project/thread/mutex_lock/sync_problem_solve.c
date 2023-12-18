#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX 50

int num;

// 創建互斥鎖
pthread_mutex_t mutex;

void *funcA( void *arg ) {
    for ( int i = 0; i < MAX; i++ ) {

        pthread_mutex_lock( &mutex );
        // 要保護的臨界區間的開始
        int cur = num;
        cur++;
        usleep( 10 );
        num = cur;
        printf( "Thread A, id = %lu, number = %d\n", pthread_self(), num );
        // 要保護的臨界區間的結束
        pthread_mutex_unlock( &mutex );

    } // for

    
    return NULL;
} // void

void *funcB( void *arg ) {

    for ( int i = 0; i < MAX; i++ ) {
        pthread_mutex_lock( &mutex );
        // 要保護的臨界區間的開始
        int cur = num;
        cur++;
        num = cur;
        printf( "Thread B, id = %lu, number = %d\n", pthread_self(), num );
        // 要保護的臨界區間的結束
        pthread_mutex_unlock( &mutex );

        usleep( 10 );
    } // for

    
    return NULL;
} // void

int main( int argc, char *argv ) {
    // 這個程式展示了不進行thread同步處理造成的後果
    // A和B thread會對全域變數num+1，但是稍微複雜一點，會先透過一個變數cur存取num的值
    // 接著將變數cur++後再將值assign給num，這樣做是為了更符合現實中的複雜情況，使同步問題更加明顯
    // 理想情況下，num的值會被+到100，但實際執行的結果每次都不同。
    pthread_t A, B;
    // 初始化
    // 地按個參數為屬性，用來指定不同thread間爭奪臨界區間的方式
    pthread_mutex_init( &mutex, NULL );

    pthread_create( &A, NULL, funcA, NULL );
    pthread_create( &B, NULL, funcB, NULL );


    pthread_join( A, NULL );
    pthread_join( B, NULL );

    // 銷毀
    pthread_mutex_destroy( &mutex );

    
    return 0;
} // int