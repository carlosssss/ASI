#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


#define fifo "/tmp/fifo_monitor_1"
#define fifo2 "/tmp/fifo_monitor_2"
#define DNI "0x25203218L"

int main(int argc, char *argv[]){

//1
char buffer[100];
char *secreto1 = "<654>";
char *secreto2 = "<763>";
int fd = open(fifo, O_RDONLY);
int bytes_leidos = read(fd, buffer, 100);
buffer[bytes_leidos] = '\0';
printf("Primer secreto: %s", buffer);

//2
printf("\nEnter para continuar\n");
getchar();

int fd2 = mkfifo(fifo2, 0666);
int fd_escritura = open(fifo2, O_WRONLY); // LO ABRES
write(fd_escritura, secreto1, strlen(secreto1));
//1
fd = open(fifo, O_RDONLY);
bytes_leidos = read(fd, buffer, 100);
buffer[bytes_leidos] = '\0';
write(fd_escritura, buffer, bytes_leidos);
printf("Segundo secreto: %s", buffer);

//3
printf("Secreto 2 conseguido, dale al enter para el tercero");
getchar();




return EXIT_SUCCESS;
}
