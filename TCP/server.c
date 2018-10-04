
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

struct info_clt  {
    
    char utilisateur[100];
    int ID;
    char IP[INET_ADDRSTRLEN];
    int c;

};

int n = 0;
struct info_clt clients[100];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void partage(char *message) {
    
    int a;
    
    for(a = 0; a < n; a++){
        if(send(clients[a].c,message,strlen(message),0) < 0) {
            perror("échec d'envoi");
            continue;
        }
    }
}

void disconnecte(char surnom[]) {

    pthread_mutex_lock(&mutex);
    char resultat[600];
    int i,j,find = -1;
    memset(resultat, 0x00, sizeof(resultat));
    
    for(i = 0; i < n; i++) {
        if(strcmp(clients[i].utilisateur, surnom) == 0) {
            
            printf("client trouve:%s \n", clients[i].utilisateur);
            find = i;
            strcpy(resultat, surnom);
            strcat(resultat," quitter le groupe \n");
            resultat[strlen(resultat)] = '\0';
        }
    }
    
    if (find > -1) {
        
        close(clients[find].c);
        for(i = 0; i < n; i++) {
            if(clients[i].c == clients[find].c) {
                j = i;
                while(j < n-1) {
                    clients[j] = clients[j+1];
                    j++;
                }
            }
        }
        n--;
        partage(resultat);
        memset(resultat, 0x00, sizeof(resultat));
    
    } else {
        printf("Cet utilisateur n'existe pas \n");
    }
    
    pthread_mutex_unlock(&mutex);
}

char shdn[] = "_shdn";

void *keyb()  {
    
    int longueur;
    char kill[500],space,surnom[100],message[500];
    while(fgets(message,500,stdin) > 0) {
        
        if(longueur < 0) {
            perror("aucun message n'a été envoyé");
            exit(1);
        }
        
        if (strlen(message) == strlen(shdn) + 1 && message[0] == '_' && message[1] == 's' && message[2] == 'h' && message[3] == 'd' && message[4] == 'n') {
            exit(1);
        } else {
            
            if (message[0] == '_' && message[1] == 'k' && message[2] == 'i' && message[3] == 'l' && message[4] == 'l') {
                
                sscanf(message, "%s%c%s", kill, &space, surnom);
                disconnecte(surnom);
            }
        }
        memset(message,'\0',sizeof(message));
    }
}


void envoiClts(char *message,int courant)  {
    
    int i;
    for(i = 0; i < n; i++) {
        if(clients[i].c != courant) {
            if(send(clients[i].c,message,strlen(message),0) < 0) {
                perror("échec d'envoi de message");
                continue;
            }
        }
    }
}

char quitte[] = "_quitte";
char who[] = "_who";

void *receiveMessage(void *sock) {
	
    struct info_clt client = *((struct info_clt *)sock);
	char message[500],resultat[600];
    int longueur,i,j;
    memset(message, 0x00, sizeof(message));
    memset(resultat, 0x00, sizeof(resultat));
    while((longueur = recv(client.c,message,500,0)) > 0) {
        
        if (strlen(message) == strlen(who) + 1 && message[0] == '_' && message[1] == 'w' && message[2] == 'h'
                    && message[3] == 'o') {
            
            memset(resultat, 0x00, sizeof(resultat));
            for(i = 0; i < n; i++) {
                if (i == n - 1) {
                    strcat(resultat, clients[i].utilisateur);
                } else {
                    strcat(resultat, clients[i].utilisateur);
                    strcat(resultat, ", ");
                }
            }
            strcat(resultat, "\n");
            resultat[strlen(resultat)] = '\0';
            if(send(client.c,resultat,strlen(resultat),0) < 0) {
                perror("échec d'envoi");
            }
            
            memset(message, 0x00, sizeof(message));
            memset(resultat, 0x00, sizeof(resultat));
        
        } else
            if(strlen(message) == strlen(quitte) + 1 && message[0] == '_' && message[1] == 'q' && message[2] == 'u' && message[3] == 'i' && message[4] == 't') {
            
            } else {
            
                strcpy(resultat, client.utilisateur);
                strcat(resultat,": ");
                strcat(resultat,message);
                message[longueur] = '\0';
                resultat[strlen(resultat)] = '\0';
                envoiClts(resultat,client.c);
                memset(message, 0x00, sizeof(message));
                memset(resultat, 0x00, sizeof(resultat));
            }
        }
	
    pthread_mutex_lock(&mutex);
    strcpy(resultat, client.utilisateur);
    strcat(resultat, " quitter le groupe \n");
    resultat[strlen(resultat)] = '\0';
    printf("%s disconnected\n",client.utilisateur);
    
    for(i = 0; i < n; i++) {
		if(clients[i].c == client.c) {
			j = i;
            while(j < n-1)  {
				clients[j] = clients[j+1];
				j++;
			}
		}
	}
n--;
    
partage(resultat);
    
memset(message,'\0',sizeof(message));
    
memset(resultat,'\0',sizeof(resultat));
	
pthread_mutex_unlock(&mutex);

}


int main(int argc,char *argv[]) 
{
    
printf("Démarrage du serveur ...)\n");
    
printf("Attendez que le client se connecte ... si vous voulez fermer le serveur, tapez simplement: _shdn");

	
struct sockaddr_in my_addresultatse,t_addr;
	
int my_socket,sock,numport,longueur;
	
socklen_t taddresultatse_size;
	
pthread_t recvt, keybThread;
    
char resultat[600],IP[INET_ADDRSTRLEN];
    
struct info_clt client;
	
if(argc > 2) 
{
		
printf("trop d'argument ...");
		
exit(1);
	
}

	
numport = atoi(argv[1]);

	
my_socket = socket(AF_INET,SOCK_STREAM,0);
	
memset(my_addresultatse.sin_zero,'\0',sizeof(my_addresultatse.sin_zero));


	my_addresultatse.sin_family = AF_INET;
	
my_addresultatse.sin_port = htons(numport);
	
my_addresultatse.sin_addr.s_addr = inet_addr("127.0.0.1");
	
taddresultatse_size = sizeof(t_addr);

	
if(bind(my_socket,(struct sockaddr *)&my_addresultatse,sizeof(my_addresultatse)) != 0) 
{
		
perror("liaison inutile !!!");
		
exit(1);
	
}

	
if(listen(my_socket,5) != 0) 
{
		
perror("ecoute inutile !!");
		
exit(1);
	
}

    
pthread_create(&keybThread, NULL, keyb, NULL);
    
    
memset(resultat, 0x00, sizeof(resultat));
	
while(1) 
{
		
if((sock = accept(my_socket,(struct sockaddr *)&t_addr,&taddresultatse_size)) < 0) 
{
			
perror("accepter sans succes !!");
			
exit(1);
		
}
		
pthread_mutex_lock(&mutex);
		
inet_ntop(AF_INET, (struct sockaddr *)&t_addr, IP, INET_ADDRSTRLEN);
		
printf("%s connecte",IP);
		
client.c = sock;
		
strcpy(client.IP,IP);
		
clients[n].c = sock;
        
clients[n].ID = n+1;
        
strcpy(clients[n].IP,IP);
        
        
memset(clients[n].utilisateur, 0x00, sizeof(clients[n].utilisateur));
        
longueur = recv(clients[n].c, clients[n].utilisateur, 100, 0);
        
while (longueur < 0) 
{
            
longueur = recv(clients[n].c, clients[n].utilisateur, 100, 0);
        
}
       
 
printf("Utilisateur: %s\n", clients[n].utilisateur);
        
clients[n].utilisateur[strlen(clients[n].utilisateur)] = '\0';
        
strcpy(resultat, clients[n].utilisateur);
        
strcat(resultat,"a ete ajoute au groupe");
        
resultat[strlen(resultat)] = '\0';
        
envoiClts(resultat, clients[n].c);
        
memset(resultat, 0x00, sizeof(resultat));
		
pthread_create(&recvt,NULL,receiveMessage,&clients[n]);
        
n++;
		
pthread_mutex_unlock(&mutex);
	
}
	
return 0;

}
