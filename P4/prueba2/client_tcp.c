#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    
    char dni[10];
    char direccion[30];
    int puerto;

    if (argc != 2) {
        printf("Uso: %s <DNI@dominio:puerto>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (sscanf(argv[1], "%[^@]@%[^:]:%d", dni, direccion, &puerto) != 3) {
        printf("Error: El formato debe ser DNI@dominio:puerto\n");
        return EXIT_FAILURE;
    }

    printf("DNI: %s\n", dni);
    printf("Direccion: %s\n", direccion);
    printf("Puerto: %d\n", puerto);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_port = htons(puerto);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(direccion);
    
    connect(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    char buf[30] = "user DNI";
    send(socketfd, buf, 8,0);


    return EXIT_SUCCESS;
}
