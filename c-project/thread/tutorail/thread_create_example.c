#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
void *func( void *arg ) {
    for ( int i = 0; i < 5; i++ ) {
        printf( "sub thread i = %d\n", i );
    } // for

    printf( "sub thread i = %ld\n", pthread_self() );

    return NULL;
} // void
int main( int argc, char *argv ) {

    pthread_t pid;
    pthread_create( &pid, NULL, func, NULL );

    // // 代表著主thread會等待副thread執行完畢才將資源釋放
    // // 若沒有這行，副thread大機率執行不完或是根本沒有執行
    // // 因為主thread會佔有著cpu，當主thread執行完畢後，time slice很有可能會剩不多，所以副thread會執行不完或沒有執行
    // // 也能延長主thread的執行時間，例如sleep()會將主thread從CPU的執行狀態改變為等待狀態，所以就能讓CPU給付thread使用
    // // 但是當副thread的執行時間大於sleep的時間，副thread的程式一樣執行不完，所以是個治標不治本的方法
    // // 並且函數的第二個參數為一個傳出參數，是一個間接指標void **p，必須在副thread中呼叫pthread_exit( void *arg )，將要傳出的數據當作參數或是直接使用return來回傳
    // // 如此一來，主thread在呼叫pthread_join回收資源時，就能夠收到副thread想傳出的參數
    // pthread_join( pid, NULL );

    for ( int i = 0; i < 5; i++ ) {
        printf( "main thread i = %d\n", i );
    } // for

    printf( "main thread i = %ld\n", pthread_self() );

    // 若在主thread加上這個函數，主thread會在所有thread都執行完畢時才退出
    // 所以也能夠利用這個函數來保證副thread的執行
    // 但這行後面的程式碼將不會被執行
    pthread_exit( NULL );
    return 0;
} // int