#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <signal.h>

void myfun( int signum ) {
    return;
} // void

int main( int argc, char *argv[] ) {
    // share memory 會一直存在於kernel當中
    // 直到被刪除或是系統關閉
    int shmid;
    int key;
    char *p;
    
    // // 透過ftok函數來創建一個key
    // key = ftok( "./a.c", 'a' );
    // if ( key == -1 ) {
    //     perror( "create key failure!\n" );
    //     return -1;
    // } // if

    int pid;
    shmid = shmget( IPC_PRIVATE, 128, IPC_CREAT | 0777 );
    if ( shmid == -1 ) {
        perror( "create share memory failure!\n" );
        return -1;
    } // 
    printf( "create share memory sucess shmid = %d\n", shmid );

    // fork要在shmget之後呼叫
    // 這樣父子process才會是在同一個shm進行溝通
    pid = fork();
    // parent process
    if ( pid > 0 ) {

        // 設定收到的信號處理方式，信號為parent process發的SIGUSR2
        // 因為pause收到signal就會被喚醒，所以處理方式直接結束即可
        signal( SIGUSR2, myfun );

        // shmat函數會產生一個位於user space的記憶體地址
        // 提供process進行資料的共享
        // 位於kernel space的shm不進行資料的讀寫
        p = ( char * )shmat( shmid, NULL, 0 );
        if ( p == NULL ) {
            perror( "parenet process : shmat function error\n" );
            return -1;
        } // if

        while ( 1 ) {
            // write share memory
            printf( "parent process writing:\n" );
            fgets( p, 128, stdin );

            // 發送signal給child process說parent process已經寫完資料
            kill( pid, SIGUSR1 );

            // 等待child process讀完資料
            // 收到signal後會被喚醒
            pause();
        } // while
    } // if
    // child parent
    else if ( pid == 0 ) {
        // 設定收到的信號處理方式，信號為parent process發的SIGUSR1
        // 因為pause收到signal就會被喚醒，所以處理方式直接結束即可
        signal( SIGUSR1, myfun );
        // shmat函數會產生一個位於user space的記憶體地址
        // 提供process進行資料的共享
        // 位於kernel space的shm不進行資料的讀寫
        p = ( char * )shmat( shmid, NULL, 0 );
        if ( p == NULL ) {
            perror( "parenet process : shmat function error\n" );
            return -1;
        } // if

        // read share memory
        while ( 1 ) {


            // 等待parent process寫完資料
            // 收到signal後會被喚醒
            pause();

            printf( "share memory data:%s", p );
            
            // 發送signal給parent process說child process已經讀完資料
            kill( getppid(), SIGUSR2 );
        } // while
    } // else


    




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