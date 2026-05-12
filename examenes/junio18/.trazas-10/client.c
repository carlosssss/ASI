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

#define CLAVE 0x25203218	// Clave de los recursos. Sustituir por DNI.

#define SIZE_SHM 4096		// Tamaño del segmento de memoria compartida

#define MAX_DEV	5		// Máximo # dispositivos # [0..4]


/* El dispositivo 1 comienza en la posición 0 de la memoria
	El dispositivo 2 en la posición 0+OFFSET
	El dispositivo 3 en OFFSET*2
	El dispositivo 4 en OFFSET*3
*/

/* Formato de un registro de dispositivo */

struct shm_dev_reg{
	int estado;	/* 1 activo, cualquier otra cosa libre */
	int num_dev;	/* numero de dispositivo #[1..4] */
	char descr[16];	/* descripción del dispositivo*/
	int contador[4];	/* contadores para el dispositivo*/
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

/* Comandos DUMP cola de mensajes mediante señales */

#define MSQ_TYPE_BASE 100
#define DUMP_ALL -1			//Código para el dump de la totalidad de los dispositivos
#define DEV_DUMP 1500		// Posición del dispositivo del que se pide el dump



int main(int argc, char *argv[]){
    int bytes;
    int num_dev;
    int cont[4];
    char buf[100];
    char respuesta[5];
    char descripcion[16];

    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_addr.s_addr = inet_addr(SERVERIP);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    int shmid = shmget(CLAVE, 0, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("Error shmget. (Prueba a cambiar el #define CLAVE temporalmente)");
        exit(1);
    }

    bind(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    socklen_t addr_len = sizeof(serv_addr);

    //struct shm_dev_reg dispositivos[5];

    struct shm_dev_reg *dispositivos = (struct shm_dev_reg *) shmat(shmid, NULL, 0);

    while((bytes = recvfrom(socketfd, buf, 20, 0, (struct sockaddr*)&serv_addr, &addr_len)) > 0){
        printf("Recibido: %s\n", buf);
        memcpy(&num_dev, buf + 1, sizeof(int));
        memcpy(&descripcion, buf + 1 + sizeof(int), sizeof(char)*16);

        dispositivos[num_dev].estado = 1;
        dispositivos[num_dev].num_dev = num_dev;
        //dispositivos[num_dev].descr = descripcion;
        //dispositivos[num_dev].contador = cont;

        //memcpy(direccion, dispositivos, sizeof(dispositivos));

        respuesta[0] = 'O';
        memcpy(respuesta + 1, &num_dev, sizeof(int));
        printf("Enviando: %s\n", respuesta);
        sendto(socketfd, respuesta, 5, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    }
}
