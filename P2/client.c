#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_PATH "/tmp/fifo_monitor_1"

int main() {
    int fd;
    char buffer[16]; 
    int secreto;

    fd = open(FIFO_PATH, O_RDONLY);
    ssize_t bytes_leidos = read(fd, buffer, sizeof(buffer) - 1);
    buffer[bytes_leidos] = '\0';     
    sscanf(buffer, "<%d>", &secreto);
    printf("Secreto capturado correctamente: %d\n", secreto);

    close(fd);
    return 0;
}
