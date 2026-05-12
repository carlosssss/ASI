#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/signal.h>

#define SERVER_PORT 3001
#define DESTIP "127.0.0.1"

#define CLAVE 0x25203218L	// Clave de los recursos. Sustituir por DNI.
#define TIME 4			// Temporizador de retransmisión
#define BUFLEN 256	// Tamaño de bufferes genérico

#define SIZE_SHM 1024	//Tamaño del segmento de memoria compartida

#define MAX_DEV	4	//Maximo # dispositivos en la tabla de sesiones


/* El array de semáforos a crear tiene tamaño 4
	El semáforo 0 servirá para controlar el acceso a lecturas de valores de
		los semáforos 1 y 2
	El semáforo 1 para poder modificar su valor en comando SEM
	El semáforo 2 para poder modificar su valor en comando SEM
	el semáforo 3 para controlar los accesos a la tabla de sesiones en 
		fase de registro.
*/
#define MAX_SEM 4	// Número de semáforos en el array

/* Lista de tipos de mensaje recibidos en la cola de mensajes. */

#define COMM_SPEED '2'
#define COMM_RPM '3'
#define COMM_SEM '4'
#define COMM_BYE '5'

#define MAX_COMMAND 6

char *udp_cmd[]={
	"HELLO", "PORT","SPEED","RPM","SEM","BYE",""
};

#define OFF_DATA_TBL 0	// Desplazamiento de la tabla de sesiones en SHM

#define ST_FREE 0
#define ST_PID 1
#define ST_DATA 2
#define LEN_NAME 16	//Usar como maximo nombres de 8

struct st_data {
	int state;	// State of register
	char name[LEN_NAME];	// Name of device
	int speed;	// velocidad 
	int rpm;	// revoluciones por minuto
	int port;	// Original port 
	int sem;	// Sem number 
	int semval;	// Sem value 
	pid_t pid;	// Process identifier
};

/*************************************************************************/
/* Función a utilizar para sustituir a signal() de la libreria.
Esta función permite programar la recepción de la señál de temporización de
alarm() para que pueda interrumpir una funcion bloqueante.
El alumno debe saber como utilizarla.
*/
int signal_EINTR(int sig,void(*handler)())
{
struct sigaction sa;
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	return(sigaction(sig,&sa,NULL));
}
/*************************************************************************/
struct msg_q {
    long mtype;
    char mtext[100];
};

int main(int argc, char *argv[]){
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serv_addr;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(DESTIP);

    char buf[100];
    int pid = getpid();
    sprintf(buf, "HELLO %d Equipo", pid);
    printf("Enviamos %s\n", buf);
    sendto(socketfd, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    struct sockaddr_in mi_addr;
    socklen_t mi_addr_len = sizeof(mi_addr);

    getsockname(socketfd, (struct sockaddr*)&mi_addr, &mi_addr_len);
    
    int puerto = ntohs(mi_addr.sin_port);

    sprintf(buf, "PORT %d", puerto);
    printf("Enviamos %s\n", buf);
    sendto(socketfd, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    int msgid = msgget(CLAVE, IPC_CREAT | 0666);
    struct msg_q mensaje;
    while(1){
        memset(mensaje.mtext, 0, sizeof(mensaje.mtext));
        msgrcv(msgid, &mensaje, sizeof(mensaje.mtype), pid, 0);
        char tipo_comando = mensaje.mtext[0];
        if (tipo_comando == COMM_SPEED) {
            char velocidad[20];
            strcpy(velocidad, mensaje.mtext + 1);

            sprintf(buf, "SPEED %s", velocidad);
            printf("-> Enviando: %s\n", buf);
            sendto(socketfd, buf, strlen(buf), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        }
        else if (tipo_comando == COMM_RPM) {
            int rpm_val;
            memcpy(&rpm_val, mensaje.mtext + 1, sizeof(int));

            sprintf(buf, "RPM %d", rpm_val);
            printf("-> Enviando: %s\n", buf);
            sendto(socketfd, buf, strlen(buf), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        }
        else if (tipo_comando == COMM_BYE) {
            sprintf(buf, "BYE");
            printf("-> Enviando: %s\n", buf);
            sendto(socketfd, buf, strlen(buf), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
            break;
        }
    }

}



/*
    sprintf(buf, "BYE", puerto);
    printf("Enviamos %s\n", buf);
    sendto(socketfd, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    sleep(3);

    close(socketfd);
*/