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
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(3000);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    
    socklen_t addr_len = sizeof(serv_addr);

    printf("Arranca el monitor\n");
    char peticion[] = "<???>";
    sendto(socketfd, peticion, strlen(peticion), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    char respuesta[10]; 
    int bytes = recvfrom(socketfd, respuesta, sizeof(respuesta), 0, (struct sockaddr*)&serv_addr, &addr_len);
    printf("%s", respuesta);

    int respuestaint;
    sscanf(respuesta, "<%d>", &respuestaint);

    sendto(socketfd, &respuestaint, sizeof(int), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
    close(socketfd);

    return EXIT_SUCCESS;
}
