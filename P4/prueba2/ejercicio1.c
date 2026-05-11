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
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(3000);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    int secreto;
    int bytes = recv(socketfd, &secreto, sizeof(int), 0);
    char buf[30];
    sprintf(buf, "<%d>", secreto);
    send(socketfd, buf, strlen(buf), 0);

    close(socketfd);


    return EXIT_SUCCESS;
}
