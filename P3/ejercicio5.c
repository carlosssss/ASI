#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>


int main(int argc, char *argv[]){
    key_t clave = 0x25203218;
    size_t tam = 1024;
    void *shm_ptr;
    
    int idsem = semget(clave, 2, 0666);
    int idshm = shmget(clave, tam, 0666); 

    struct sembuf entrada[2];

    // Operación para el semáforo -1
    entrada[0].sem_num = 0;
    entrada[0].sem_op = -1;
    entrada[0].sem_flg = 0;

    // Operación para el semáforo 1
    entrada[1].sem_num = 1;
    entrada[1].sem_op = -1;
    entrada[1].sem_flg = 0;

    // Llamamos a semop pasándole el array de 2 operaciones
    semop(idsem, entrada, 2); 

    shm_ptr = shmat(idshm, NULL, 0);
    int *p_enteros = (int *)shm_ptr;
    int secreto1 = p_enteros[0];
    printf("Secreto <1> (posición 0): <%d>\n", secreto1);

    secreto1 = secreto1 * -1;
    p_enteros[0] = secreto1;
    sleep(3);

    struct sembuf salida[2];

    salida[0].sem_num = 0; salida[0].sem_op = 1; salida[0].sem_flg = 0;
    salida[1].sem_num = 1; salida[1].sem_op = 1; salida[1].sem_flg = 0;

    semop(idsem, salida, 2);

    shmdt(shm_ptr);

    return EXIT_SUCCESS;
}
