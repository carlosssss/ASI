#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);


    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(3001);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    struct sockaddr_in monitor_addr;
    socklen_t monitor_len = sizeof(monitor_addr);
    char buf[20];
    memset(buf, 0, sizeof(buf));
    
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    int bytes = recvfrom(sockfd, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&monitor_addr, &monitor_len);

    int enviados = sendto(sockfd, buf, bytes, 0, (struct sockaddr *)&monitor_addr, monitor_len);

    close(sockfd);
    return EXIT_SUCCESS;
}
