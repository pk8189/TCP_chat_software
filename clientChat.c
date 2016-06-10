#include <stdio.h>	
#include <stdlib.h>	// needed for exit()
#include <sys/socket.h>	// needed for socket()
#include <netinet/in.h>	// needed for the sockaddr_in structure
#include <sys/time.h>	// needed for ctime()
#include <strings.h>	// needed for bzero()
#include <arpa/inet.h> // needed for inet_addr()
#include <unistd.h>
#include <string.h>	// needed for strlen

struct sockaddr_in server_addr;
uint16_t server_port;

void usage();

int main(int argc, char *argv[]){
	int sockfd;
	time_t ticks;
	struct hostent *server;
	int n, bytes_to_read, bye;
	char *buff, buffer[121];
	char t[256];

		
	if (argc < 3){
		printf("IP address and/or port missing!\n");
		usage();
	}

	server_port = atoi(argv[1]);
//	printf("prot no: %d\n", server_port);
	ticks = time(NULL);
	printf("\nClient starts running at %s\n", (char *) ctime(&ticks));
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		fprintf(stderr, "\nError opening socket!\n");
		exit(EXIT_FAILURE);
	}

	bzero((char *) &server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[2]);
	server_addr.sin_port = htons(server_port);

	if(connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
		fprintf(stderr, "\nError connecting to the server\n");
		exit(EXIT_FAILURE);
	} 

	printf("\nCommunicate to the server :)\n\n");
	
	
	do{
	ticks = time(NULL);
	strcpy(t,(char *) ctime(&ticks));
	t[strlen(t)-1] = '\0';	
//	printf("Client[%s]: ", (char *) ctime(&ticks));
	printf("Client[%s]: ", t);

	gets(buffer);
	write(sockfd, buffer, 120);
	if(strcmp(buffer, "bye") == 0)
		break;
	
	//bye = strcmp(buffer, "bye");


	ticks = time(NULL);
	strcpy(t, (char  *) ctime(&ticks));
	t[strlen(t)-1] = '\0';
//	printf("Server[%s]: ", (char *) ctime(&ticks));
	printf("Server[%s]: ", t);
	buff = buffer;
	bytes_to_read = 120;
	while((n = read(sockfd, buff, bytes_to_read)) > 0){
		buff += n;
		bytes_to_read -= n;	
	}
	printf("%s\n",buffer);
	if(strcmp(buffer, "bye") == 0)
		break;
	}while(bye!=0);

	close(sockfd);
    printf("\n[Server Connection Terminated by Client]!\n");
	return 0;
}

void usage(){
	fprintf(stderr, "Command format: ./client port_number IP_address\n");
	exit(EXIT_FAILURE);
}
