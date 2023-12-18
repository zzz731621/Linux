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

    // 若在副thread中使用區域變數，因為區域變數是存放在記憶體中的stack
    // 而每個prcoess只有一個stack，所以屬於該process的所有thread會分stack空間
    // 該thread執行結束後，會將分到的stack歸還，所以原本該地址的區域變數大機率會得到不是預期的值
    // 可以使用malloc來宣告變數可以解決，因為malloc是存放在heap區域的，或是乾脆使用全域變數或是靜態變數(static)
    // 也可以在創建thread的時候，將地址當作參數傳進來
    // struct test t;
    static struct test t;
    // struct test *t = malloc( sizeof( struct test ) );
    t.num = 100;
    t.age = 15;

    pthread_exit( &t );

    // return NULL;
} // void
int main( int argc, char *argv ) {

    pthread_t tid;
    pthread_create( &tid, NULL, func, NULL );
    void *t;

    // 主thread會楚瑜block並等待副thread執行完畢才將資源釋放
    // 第二個參數為傳出參數，型別是void**，間接指標
    pthread_join( tid, &t );

    struct test *a = ( struct test * )t;
    
    printf( "num = %d, age = %d\n", a->num, a->age );
    // free( a );
    return 0;
} // int