#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>

struct sockaddr_in server_addr, client_addr;
uint16_t server_port;

void usage(){
	printf("Command format: ./server port_number\n\n");
	printf("\tport_number \tShould be an ephemeral-port address\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
	int sockfd, nsockfd;
	time_t ticks;
	socklen_t client_len;
	char *buff, buffer[121];
	_Bool flag = 1;
	int n, bytes_to_read, bye;
	char t[256];
	

	if (argc == 1){
		printf("Server Port  missing!\n");
		usage();
	}

	server_port = atoi(argv[1]);
	ticks = time(NULL);
	printf("\nServer starts running at %s\n", (char *) ctime(&ticks));
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		fprintf(stderr,"\nError openning socket!\n");
		exit(EXIT_FAILURE);
	}

	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(server_port);

	if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
		fprintf(stderr, "Error binding address to socket\n");
		exit(EXIT_FAILURE);
	}
//	printf("server port no: %d", server_port);

	if(listen(sockfd, 1) < 0){
		fprintf(stderr, "Error creating a passive socket!\n");
		exit(EXIT_FAILURE);
	}

	for( ; ; ){
		client_len = sizeof(client_addr);
		if((nsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_len)) < 0){
			fprintf(stderr, "Error accepting client\n");
			exit(EXIT_FAILURE);		
		}
		
		do{
		ticks = time(NULL);
		strcpy(t,(char *) ctime(&ticks));
		t[strlen(t)-1] = '\0';	
		printf("Client[%s]: ", t);	
		buff = buffer;
		bytes_to_read = 120;
		while((n = read(nsockfd, buff, bytes_to_read) ) > 0){
			buff += n;
			bytes_to_read -= n;
		}		
		printf("%s\n", buffer);
		printf("Server[%s]: ", t);
		if(strcmp(buffer, "bye") == 0)
			break;
		
		gets(buffer);
		write(nsockfd,buffer,120);
		if(strcmp(buffer, "bye") == 0)
			break;
		}while (flag==1);
		
		close(nsockfd);
		printf("[Client Connection Terminated by Server!]\n\n");
		break;
		
	}
	
		//close(sockfd);	
		//printf("\nEnd of program[Connection terminated]!\n");

	return 0;
}
