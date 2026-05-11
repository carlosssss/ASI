#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(3001);
    serv_addr.sin_family = AF_INET;
    bind(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    struct sockaddr_in monitor_addr;
    socklen_t addr_len = sizeof(monitor_addr);
    int numero_recibido;
    char buf[50];
    recvfrom(socketfd, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&monitor_addr, &addr_len);
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "<%d>", numero_recibido);

    sendto(socketfd, buf, strlen(buf), 0, (struct sockaddr *)&monitor_addr, addr_len);
    return EXIT_SUCCESS;
}
