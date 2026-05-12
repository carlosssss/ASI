#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/signal.h>

#define SERVER_PORT 3010
#define SERVERIP "127.0.0.1"

#define CLAVE 0x25203218L	// Clave de los recursos. Sustituir por DNI.

#define SIZE_SHM 4096		// Tamaño del segmento de memoria compartida

#define MAX_DEV	4		// Máximo # dispositivos # [1..4]

#define DEVOFFSET 200		// Offset entre dispositivos

/* El dispositivo 1 comienza en la posición 0 de la memoria
	El dispositivo 2 en la posición 0+OFFSET
	El dispositivo 3 en OFFSET*2
	El dispositivo 4 en OFFSET*3
*/

/* Formato de un registro de dispositivo */

struct shm_dev_reg{
	int estado;	/* 1 activo, cualquier otra cosa libre */
	int num_dev;	/* numero de dispositivo #[1..4] */
	char descr[15];	/* descripción del dispositivo*/
	int n_cont;	/* número de contadores para el dispositivo*/
};

/* Detrás del registro del dispositivo (+sizeof(struct shm_dev_reg)) 
   se almacenan uno tras otro sus valores en formato (int)
*/


/* El array de semáforos a crear tiene tamaño 5
	El semáforo 0 no se utilizará
	El semáforo 1 controla el acceso al registro del dispositivo 1 
	El semáforo # controla el acceso al registro del dispositivo # [1..4]
*/
#define MAX_SEM 5	// Número de semáforos en el array

/* Lista de mensajes UDP */
#define HELLO	"HLO"
#define OK	"OK"
#define WRITE	'W'

/* Comandos cola de mensajes */
#define DUMP	'D'

/* Estructura para la lectura de comando + dispositivo: Total 8 bytes */

struct msgq_input {
	char cmd;	/* Comando */
	int num_dev;	/* numero de dispositivo */
};

int main(int argc, char *argv[]){
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_port = htons(3010);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buf[40];

    bind(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    int bytes;

    int shmid = shmget(CLAVE, SIZE_SHM, IPC_CREAT | 0666);
    struct shm_dev_reg *dispositivos = (struct shm_dev_reg *)shmat(shmid, NULL, 0);

    while((bytes = recvfrom(socketfd, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&client_addr, &client_len)) > 0){ 
        if(buf[0] == 'H'){
            int num_dev;
            char descr[15];
            
            memcpy(&num_dev, buf + 1, sizeof(int));
            strcpy(descr, buf + 1 + sizeof(int)); 
            
            printf("Recibido H: Alta del dispositivo %d (%s)\n", num_dev, descr);

            struct shm_dev_reg *registro = (struct shm_dev_reg *)((char *)dispositivos + (num_dev * DEVOFFSET));

            registro->estado = 1;
            registro->num_dev = num_dev;
            strcpy(registro->descr, descr);
            registro->n_cont = 0;
            
            char respuesta[100];
            respuesta[0] = 'O';
            memcpy(respuesta + 1, &num_dev, sizeof(int));

            printf("Mandando respuesta O...\n");
            sendto(socketfd, respuesta, 5, 0, (struct sockaddr*)&client_addr, client_len);
        }
    }

    close(socketfd);
    return EXIT_SUCCESS;
}