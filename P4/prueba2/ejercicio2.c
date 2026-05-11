#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

int main(int argc, char *argv[]){
    int clientfd;
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(3001);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(socketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(socketfd, 5);

    signal(SIGCHLD, SIG_IGN);

    while (1){
        clientfd = accept(socketfd, NULL, NULL);
        pid_t pid = fork();
        if (pid == 0) {
            close(socketfd);
            int numero_recibido;
            char buf[50];
            while (recv(clientfd, &numero_recibido, sizeof(int), 0) > 0) {
                sprintf(buf, "<%d>", numero_recibido);
                send(clientfd, buf, strlen(buf), 0);
            }
            close(clientfd);
            exit(0);
        } else{
        close(clientfd);
        }
    }
    close(socketfd);


    return EXIT_SUCCESS;
}
