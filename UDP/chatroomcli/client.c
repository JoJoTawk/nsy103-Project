#include "unphead.h"

int main(int argc, char* argv[]) 
{
	int sockfd, ret;
	struct sockaddr_in servaddr = {PF_INET};
	socklen_t addrlen = sizeof(servaddr);
	pid_t pid;
	struct umsg msg;
	msg.type = '1';
	msg.name[_INT_NAME - 1] = '\0';
	msg.text[_INT_TEXT - 1] = '\0';
/*
	if(argc != 4) 
	{
		fprintf(stderr, "usage : %s [ip] [port] [name]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
*/
	if((ret = atoi("8088")) < 1024 || ret > 65535)
		CERR("Error port = %s ", argv[2]);

	IF_CHECK(inet_aton("127.0.0.1", &servaddr.sin_addr));
	servaddr.sin_port = htons(2001);

	strncpy(msg.name, argv[1], _INT_NAME - 1);	//safe function

	IF_CHECK(sockfd = socket(AF_INET, SOCK_DGRAM, 0));

	IF_CHECK(sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)&servaddr, addrlen));

	IF_CHECK(pid = fork());

	if(pid == 0)
	{
		while(fgets(msg.text, _INT_TEXT, stdin) != NULL)
		{
			if(strcasecmp(msg.text, "_quit\n") == 0)
			{
				msg.type = '3';	              
			    IF_CHECK(sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)&servaddr, addrlen));
				break;
			}
			if(strcasecmp(msg.text, "_who\n") == 0)
			{
				msg.type = '4';
				IF_CHECK(sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)&servaddr, addrlen));
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

	if(pid > 0)
	{
		for(;;)
		{
			bzero(&msg, sizeof(msg));
			IF_CHECK(recvfrom(sockfd, &msg, sizeof(msg), 0, (struct sockaddr*)&servaddr, &addrlen));
			msg.name[_INT_NAME - 1] = msg.text[_INT_TEXT - 1] = '\0';
			switch(msg.type)
			{
				case '1':
					printf("\r\n%s login the chatroom!\r\n", msg.name);
					break;
				case '2':
					printf("\r\n%s say: %s\r\n", msg.name, msg.text);
					break;
				case '3':
					printf("\r\n%s logout the chatroom!\r\n", msg.name);
					break;
				case '4':
					printf("\r\n%s in online\r\n", msg.name);
					break;
				default:
					fprintf(stderr, "\r\n msg is error! [%s:%d] => [%c:%s:%s]\r\n", inet_ntoa(servaddr.sin_addr), ntohs(servaddr.sin_port), msg.type, msg.name, msg.text);
				 	goto __exit;
			}
		}

	__exit:    	    
		close(sockfd);
		kill(pid, SIGKILL);
		waitpid(pid, NULL, -1);
	}

	return 0;
}

