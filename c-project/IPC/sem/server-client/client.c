#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
}; 

int semid;
// 聯合體有4個成員，要進行初始化的只有第一個成員val，表示semphore的初值
union semun mysemun;

// 結構體有3個成員分別是sem_num代表要操作哪一個semphore、sem_op代表要執行哪種操作，以及sem_flg代表要以哪種模式執行block或是non block
struct sembuf mysembuff;

int main( int argc, char *argv[] ) {
    int i;
    int key;
    key = ftok( "./a.c", 'a' );
    if ( key == -1 ) {
        perror( "create key failure!\n" );
    } // if

    printf( "create key success!\n" );


    // 創建或打開一個semphore集合
    // 第一個參數為key
    // 第二個參數是該集合內有多少個semphore
    // 第三個參數為權限
    semid = semget( key, 3, IPC_CREAT | 0777 );
    if ( semid == -1 ) {
        perror( "create semphore failure!\n" );
        return -1;
    } // if
    printf( "create semphore sucess semid = %d\n", semid );
    system( "ipcs -s" );

    // 初始化union
    // 因為我們是要先運行server，所以client端就不需要進行初始化
    // 也可以反過來，client端初始化server端不初始化
    // 所以如果先運行client，並不會print初10條訊息
    // 會等到server運行完並執行V操作後，client才會繼續運行下去
    // mysemun.val = 0; // 將semphore初值設為0
    // semctl( semid, 0, SETVAL, mysemun );

    // 初始化sem
    mysembuff.sem_num = 0; // 代表要操作集合中第1個semphore
    mysembuff.sem_flg = 0; // 代表是non block模式



    // 執行P操作
    // 因為目的是要讓Server先print 10條訊息
    // 所以client端要先進行P操作來判斷是否能進入臨界區間
    mysembuff.sem_op = -1;
    // 第一個參數為semphore id
    // 第二個參數為一個結構體，有3個成員
    // 分別是sem_num代表要操作哪一個semphore、sem_op代表要執行哪種操作，以及sem_flg代表要以哪種模式執行block或是non block
    // 第三個參數為要操作幾個semphore
    semop( semid, &mysembuff, 1 );
    for ( i = 0; i < 10; i++ ) {
        usleep( 100 );
        printf( "this is main fun i = %d\n", i );
    } // for

    
    


    // 第一個參數為semaphore id
    // 第二個參數為要操作semaphore集合中的哪一個semaphore
    // 第三個參數為命令，除了有IPC_RMID還有GETVAL和SETVAL等等多種不同的定義
    // 第四個參數沒有的話可加可不加
    semctl( semid, 0, IPC_RMID, NULL );
    system( "ipcs -s" );

    return 0;
} // int