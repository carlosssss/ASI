#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

struct mensaje {
    long mtype;
    char mtext[100];
};

int main(int argc, char *argv[]){
    int msqid;
    struct mensaje msg;
    key_t clave = 0x25203218L;

    msqid = msgget(clave, 0666);
    
    msgrcv(msqid, &msg, sizeof(msg.mtext), getpid(), 0);

    printf("Aquí lo tienes: %s\n\n", msg.mtext);

    long pid_monitor;
    printf("\nPID Monitor: ");
    scanf("%ld", &pid_monitor);
    msg.mtype = pid_monitor;
    msgsnd(msqid, &msg, sizeof(msg.mtext), 0);

    printf("\n--- EJERCICIO 7: A LIMPIAR LA MIERDA ---\n");
    // El PID del monitor ya lo pediste en el paso 6, así que lo reusamos.
    // Si te pide reiniciar el monitor para el 7, vas a tener que volver a pedir el PID con scanf.
    
    printf("Dale al Enter cuando hayas arrancado la opción 7 en el monitor...\n");
    getchar(); // Pausita para sincronizar
    getchar(); // Por si hay basura en el buffer del scanf anterior

    // 1. Avisamos al monitor
    msg.mtype = pid_monitor; 
    sprintf(msg.mtext, "limpiando"); // El texto da igual
    
    printf("Avisando al monitor de que saco la basura...\n");
    if (msgsnd(msqid, &msg, sizeof(msg.mtext), 0) == -1) {
        perror("Fallo enviando el aviso final");
    }

    // 2. Destruimos la cola de mensajes usando msgctl y el flag IPC_RMID
    printf("Destruyendo la cola IPC...\n");
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("Fallo al borrar la cola (o ya estaba borrada)");
    }

    // 3. Destruimos los archivos FIFO usando unlink
    printf("Destruyendo los FIFOs...\n");
    unlink("/tmp/fifo_monitor_1");
    unlink("/tmp/fifo_monitor_2");

    printf("\nTodo limpio. Mira el monitor a ver si te tira el Secreto 8 de una puta vez.\n");

    return EXIT_SUCCESS;
}
