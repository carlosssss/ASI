#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>

union semun{
        int val;
        struct semid_ds *buf;
        unsigned short *array;
};

int main(int argc, char *argv[]){
    key_t clave = 0x25203218;
    size_t tam = 1024;

    union semun arg;

    int idsem = semget(clave, 2, IPC_CREAT | 0666);

    arg.val = 1;
    semctl(idsem, 0, SETVAL, arg);

    arg.val = 2;
    semctl(idsem, 1, SETVAL, arg);

    return EXIT_SUCCESS;
}
