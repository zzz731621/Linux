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
    // 創建message queue
    // 第一個參數為key，IPC_PRIVATE是0
    // 第二個參數為權限
    msgid = msgget( IPC_PRIVATE, 0777 );
    if ( msgid == -1 ) {
        perror( "create message queue failure!\n" );
        return -1;
    } // if
    printf( "create message queue success msgid = %d\n", msgid );

    // 查看msg queue
    system( "ipcs -q" );


    // 初始化
    sendbuff.type = 100;
    printf( "please enter message:\n" );
    fgets( sendbuff.voltage, 124, stdin );


    // 向message queue寫入數據
    // 第一個參數為要寫的message queue id
    // 第二個參數是要寫的結構體，要強制轉換類型成void*，結構體內容規定必須要有一個數據類型long type
    // 實際傳送的數據可以自訂
    // 第三個參數是要寫的實際內容大小，不是結構體的大小
    // 第四個參數是以block還是non block方式寫入，0為block，IPC_NOWAIT為non block
    msgsnd( msgid, ( void * )&sendbuff, strlen( sendbuff.voltage ), 0 );


    // 先初始化
    memset( recvbuff.voltage, 0, sizeof( recvbuff.voltage ) );

    // 從message queue讀取數據
    // 第一個參數為要寫的message queue id
    // 第二個參數是數據會讀到哪個資料結構，要強制轉換類型成void*
    // 第三個參數是要讀取的實際內容大小，不是結構體的大小
    // 第四個參數是要讀哪一種類型的數據
    // 第五個參數是以block還是non block方式讀取，0為block，IPC_NOWAIT為non block
    // 成功的話會回傳實際讀取到的大小(byte)
    readret = msgrcv( msgid, ( void * )&recvbuff, 124, 100, 0 );

    printf( "recv:%s", recvbuff.voltage );
    printf( "readret = %d\n", readret );


    // 讀取完之後會將該node的資料刪除
    // 所以若要再讀取第二次會導致block，直到message queue資料寫入
    readret = msgrcv( msgid, ( void * )&recvbuff, 124, 100, 0 );

    printf( "second recv:%s", recvbuff.voltage );
    printf( "second readret = %d\n", readret );



    // 刪除message queue
    // 第二個參數是命令，有三種IPC_STAT、IPC_SET、IPC_RMID，和shm非常類似
    msgctl( msgid, IPC_RMID, NULL );



    // 查看msg queue
    system( "ipcs -q" );
    return 0;
} // int