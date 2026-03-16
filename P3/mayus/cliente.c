#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    key_t key = 25203218L;
    int shmid = shmget(key, 512, 0666);
    char *mem = (char *) shmat(shmid, NULL, 0);
    
    char *pre = mem;
    char *res = mem + 256;

    printf("Escribir un mensaje: ");
    fgets(pre, 256, stdin);
    pre[strcspn(pre, "\n")] = 0;
    while (res[0] == '\0') {
        sleep(1);
    }
    printf("Servidor responde: %s\n", res);

    res[0] = '\0';
    return EXIT_SUCCESS;
}
