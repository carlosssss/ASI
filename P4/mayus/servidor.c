#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PUERTO 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, nuevo_socket;
    struct sockaddr_in direccion;
    int opt = 1;
    int addrlen = sizeof(direccion);
    char buffer[BUFFER_SIZE] = {0};
    char *mensaje_respuesta = "Mensaje recibido por el servidor.";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Fallo al crear el socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Fallo en setsockopt");
        exit(EXIT_FAILURE);
    }

    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(PUERTO);

    if (bind(server_fd, (struct sockaddr *)&direccion, sizeof(direccion)) < 0) {
        perror("Fallo en el bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Fallo en listen");
        exit(EXIT_FAILURE);
    }

    printf("Servidor escuchando en el puerto %d...\n", PUERTO);

    while(1) {
        if ((nuevo_socket = accept(server_fd, (struct sockaddr *)&direccion, (socklen_t*)&addrlen)) < 0) {
            perror("Fallo al aceptar conexion");
            continue;
        }

        printf("Cliente conectado.\n");

        int valread = read(nuevo_socket, buffer, BUFFER_SIZE);
        printf("Cliente dice: %s\n", buffer);

        send(nuevo_socket, mensaje_respuesta, strlen(mensaje_respuesta), 0);
        printf("Respuesta enviada.\n");

        close(nuevo_socket);
        memset(buffer, 0, BUFFER_SIZE);
        printf("Conexion cerrada. Esperando otro cliente...\n\n");
    }

    return 0;
}
