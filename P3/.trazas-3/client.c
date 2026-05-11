#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    key_t clave = 0x25203218;
    size_t tam = 1024;
    void *shm_ptr;

    int mc1 = shmget(clave, tam, 0666); 
    shm_ptr = shmat(mc1, NULL, 0);

    int *p_enteros = (int *)shm_ptr;
    int secreto1 = p_enteros[0];
    printf("Secreto <1> (posición 0): <%d>\n", secreto1);
    int offset = p_enteros[1];
    char *secreto2 = (char *)(shm_ptr + offset);

    char secreto2_str[100];
    strcpy(secreto2_str, secreto2);

    printf("Secreto <2> (en el offset): %s\n", secreto2);
    sleep(3);
    shmdt(shm_ptr);
    
    printf("Hecho el ejercicio 1, en 10 segundos empieza el 2\n");
    sleep(10);

    //ejercicio2
    void *shm_ptr2;
    int mc2 = shmget(clave, tam, IPC_CREAT | 0666);
    if (mc2 == -1) {
        perror("error en shmget");
        exit(1);
    }
    shm_ptr2 = shmat(mc2, NULL, 0);
    if (shm_ptr2 == (void *)-1) {
        perror("error en shmat");
        exit(1);
    }

    sprintf((char *)shm_ptr2, "%s", secreto2_str);
    printf("Escrito secreto 2: %s\n", (char *)shm_ptr2);

    int valor_secreto2;
    sscanf(secreto2_str, "<%d>", &valor_secreto2); 
    int dsp = valor_secreto2 + 16;

    sprintf((char *)shm_ptr2 + dsp, "<%d>", secreto1);
    printf("Escrito secreto 1 <%d> en offset %d\n", secreto1, dsp);

    sleep(2);
    shmdt(shm_ptr2);

    //Ejercicio3

    return EXIT_SUCCESS;
}
