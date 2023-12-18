#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <signal.h>
#include <string.h>

struct msgbuff {
    long type;
    char voltage[124];
    char ID[4];
};

int main( int argc, char *argv[] ) {
    int msgid;
    int readret;
    struct msgbuff sendbuff, recvbuff;

    // 要實現不同process間的溝通，要使用ftok函數來創建key
    int key = ftok( "./a.c", 'a' );
    if ( key == -1 ) {
        perror( "create key failure!\n" );
        return -1;
    } // if


    // 創建message queue
    // 第一個參數為key
    // 第二個參數為權限
    msgid = msgget( key, IPC_CREAT | 0777 );
    if ( msgid == -1 ) {
        perror( "create message queue failure!\n" );
        return -1;
    } // if
    printf( "create message queue success msgid = %d\n", msgid );

    // 查看msg queue
    system( "ipcs -q" );



    while ( 1 ) {
        // 清空
        memset( recvbuff.voltage, 0, 124 );

        

        // 從message queue讀取數據
        // 第一個參數為要寫的message queue id
        // 第二個參數是數據會讀到哪個資料結構，要強制轉換類型成void*
        // 第三個參數是要讀取的實際內容大小，不是結構體的大小
        // 第四個參數是要讀哪一種類型的數據
        // 第五個參數是以block還是non block方式讀取，0為block，IPC_NOWAIT為non block
        // 成功的話會回傳實際讀取到的大小(byte)
        readret = msgrcv( msgid, ( void * )&recvbuff, 124, 100, 0 );
        if ( readret == -1 ) {
            perror( "message receive failure!\n" );
            return -1;
        } // if
        
        printf( "receive data form message queue:%s", recvbuff.voltage );
        
    } // while




    // 刪除message queue
    msgctl( msgid, IPC_RMID, NULL );
    // 查看msg queue
    system( "ipcs -q" );
    return 0;
} // int