#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>



int main(int argc, char *argv[]){
    key_t key = 25203218L;
    int shmid = shmget(key, 512, IPC_CREAT | 0666);

    char *mem = (char *) shmat(shmid, NULL, 0);

    char *pre = mem;
    char *res = mem + 256;
    pre[0] = '\0';
    res[0] = '\0';
    while(1){
        if(pre[0]!=0){
        int i = 0;
            while (pre[i] != '\0' && i < 255) {
                res[i] = toupper(pre[i]);
                i++;
            }
            res[i] = '\0';            
            pre[0] = '\0';
        }
    }
    return EXIT_SUCCESS;
}
