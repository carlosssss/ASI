#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

int pidmonitor;
int ejercicio;

int contador_sigusr2 = 0;
int contando = 0; 

void handlerH2_inicio_fin(int sig) {
    if (contando == 0) {
        contando = 1;
        contador_sigusr2 = 0;
    } else {
        contando = 0;
    }
}

void handlerH2_contar_y_responder(int sig) {
    if (contando == 1) {
        contador_sigusr2++;
    }
    kill(pidmonitor, SIGUSR2);
}

void handlerH2(int sig) {
    kill(pidmonitor, SIGUSR2);
}

void* funcion_hilo(void* arg){
    while(1){
        sleep(5);
    }
    return NULL;
}

void handlerH1(int sig){
    pthread_t hilo_id;
    pthread_create(&hilo_id, NULL, funcion_hilo, NULL);
    pthread_detach(hilo_id);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <PID_MONITOR> <NUM_EJERCICIO>\n", argv[0]);
        return 1;
    }
    
    pidmonitor = atoi(argv[1]);
    ejercicio = atoi(argv[2]);

    signal(SIGUSR1, SIG_IGN); 

    if (ejercicio == 2) {
        kill(pidmonitor, SIGUSR1); 
        usleep(200000);
    } else if (ejercicio == 3) {
        kill(pidmonitor, SIGUSR1);
        sleep(1); 
    }

    pid_t pidh1 = fork();
    if (pidh1 == 0) {
        if (ejercicio == 4) signal(SIGUSR2, handlerH1);
        while (1) pause();
        exit(0);
    }

    pid_t pidh2 = fork();
    if (pidh2 == 0) {
        if (ejercicio == 2) {
            signal(SIGUSR2, handlerH2);
            kill(pidmonitor, SIGUSR2); 
        } 
        else if (ejercicio == 5) {
            signal(SIGUSR1, handlerH2_inicio_fin);
            signal(SIGUSR2, handlerH2_contar_y_responder);
        }
        while(1) pause();
        exit(0);
    }

    usleep(500000); 
    
    if (ejercicio == 3) {
        kill(pidmonitor, SIGUSR1);
    } else if (ejercicio >= 4 && ejercicio <= 6) {
        kill(pidmonitor, SIGUSR1); 
    }

    if (ejercicio == 6) {
        sleep(1);
        kill(pidh1, SIGKILL);
        kill(pidh2, SIGKILL);
        
        waitpid(pidh1, NULL, 0);
        waitpid(pidh2, NULL, 0);
        return 0; 
    }

    wait(NULL);
    wait(NULL);
    
    return 0;
}