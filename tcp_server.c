#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/errno.h> 
#include <arpa/inet.h> 

#define SIZE 1024

int main(int argc, char **argv){
	
	int sock_fd, sock_req, sock_len;
	int ret, msg_count = 0;
	struct sockaddr_in sa, ca; 
	char buffer[SIZE];
	char hostname[128];	

	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)	{
		perror("Failed to create socket");
		exit(EXIT_FAILURE);
	}
	
	printf("\nServer socket descriptor created successfully!\n");
	
	memset((char *)&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(1050);
	
	if (bind(sock_fd, (struct sockaddr *)&sa, sizeof(sa)) < 0){
		perror("Failed to bind the socket!");
		exit(EXIT_FAILURE);
	}

	printf("\nBind successfull!\n");

	if (listen(sock_fd, 1) < 0){
		perror("Listen Failed!\n");
		exit(EXIT_FAILURE);
	}
	
	gethostname(hostname, 128);
	
	printf("\nServer started on %s, listening on port %d\n", hostname, ntohs(sa.sin_port));	
	sock_len = sizeof(ca);
	
	
	sock_req = accept(sock_fd, (struct sockaddr*)&ca, &sock_len);
	if(sock_req < 0){
		perror("Accept Failed!\n");
		exit(EXIT_FAILURE);
	}
	
	printf("\nReceived a connection from %s port %d\n", inet_ntoa(ca.sin_addr), ntohs(ca.sin_port));
	
	while(1){	
		ret = recv(sock_req, buffer, SIZE, 0);
		if(ret > 0){
			buffer[ret] = 0; 			
			printf("\n%d byte message received from client: %s\n", ret, buffer);
		}
		else{
			printf("\nError Recieving from client!!\n");
			exit(EXIT_FAILURE);
		}
		
		sprintf(buffer, "\nAck message no: %d\n", msg_count++);

                printf("\nResponding to client....\n");
		
		if(send(sock_req, buffer, strlen(buffer), 0) < 0){
                        perror("\nError sending to client!!\n");
			exit(EXIT_FAILURE);
                }

		printf("\nWaiting for next message from client.....\n");
	}	
}
