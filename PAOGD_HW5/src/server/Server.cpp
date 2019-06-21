#include<sys/types.h> 
#include<sys/socket.h> 
#include<unistd.h> 
#include<netinet/in.h> 
#include<arpa/inet.h> 
#include<stdio.h> 
#include<stdlib.h> 
#include<errno.h> 
#include<netdb.h> 
#include<stdarg.h> 
#include<string.h> 
  
#define SERVER_PORT 8000 
#define BUFFER_SIZE 1024 
#define FILE_NAME_MAX_SIZE 512 
  
int main() 
{ 
	struct sockaddr_in server_addr; 
	bzero(&server_addr, sizeof(server_addr)); 
	server_addr.sin_family = AF_INET; 
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	server_addr.sin_port = htons(SERVER_PORT); 

	int serverSocketFd = socket(AF_INET, SOCK_DGRAM, 0); 
	if(serverSocketFd == -1) 
	{ 
		perror("Create socket failed"); 
		exit(1); 
	} 

	if(-1 == (bind(serverSocketFd,(struct sockaddr*)&server_addr,sizeof(server_addr)))) 
	{ 
		perror("Server bind failed"); 
		exit(1); 
	} 
    
    int playerTag[2] = {0, 0};
    char p[2] = {'1','2'};
    int isStart = 0;

	char client1Instructions[10]; 
    bzero(client1Instructions, 10); 
    char client2Instructions[10]; 
    bzero(client2Instructions, 10);

	while(true) 
	{  
		struct sockaddr_in client_addr; 
		socklen_t client_addr_length = sizeof(client_addr); 

		char buffer[BUFFER_SIZE]; 
		bzero(buffer, BUFFER_SIZE); 
        
		if(recvfrom(serverSocketFd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&client_addr, &client_addr_length) == -1) 
		{ 
			perror("Receive data failed"); 
			exit(1); 
		}

		if(strcmp(buffer,"Connect UDP") == 0){
			printf("Connect successfully\n");
			memset(buffer, 0, BUFFER_SIZE);
			for(int i = 0;i < 2;i++) {
				if(playerTag[i] == 0) {
					playerTag[i] = 1;
					char message[15] = "Your id is  .";
					message[11] = p[i];
					strcpy(buffer, message);
					sendto(serverSocketFd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&client_addr,sizeof(client_addr));
					memset(buffer, 0, BUFFER_SIZE);
					break;
				}
			}
		}

		if(playerTag[0] == 1 && playerTag[1] == 1 && isStart < 10) { 
			printf("All player is online.\n Start game!\n");
			isStart++;
			memset(buffer, 0, BUFFER_SIZE);
			char s[10] = "Start";
            strncpy(buffer, s, strlen(s)>BUFFER_SIZE?BUFFER_SIZE:strlen(s)); 
			if(sendto(serverSocketFd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&client_addr,sizeof(client_addr)) < 0) 
			{ 
				perror("Response failed"); 
				exit(1); 
			}
		}

		if(buffer[0] == '1') {
		    if(buffer[1] == 'a' || buffer[1] == 's' || buffer[1] == 'd' || buffer[1] == 'w') {
	    		printf("Receive message from cilent1: %s\n", buffer);
	    	}
	    	if(buffer[1] == 'q') {
	    		printf("Client1 quit!\n");
	    		playerTag[0] = 0;
	    		isStart = 0;
	    	}
		    strcpy(client1Instructions, buffer);
		    memset(buffer, 0, BUFFER_SIZE);
            strncpy(buffer, client2Instructions, strlen(client2Instructions)>BUFFER_SIZE?BUFFER_SIZE:strlen(client2Instructions)); 
			if(sendto(serverSocketFd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&client_addr,sizeof(client_addr)) < 0) 
			{ 
				perror("Response failed"); 
				exit(1); 
			}
		}
		if(buffer[0] == '2') {
			if(buffer[1] == 'a' || buffer[1] == 's' || buffer[1] == 'd' || buffer[1] == 'w') {
	    		printf("Receive message from cilent2: %s\n", buffer);
	    	}
	    	if(buffer[1] == 'q') {
	    		printf("Client2 quit!\n");
	    		playerTag[1] = 0;
	    		isStart = 0;
	    	}
			strcpy(client2Instructions, buffer);
			memset(buffer, 0, BUFFER_SIZE);
            strncpy(buffer, client1Instructions, strlen(client1Instructions)>BUFFER_SIZE?BUFFER_SIZE:strlen(client1Instructions)); 
			if(sendto(serverSocketFd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&client_addr,sizeof(client_addr)) < 0) 
			{ 
				perror("Response failed"); 
				exit(1); 
			}
		}
	}  
	close(serverSocketFd); 
	return 0; 
} 