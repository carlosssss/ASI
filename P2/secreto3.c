#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


struct mensaje {
    long mtype;
    char mtext[100];
};

int main(int argc, char *argv[]){
    int msqid;
    struct mensaje msg;

    key_t clave = 0x25203218L;

    msqid = msgget(clave, 0666);

    msgrcv(msqid, &msg, sizeof(msg.mtext), 0, 0);
    
    printf("\n--- SECRETO 3 (Tipo de mensaje) ---\n");
    printf("Valor: %ld\n", msg.mtype);
    
    printf("\n--- SECRETO 4 (Texto del mensaje) ---\n");
    printf("Valor: %s\n\n", msg.mtext);

    //secreto 4
    printf("enter para seguir");
    getchar();
    msqid = msgget(clave, IPC_CREAT | 0666);
    char texto_viejo[100];
    strcpy(texto_viejo, msg.mtext);
    sprintf(msg.mtext, "<%ld>%s", msg.mtype, texto_viejo);
    msg.mtype = 1;
    msgsnd(msqid, &msg, sizeof(msg.mtext), 0); 

    return EXIT_SUCCESS;
}
