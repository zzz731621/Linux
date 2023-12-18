#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <signal.h>

int main( int argc, char *argv[] ) {
    // share memory 會一直存在於kernel當中
    // 直到被刪除或是系統關閉
    int shmid;
    int key;
    char *p;
    // 透過ftok函數來創建一個key
    key = ftok( "./a.c", 'a' );
    if ( key == -1 ) {
        perror( "create key failure!\n" );
        return -1;
    } // if

    printf( "create key sucess key = %x\n", key );

    // 使用該key來創建shm的IPC對象
    shmid = shmget( key, 128, IPC_CREAT | 0777 );
    if ( shmid == -1 ) {
        perror( "create share memory failure!\n" );
        return -1;
    } // 
    

    printf( "create share memory sucess shmid = %d\n", shmid );


    // 查看位於kernel space共享記憶體
    // 實際上就是使用shmctl( shmid, IPC_STAT, NULL )來實現
    system( "ipcs -m" );
    
    
    // shmat函數會產生一個位於user space的記憶體地址
    // 提供process進行資料的共享
    // 位於kernel space的shm不進行資料的讀寫
    p = ( char * )shmat( shmid, NULL, 0 );
    if ( p == NULL ) {
        perror( "shmat function error\n" );
        return -1;
    } // if

    // write share memory
    fgets( p, 128, stdin );

    // read share memory
    printf( "share memory data:%s", p );



    // 刪除位於user space的記憶體，位於kernel中的還存在
    shmdt( p );

    // 刪除位於kernel
    // 第二個參數是要執行的命令，有IPC_STAT、IPC_SET和IPC_RMID
    // 等同於在CLI輸入"ipcrm -m shmid"
    shmctl( shmid, IPC_RMID, NULL );

    // 查看位於kernel space共享記憶體
    // 實際上就是使用shmctl( shmid, IPC_STAT, NULL )來實現
    system( "ipcs -m" );

    return 0;
} // int