#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

char quitte[] = "_quit";

void *RecMessage(void *sock) {
	
    int longueur;
    int sockk = *((int *)sock);
    char message[500];
    while((longueur = recv(sockk,message,500,0)) > 0) {
        message[longueur] = '\0';
        fputs(message,stdout);
		memset(message,'\0',sizeof(message));
    }
    exit(1);
}

int connectSRV(char nom[], char machine[], char port[]) {
	
    struct sockaddr_in their_addr;
    int m_socket;
    int their_addr_size;
    int numport;
    
    char message[500];
    char nomusr[100];
    char res[600];
    char IP[INET_ADDRSTRLEN];
    int longueur;
    
    pthread_t envoit,recoit;
    
    numport = atoi(port);
    strcpy(nomusr, nom);
    m_socket = socket(AF_INET,SOCK_STREAM,0);
    memset(their_addr.sin_zero,'\0',sizeof(their_addr.sin_zero));
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(numport);
    their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
    if(connect(m_socket,(struct sockaddr *)&their_addr,sizeof(their_addr)) < 0) {
        perror("Connection error");
        exit(1);
    }
	
    inet_ntop(AF_INET, (struct sockaddr *)&their_addr, IP, INET_ADDRSTRLEN);
    longueur = write(m_socket, nomusr, strlen(nomusr));
    
    if (longueur < 0) {
        printf("nomusr n'est pas envoyé au serveur \n");
        
    } else {
//        printf("")
    }
	
    pthread_create(&recoit,NULL,RecMessage,&m_socket);
    memset(message, 0x00, sizeof(message));
    memset(res, 0x00, sizeof(res));
	
    while(fgets(message,500,stdin) > 0) {
		
        strcat(res,message);
		longueur = write(m_socket,res,strlen(res));
		if(longueur < 0) {
			perror("message non envoyé");
			exit(1);
		}
        
        if (strlen(message) == strlen(quitte) + 1 && message[0] == '_' && message[1] == 'q' && message[2] == 'u' && message[3] == 'i' && message[4] == 't') {
            
            exit(1);
        }
        
        memset(message, 0x00, sizeof(message));
        memset(res, 0x00, sizeof(res));
    }
	
    pthread_join(recoit,NULL);
    close(m_socket);
}

int main(int argc, char**argv)  {
    
    char commande[2000];
    char port[2000];
    char nom[2000];
    char connect[2000];
    char espace;
    char machine[2000];
    
    printf("Bienvenue à ChatRoom:) \n");
    printf("Ecrivez '_connect <surnom> <machine> <port>' et connectez-vous à votre serveur maintenant. \n");
    printf("Ecrivez '_quit' pour se déconnecter de votre serveur. \n");
    fputs("", stdout);
    fgets(commande, sizeof (commande), stdin);
    if (commande[0] == '_' && commande[1] == 'q' && commande[2] == 'u' && commande[3] == 'i' && commande[4] == 't') {
        return 0;
    
    } else{
        if (commande[0] == '_' && commande[1] == 'c' && commande[2] == 'o' && commande[3] == 'n' && commande[4] == 'n' && commande[5] == 'e' && commande[6] == 'c' && commande[7] == 't') {
            
            sscanf(commande, "%s%c%s%c%s%c%s", connect, &espace, nom, &espace, machine, &espace, port);
            connectSRV(nom, machine, port);
        } else {
            
            printf("Au revoir.\n");
            return 0;
        
        }
    }
}
