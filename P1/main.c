#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>

void handler(int s);
void toupper_str(char *str);
void *funcion_traductor(void *arg);

int main(int argc, char *argv[]){
    pid_t   pidh1, pidh2;
    // Hijo 1
    pidh1 = fork();
    if(pidh1 == 0){
        printf("Hijo1\n");
        signal(SIGUSR1,handler);
        while(1){
            pause(); //Dormido hasta que llegue la senal    
            printf("Hijo1, Senal recibida\n");
            pthread_t hilo_id; //Creamos un hilo
            pthread_create(&hilo_id, NULL, funcion_traductor, NULL); //Creamos un nuevo hilo
            pthread_join(hilo_id, NULL); //El proceso se pare hasta que haya acabado el hilo
        }
    }

    // Hijo 2
    pidh2 = fork();
    if(pidh2 == 0){
        char entrada[1024];
        int fd;
        printf("Hijo2, escribe un mensaje: ");
            //Escribir en el fichero
        while(fgets(entrada, sizeof(entrada), stdin) != NULL) {
            fd = open("mensaje", O_CREAT | O_WRONLY | O_TRUNC, 0666); //TRUNC para reinciar el archivo, 0666 permisos, WRONLY solo escritura, CREAT para crear
            write(fd, entrada, strlen(entrada));

            close(fd);
            sleep(1);
            kill(pidh1, SIGUSR1);
        }
    }
    wait(NULL);
    wait(NULL);
    return EXIT_SUCCESS;
}

void handler(int s){
    int ultimo_estado = s;
    //printf("%d", ultimo_estado);
}

void toupper_str(char *str) {
    // Recorremos la cadena letra a letra
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void *funcion_traductor(void *arg) {
    char buffer[1024];
    int fd;
    ssize_t leidos;

    printf("Hijo1,Hilo\n");
    fd = open("mensaje", O_RDONLY); //O_RDONLY, read only
    leidos = read(fd, buffer, sizeof(buffer) - 1);
 
    buffer[leidos] = '\0'; 
    toupper_str(buffer);
    printf("Hijo1,Hilo RESULTADO: %s\n", buffer);

    //printf("Hijo1,Hilo Vuelva a escribir o Pulse CtrlC para salir >");
    close(fd);
    pthread_exit(NULL); //Acabamos el hilo
}
