
#include <stdio.h> 
#include<stdlib.h>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSGSZ     128
// очень много заимствования с http://www.linuxcenter.ru/lib/articles/programming/ipc_msg.phtml?style=print и https://www.geeksforgeeks.org/named-pipe-fifo-example-c-program/
typedef struct msgbuf {
         long    mtype;
         char    mtext[MSGSZ];
         } message_buf;
  
int main(int argc, char **argv) 
{ 
    if(argc <= 1) 
    {
         printf("plz use  \n\t%s --posix\n\tor\n\t%s --systemV \n",argv[0],argv[0]);
         return 0;
    }
    if(!strcmp(argv[1], "--systemV"))
    { 
        int msqid;
        int msgflg = IPC_CREAT | 0666;
        key_t key;
        message_buf sbuf;
        size_t buf_length;

        key = 10;

        printf("Calling msgget with key %#lx and flag %#o\n",key,msgflg);

        if ((msqid = msgget(key, msgflg )) < 0) {
            perror("msgget");
            exit(1);
        }
        else 
         printf("msgget: msgget succeeded: msqid = %d\n", msqid);


        sbuf.mtype = 1;

        printf("msgget: msgget succeeded: msqid = %d\n", msqid);

        (void) strcpy(sbuf.mtext, "I am in the queue?");

        printf("msgget: msgget succeeded: msqid = %d\n", msqid);

        buf_length = strlen(sbuf.mtext) + 1 ;

        if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
           printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
            perror("msgsnd");
            exit(1);
    }

    } 
  
    if(!strcmp(argv[1], "--posix"))
     {

         printf("standard selected POSIX\n");
        int fd;  
        char * myfifo = "/tmp/myfifo"; 
        mkfifo(myfifo, 0666); 
        char arr1[80], arr2[80]; 
        while (1) 
        { 
            fd = open(myfifo, O_WRONLY); 
            fgets(arr2, 80, stdin);   
            write(fd, arr2, strlen(arr2)+1); 
            close(fd); 
            fd = open(myfifo, O_RDONLY); /// открываем очередь 
            read(fd, arr1, sizeof(arr1)); //читаем из fifo 
            printf("User2: %s\n", arr1); 
            close(fd); 
        } 
    }
    return 0; 
}

