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
        key_t key;
        message_buf  rbuf;

        key = 10;

        if ((msqid = msgget(key, 0666)) < 0) {
            perror("msgget");
            exit(1);
        }


        if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }

        printf("%s\n", rbuf.mtext);
        exit(0);
    }

    if(!strcmp(argv[1], "--posix"))
    {
        int fd1; 
        char * myfifo = "/tmp/myfifo"; 
        mkfifo(myfifo, 0666); 
        char str1[80], str2[80]; 
        while (1) 
        { 
        
            fd1 = open(myfifo,O_RDONLY); 
            read(fd1, str1, 80);     
            printf("User1: %s\n", str1); 
            close(fd1); 
            fd1 = open(myfifo,O_WRONLY); 
            fgets(str2, 80, stdin); 
            write(fd1, str2, strlen(str2)+1); 
            close(fd1);     
        } 
    }
  


    return 0; 
} 

