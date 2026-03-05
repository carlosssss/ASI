#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FIFO_PATH "/tmp/fifo_monitor_1"
#define FIFO_PATH2 "/tmp/fifo_monitor_2"

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

        // 5. Enviar en eco el secreto 2 por el fifo 2 inmediatamente
        write(fd2, buffer, bytes_leidos);
        printf("-> Secreto 2 devuelto en eco por %s.\n", FIFO_PATH2);
    }

    close(fd1);
    close(fd2);

    return 0;
}
