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

int pidmonitor;

void handlerH2() {
    printf("señal recibida");
    kill(pidmonitor, SIGUSR2);
}

void* funcion_hilo(){
    while(1){
        printf("Hilo\n");
        sleep(5);
    }
}

void handlerH1(){
    pthread_t hilo_id;
    pthread_create(&hilo_id, NULL, funcion_hilo, NULL);
    pthread_detach(hilo_id);

}


int main(int argc, char *argv[]) {
    pidmonitor = atoi(argv[1]);
    kill(pidmonitor, SIGUSR1);
    sleep(2);

    int pid;
    pid = getpid();
    printf("PID client: %i\n", pid);

    pid_t pidh1, pidh2;
    pidh1 = fork();
    if (pidh1 == 0) {
        signal(SIGUSR2, handlerH1);
        while (1) pause();
    }
    pidh2 = fork();
    if (pidh2 == 0) {
        signal(SIGUSR2, handlerH2);
        printf("PID Hijo2: %d\n", getpid());
        kill(pidmonitor, SIGUSR1);
        sleep(1);
        kill(pidmonitor, SIGUSR2);
        sleep(10);
    }
    wait(NULL);
    wait(NULL);
}

