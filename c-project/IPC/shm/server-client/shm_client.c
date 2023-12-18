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
    int pid;
    int key;
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


    // 設定收到的信號處理方式，信號為server process發的SIGUSR1
    // 因為pause收到signal就會被喚醒，所以處理方式直接結束即可
    signal( SIGUSR1, myfun );

    // shmat函數會產生一個位於user space的記憶體地址
    // 提供process進行資料的共享
    // 位於kernel space的shm不進行資料的讀寫
    p = ( struct mybuff * )shmat( shmid, NULL, 0 );
    if ( p == NULL ) {
        perror( "parenet process : shmat function error\n" );
        return -1;
    } // if


    // 因為server端會先啟動加上先寫入server pid
    // 所以這時共享記憶體中的p->pid是server pid
    pid = p->pid;

    // 接著將client pid寫入共享記憶體供server端讀取
    p->pid = getpid();

    // 通知server端已經讀取並寫入完畢
    kill( pid, SIGUSR2 );

    while ( 1 ) {
        // // 等待server process寫完資料
        pause();

        // 從上面喚醒後表示收到來自server端的signal
        // 代表server端已經寫完資料了
        printf( "client process receive data:%s", p->buff );

        // 向server端發送signal表示client端已經讀取完畢
        kill( pid, SIGUSR2 );
        
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