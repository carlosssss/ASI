#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in serv_addr;

    printf("Ejercicio1\n");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(3000); 
    
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    printf("Arranca el monitor y pulsa ENTER\n");
    getchar();

    printf("Intentando conectar al puerto 3000\n");
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("Conectado\n");
    
    char buf[20];

    int bytes = recv(sockfd, buf, 5, 0);
    printf("Secreto 2: %s\n", buf);

    printf("Pulsa Enter para enviar\n");
    getchar();
    printf("Enviando...\n");

    send(sockfd, buf, bytes, 0);
    printf("Enviado\n");
    
    bytes = recv(sockfd, buf, sizeof(buf) - 1, 0);
    send(sockfd, buf, bytes, 0);
    close(sockfd);
    return EXIT_SUCCESS;
}
