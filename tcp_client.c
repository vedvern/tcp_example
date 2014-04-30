#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <arpa/inet.h>

#define SIZE 1024

int main(int argc, char **argv){
	
	struct sockaddr_in ca, sa;
	int sock_fd;
	int i, ret;
	char *server = "127.0.0.1";
	char buffer[SIZE];
	
	if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("\nFailed to create client socket\n");
		exit(EXIT_FAILURE);
	}

	printf("\nClient socket descriptor created successfully!\n");
	
	memset((char *)&ca, 0, sizeof(ca));
	ca.sin_family = AF_INET;
	ca.sin_addr.s_addr = htonl(INADDR_ANY);
	ca.sin_port = htons(0);

	memset((char *)&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(1050);

	if(inet_aton(server, &sa.sin_addr) == 0){
                perror("\nError!!\n");
                exit(EXIT_FAILURE);
        }

	printf("\nReady to connect to %s port %d\n", server, htons(sa.sin_port));

	if (connect(sock_fd, (struct sockaddr *)&sa, sizeof(sa)) < 0){
		perror("\nFailed to get connect to server\n");
                exit(EXIT_FAILURE);
	}
	
	printf("\nConnection established with server IP: %s port %d\n", inet_ntoa(sa.sin_addr), ntohs(sa.sin_port));
	for(i=0; i<5; i++){
		printf("\nEnter your message: ");
		fgets(buffer, SIZE, stdin);
	
		if(send(sock_fd, buffer, strlen(buffer), 0) < 0){
                        perror("\nError sending to client!!\n");
			exit(EXIT_FAILURE);
                }
		
		printf("\nMessage sent!\n");
		
		ret = recv(sock_fd, buffer, SIZE, 0);
                if(ret > 0){
                        buffer[ret] = 0;
                        printf("\n%s\n", buffer);
                }
                else{
                        printf("\nError Recieving ACK from server!!\n");
			exit(EXIT_FAILURE);
                }
	}

	close(sock_fd);	
	return 1;
}
