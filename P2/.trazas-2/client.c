#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define FIFO_PATH "/tmp/fifo_monitor_1"
#define FIFO_PATH2 "/tmp/fifo_monitor_2"

struct msgbuf {
    long mtype;
    char mtext[100];
};

int main() {
    int fd1, fd2;
    char buffer[16]; 
    char secreto_str[16];
    int secreto1;
    ssize_t bytes_leidos;

    fd1 = open(FIFO_PATH, O_RDONLY);
    bytes_leidos = read(fd1, buffer, sizeof(buffer) - 1);
    buffer[bytes_leidos] = '\0';     
    sscanf(buffer, "<%d>", &secreto1);
    printf("Secreto capturado correctamente: %d\n", secreto1);
    
    // EJERCICIO 2
    printf("Ejercicio 1 hecho, en pausa hasta que el usuario presione ENTER");
    getchar();

    mkfifo(FIFO_PATH2, 0666);

    printf("Esperando a que el monitor abra %s \n", FIFO_PATH2);
    fd2 = open(FIFO_PATH2, O_WRONLY);

    sprintf(secreto_str, "<%d>", secreto1);
    write(fd2, secreto_str, strlen(secreto_str));
    printf("-> Secreto 1 enviado al monitor: %s\n", secreto_str);

    printf("Esperando el Secreto 2 por %s...\n", FIFO_PATH);
    fd1 = open(FIFO_PATH, O_RDONLY);
    bytes_leidos = read(fd1, buffer, sizeof(buffer) - 1);
    
    if (bytes_leidos > 0) {
        buffer[bytes_leidos] = '\0';
        printf("<- Secreto 2 recibido: %s\n", buffer);

        write(fd2, buffer, bytes_leidos);
        printf("-> Secreto 2 devuelto en eco por %s.\n", FIFO_PATH2);
    }

    printf("\n[PAUSA] pulsa ENTER\n");
    getchar();

    key_t clave_cola = 0x25203218L;
    int msgid;
    struct msgbuf mensaje;
    long secreto3;
    int secreto4;

    msgid = msgget(clave_cola, 0666);

    secreto3 = mensaje.mtype;

    sscanf(mensaje.mtext, "<%d>", &secreto4);

    printf("Secreto 3 capturado correctamente: %ld\n", secreto3);
    printf("Secreto 4 capturado correctamente: %d\n", secreto4);

    printf("\n[PAUSA] Pulsa ENTER.\n");
    getchar();


    msgid = msgget(clave_cola, IPC_CREAT | 0666);

    struct msgbuf mensaje_envio;
    mensaje_envio.mtype = 1;     
    sprintf(mensaje_envio.mtext, "<%ld><%d>", secreto3, secreto4);

    printf("\nMensaje enviado.\n");
    
    close(fd1);
    close(fd2);

    return 0;
}
