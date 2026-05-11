#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void ej123() {
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
}

void ej45() {
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
}

void ej6() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(3000); 
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    char saludo[] = "HOLA!";

    int enviados = sendto(sockfd, saludo, strlen(saludo), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    close(sockfd);
}

void ej7() {
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
}

int main(int argc, char *argv[]) {
    int opcion;

    do {
        printf("\n--- MENU DE EJERCICIOS ---\n");
        printf("1. Ejercicios 1, 2 y 3 (Cliente TCP)\n");
        printf("2. Ejercicios 4 y 5 (Servidor TCP)\n");
        printf("3. Ejercicio 6 (Cliente UDP)\n");
        printf("4. Ejercicio 7 (Servidor UDP)\n");
        printf("0. Salir\n");
        printf("Elige una opcion: ");
        scanf("%d", &opcion);
        
        while(getchar() != '\n');

        switch(opcion) {
            case 1:
                ej123();
                break;
            case 2:
                ej45();
                break;
            case 3:
                ej6();
                break;
            case 4:
                ej7();
                break;
            case 0:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion incorrecta.\n");
        }
    } while(opcion != 0);

    return EXIT_SUCCESS;
}
