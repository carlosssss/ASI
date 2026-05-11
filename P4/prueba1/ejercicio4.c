#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    int sockfd;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(3001);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("Abrimos el socket y escuchamos\n");
    listen(sockfd, 5);
    accept(sockfd, NULL, NULL);
    printf("Conexion al servidor\n");

    char buf[30];
    int bytes = recv(sockfd, buf, sizeof(buf) - 1, 0);
    send(sockfd, buf, bytes, 0);

    return EXIT_SUCCESS;
}
