#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>


#define _INT_NAME (64)

#define _INT_TEXT (512)


#define CERR(fmt, ...) \
    fprintf(stderr,"[%s:%s:%d][error %d:%s]" fmt "\r\n",__FILE__, __func__, __LINE__, errno, strerror(errno), ##__VA_ARGS__)

#define CERR_EXIT(fmt, ...) \
    CERR(fmt,##__VA_ARGS__),exit(EXIT_FAILURE)

#define IF_CHECK(code) \
    if((code) < 0) \
		CERR_EXIT(#code)


struct umsg
{
    char type;               
	char name[_INT_NAME];  
	char text[_INT_TEXT]; 
};


typedef struct ucnode
{
    struct sockaddr_in addr;
	char name[_INT_NAME];
	struct ucnode* next;
} *ucnode_t;

static inline ucnode_t _new_ucnode(struct sockaddr_in* pa)
{
	ucnode_t node = (ucnode_t)calloc(sizeof(struct ucnode), 1);    
	if(NULL == node)
		CERR_EXIT("calloc sizeof struct ucnode is error. ");
	node -> addr = *pa;
	node -> name[_INT_NAME - 1] = '\0';
	node -> next = NULL;
	return node;
}

static inline void _insert_ucnode(ucnode_t head, struct sockaddr_in* pa)	//insert a node after head node
{
    ucnode_t node = _new_ucnode(pa);
	node -> next = head -> next;
	head -> next = node;    
}

static void _login_ucnode(ucnode_t head, int sd, struct sockaddr_in* pa, struct umsg* msg)
{
    _insert_ucnode(head, pa);
	//head = head->next;
	strncpy(head -> next -> name, msg -> name, _INT_NAME - 1);
	while(head -> next)
	{
		head = head -> next;
		IF_CHECK(sendto(sd, msg, sizeof(*msg), 0, (struct sockaddr*)&(head->addr), sizeof(struct sockaddr_in)));
	}
}


static void _broadcast_ucnode(ucnode_t head, int sd, struct sockaddr_in* pa, struct umsg* msg)	//broadcast everyone except itself
{
    int flag = 0;
	while(head->next)
	{
		head = head->next;
		if((flag) || !(flag = (memcmp(pa, &(head->addr), sizeof(struct sockaddr_in))==0)))
		{
			IF_CHECK(sendto(sd, msg, sizeof(*msg), 0, (struct sockaddr*)&(head->addr), sizeof(struct sockaddr_in)));
		}
	}
}

static void _list_ucnode(ucnode_t head, int sd, struct sockaddr_in* pa, struct umsg *msg)
{
	int flag = 0;
	while(head -> next)
	{
		head = head -> next;
		if((flag) || !(flag = (memcmp(pa, &(head -> addr), sizeof(struct sockaddr_in)) == 0)))
		{
			bzero(msg, sizeof(*msg));
			strncpy(msg -> name, head -> name, _INT_NAME - 1);
			msg -> type = '4';
			IF_CHECK(sendto(sd, msg, sizeof(*msg), 0, (struct sockaddr*)pa, sizeof(struct sockaddr_in)));
		}
	}
}
static void _who_server(ucnode_t head)
{
	int flag = 0;
	while(head -> next)
	{
		head = head -> next;
		printf("%s",head->name);
			
	
	}
}
static void _quit_ucnode(ucnode_t head, int sd, struct sockaddr_in* pa, struct umsg* msg)
{
    int flag = 0;
	while(head->next)
	{
		if((flag) || !(flag = (memcmp(pa, &head->next->addr, sizeof(struct sockaddr_in))==0)))
		{
			IF_CHECK(sendto(sd, msg, sizeof(*msg), 0, (struct sockaddr*)&head->next->addr, sizeof(struct sockaddr_in)));
			head = head->next;
		}        
		else
		{ 
			ucnode_t tmp = head->next;
			head->next = tmp->next;
			free(tmp);
		}
	}        
}

static void _destroy_ucnode(ucnode_t* phead)
{
    ucnode_t head;
	if((!phead) || !(head=*phead)) return;    
	while(head)
	{
		ucnode_t tmp = head->next;
		free(head);
		head = tmp;
	}    
	*phead = NULL;
}
