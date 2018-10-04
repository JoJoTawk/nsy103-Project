#include "unphead.h"


int main(int argc, char* argv[])
{
    int sockfd, ret;
	struct sockaddr_in servaddr = { PF_INET };
	socklen_t addrlen = sizeof(servaddr);
	struct umsg msg;    
	ucnode_t head;
/*
	if(argc != 3)
	{
		fprintf(stderr, "usage : %s [ip] [port]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
*/
	if((ret = atoi("8088")) < 1024 || ret > 65535)
		CERR("Error port = %s ", argv[2]);
	
	IF_CHECK(inet_aton("127.0.0.1", &servaddr.sin_addr));
	servaddr.sin_port = htons(2001); 

	IF_CHECK(sockfd = socket(AF_INET, SOCK_DGRAM, 0));

	IF_CHECK(bind(sockfd, (struct sockaddr*)&servaddr, addrlen));

	head = _new_ucnode(&servaddr); 
	strcpy(head -> name, "server");

	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);
	bzero(&peeraddr, sizeof(peeraddr));
pid_t pid = fork();
if(pid == 0)
	{
		while(fgets(msg.text, _INT_TEXT, stdin) != NULL)
		{
			if(strcasecmp(msg.text, "_shdn\n") == 0)
			{
			printf("\r\n server is shutting down ..\r\n");
			kill(pid, SIGKILL);
			exit(1);
			}
			if(strcasecmp(msg.text, "_who\n") == 0)
			{

			_who_server(head);
			}
			else
			{
				msg.type = '2';
				IF_CHECK(sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)&servaddr, addrlen));
			}
		}
		close(sockfd);
		kill(getppid(), SIGKILL);
		exit(0);
	}
else{
	for(;;)
	{
		bzero(&msg, sizeof(msg));
		msg.name[_INT_NAME - 1] = msg.text[_INT_TEXT - 1] = '\0';
		IF_CHECK(recvfrom(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)&peeraddr, &peerlen));
		fprintf(stdout, "msg is [%s:%d] => [%c:%s:%s] \n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port), msg.type, msg.name, msg.text);

		switch(msg.type)
		{
			case '1':
				_login_ucnode(head, sockfd, &peeraddr, &msg);
				break;
			case '2':
				_broadcast_ucnode(head, sockfd, &peeraddr, &msg);
				break;
			case '3':
				_quit_ucnode(head, sockfd, &peeraddr, &msg);
				break;
			case '4':
				_list_ucnode(head, sockfd, &peeraddr, &msg);
				break;
			default:
				fprintf(stderr, "msg is error! [%s:%d] => [%c:%s:%s] \n", inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port), msg.type, msg.name, msg.text);
				_quit_ucnode(head, sockfd, &peeraddr, &msg);
				break;
		}        
	}
}
	close(sockfd);
	_destroy_ucnode(&head);

	return 0;
}

