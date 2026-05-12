#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define FIFO_RD "/tmp/fifo_rd"
#define FIFO_WR "/tmp/fifo_wr"

#define PORT 10000

#define KEY 0x12345678L

#define SIZE 1024

#define OFF_AC_COUNTER 10
#define N_COUNTERS 4


char *tbl_cmd[]={"HE","AC","OK","SC","SQ","NK",0};
char *tbl_dev_name[]={ "FIFO","TCP","UDP" };


int main(int argc,char *argv[]){
    char buf[256];
    int pid = getpid();

	if(argc != 2) {
		fprintf(stderr,"Missing parameters.\nFormat: %s <pidMonitor>\n",argv[0]);
		exit(20);
	}
	printf("Press Enter to start exercise:");
	fgets(buf,sizeof(buf)-1,stdin);

    //HIJO FIFO
/*
    int pidhijo = fork();
    if(pidhijo == 0) {
        int fd_tx = open(FIFO_WR, O_WRONLY);
        printf("HIJO: abierto el fifo\n");
        int mensajesaludo[2];
        mensajesaludo[0] = 0;
        mensajesaludo[1] = pid;
        write(fd_tx, mensajesaludo, sizeof(int) * 2);
        printf("[HIJO] Saludo <0><%d> enviado por FIFO_WR\n", mensajesaludo[1]);

        int fd_rx = open(FIFO_RD, O_RDONLY);
        char buffer[100];
        int ac_count = 0;
        int bytes;
        read(fd_rx, buffer, 2);    
        printf("Recibido: %s por FIFO_RX\n", buffer);

        while(1){
            memset(buffer, 0, sizeof(buffer));
            bytes = read(fd_rx, buffer, 2);
            if(strncmp(buffer, "HE", 2) == 0){
                printf("Recibido: HE por FIFO_RX\n");
            } else if(strncmp(buffer, "AC", 2) == 0){
                int valor;
                read(fd_rx, &valor, sizeof(int));
                ac_count++;
                int respuesta_ac[2];
                respuesta_ac[0] = 1;
                respuesta_ac[1] = ac_count;
                write(fd_tx, respuesta_ac, sizeof(int) * 2);
            } else if (strncmp(buffer, "SC", 2) == 0) {
                int num;
                read(fd_rx, &num, sizeof(int)); 
                
                printf("Recibido: SC <%d>. Solicitando Clave 2...\n", num);
                
                int respuesta_sc[3];
                respuesta_sc[0] = 2;
                respuesta_sc[1] = 2;
                respuesta_sc[2] = num;
                write(fd_tx, respuesta_sc, sizeof(int) * 3);
            } else if (strncmp(buffer, "OK", 2) == 0) {
                printf("Recibido OK\n");
            }
        }
    }
*/


    //HIJO FIFO
/*
    int pidhijotcp = fork();
    if(pidhijotcp == 0) {
        int socketfd = socket(AF_INET, SOCK_STREAM, 0);
        
        struct sockaddr_in serv_addr;
        serv_addr.sin_port = htons(10000);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        connect(socketfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        char buffer[100];
        int mensajesaludo[2];
        mensajesaludo[0] = 0;
        mensajesaludo[1] = pid;
        send(socketfd, mensajesaludo, sizeof(int) * 2, 0);
        printf("Enviado\n", buffer);
        
        recv(socketfd, buffer, 2, 0);
        printf("Recibido: %s por FIFO_RX\n", buffer);

        int ac_count = 0;
        while(1){
            memset(buffer, 0, sizeof(buffer));
            recv(socketfd, buffer, 2, 0);
            if(strncmp(buffer, "HE", 2) == 0){
                printf("Recibido: HE por FIFO_RX\n");
            } else if(strncmp(buffer, "AC", 2) == 0){
                int valor;
                recv(socketfd, &valor, sizeof(int), 0);
                ac_count++;
                int respuesta_ac[2];
                respuesta_ac[0] = 1;
                respuesta_ac[1] = ac_count;
                send(socketfd, respuesta_ac, sizeof(int) * 2, 0);
            } else if (strncmp(buffer, "SC", 2) == 0) {
                int num;
                recv(socketfd, &num, sizeof(int), 0);
                
                printf("Recibido: SC <%d>. Solicitando Clave 2...\n", num);
                
                int respuesta_sc[3];
                respuesta_sc[0] = 2;
                respuesta_sc[1] = 5;
                respuesta_sc[2] = num;
                send(socketfd, respuesta_sc, sizeof(int) * 3, 0);
            } else if (strncmp(buffer, "OK", 2) == 0) {
                printf("Recibido OK\n");
            }
        }
    }
*/
    int status;
    //waitpid(pidhijo, &status, 0);
    waitpid(pidhijotcp, &status, 0);
	printf("END EXERCISE\n");
}