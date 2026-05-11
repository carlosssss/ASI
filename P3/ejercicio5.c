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
    
    int idsem = semget(clave, 1, 0666);
    int idshm = shmget(clave, tam, 0666); 

    struct sembuf operacion;
    operacion.sem_num = 0;  // El índice del semáforo (solo hay 1, así que es el 0)
    operacion.sem_op = -1;  // IMPORTANTE: -1 significa "quiero pillar el semáforo y bloquearlo"
    operacion.sem_flg = 0;  // Banderas a 0 y a correr

    // Y ejecutas la operación (1 es la cantidad de operaciones que le pasas, o sea, 1)
    semop(idsem, &operacion, 1);

    shm_ptr = shmat(idshm, NULL, 0);
    int *p_enteros = (int *)shm_ptr;
    int secreto1 = p_enteros[0];
    printf("Secreto <1> (posición 0): <%d>\n", secreto1);

    secreto1 = secreto1 * -1;
    p_enteros[0] = secreto1;
    sleep(3);



    operacion.sem_op = 1; // IMPORTANTE: 1 significa "suelto el semáforo, que pase el siguiente"
    semop(idsem, &operacion, 1);

    shmdt(shm_ptr);

    return EXIT_SUCCESS;
}
