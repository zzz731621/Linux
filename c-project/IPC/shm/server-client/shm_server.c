#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <signal.h>

struct mybuff {
    int pid;
    char buff[128];
};

void myfun( int signum ) {
    return;
} // void

int main( int argc, char *argv[] ) {
    // share memory 會一直存在於kernel當中
    // 直到被刪除或是系統關閉
    int shmid;
    int key;
    int pid;
    struct mybuff *p;
    
    // 透過ftok函數來創建一個key
    key = ftok( "./a.c", 'a' );
    if ( key == -1 ) {
        perror( "create key failure!\n" );
        return -1;
    } // if
    printf( "create key success!\n" );

    shmid = shmget( key, 128, IPC_CREAT | 0777 );
    if ( shmid == -1 ) {
        perror( "create share memory failure!\n" );
        return -1;
    } // 
    printf( "create share memory sucess shmid = %d\n", shmid );


    // 設定收到的信號處理方式，信號為parent process發的SIGUSR2
    // 因為pause收到signal就會被喚醒，所以處理方式直接結束即可
    signal( SIGUSR2, myfun );

    // shmat函數會產生一個位於user space的記憶體地址
    // 提供process進行資料的共享
    // 位於kernel space的shm不進行資料的讀寫
    p = ( struct mybuff * )shmat( shmid, NULL, 0 );
    if ( p == NULL ) {
        perror( "parenet process : shmat function error\n" );
        return -1;
    } // if


    // 取得client端的pid
    // 但因為兩個process是獨立且沒有親緣關係的process
    // 所以server端啟動後，要先向shm寫入server端的pid
    // 之後client端啟動，讀取server端的pid
    // 接著client端寫入client端的pid，之後再根據server端的pid發送signal
    p->pid = getpid(); // 先寫入server端的pid

    // 等待client端讀取server pid且寫入client端的pid
    pause(); 

    // 等待收到client端發送的signal將pause喚醒後
    // 這時共享記憶體中的p->pid會是client端的pid
    pid = p->pid;



    while ( 1 ) {
        // write share memory
        printf( "parent process writing:\n" );
        fgets( p->buff, 128, stdin );


        // 等待client process讀完資料
        kill( pid, SIGUSR1 );
        // 收到signal後會被喚醒
        pause();
    } // while
    


    




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