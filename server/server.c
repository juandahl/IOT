// to compile: gcc -o server2 server2.c -pthread
// to run 
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "services.c"

//buffer declaration

void processRequest(int newSocket, char buffer[256]){
	//parseEntryDB(newSocket, buffer);
	//getSensors(newSocket);
	//getTypes(newSocket);
	//getPieces(newSocket);
	char * buff = 0;
	char reply[20480];
	char file [20480];
	long length; 
	FILE *fp;
	strcpy(reply, "HTTP/1.1 200 OK\n");
	char *line;
	line = strtok(buffer, "\n");

	printf("%s\n", line);
	if ((strncmp(line, "GET / HTTP/1.1", 10) == 0) || (strncmp(line, "GET /index.html HTTP/1.1", 10) == 0) )
		completePageIndex(fp, &reply);
		
	if (strncmp(line, "GET /Consumption.html HTTP/1.1", 21) == 0 )
		completePageConsumption(fp, &reply);

	if (strncmp(line, "GET /formCapteur.html HTTP/1.1", 21) == 0 )
		createPageForm(fp, &reply);

	if (strncmp(line, "GET /state.html HTTP/1.1", 21) == 0 )
		completePageState(fp, &reply);

	send(newSocket , reply , strlen(reply), 0);
	sleep(1);
	if (fp)
		fclose(fp);
}

void *newRequest(void *arg){
	//variables declaration
	char buffer[256];
	int newSocket = *((int *)arg);

	//Receive a message from client
  	read(newSocket, buffer, sizeof(buffer)-1);
	printf("%s\n", "Message received");
	
	//Code to process the request
	processRequest(newSocket, buffer);

	close(newSocket);
	}


void startServer(int port){
		int portno = port; 


	//declaration and initialization
    struct sockaddr_in server_ad, client_ad;


	//Create socket
	int socket_fd = socket (AF_INET , SOCK_STREAM,  0);

	//Prepare the sockaddr_in structure
	server_ad.sin_family = AF_INET;
	server_ad.sin_addr.s_addr = INADDR_ANY;
	server_ad.sin_port = htons (portno);


	//bind
    bind(socket_fd, (struct sockaddr*)&server_ad, sizeof(server_ad)); 
	int clilen = sizeof(struct sockaddr_in);
  
  	//listen
	listen ( socket_fd, 5 ) ;
	while (1){
		printf("%s\n", "Accepting conection with client side...");
		
		//acept
		int client_socket_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL);
		printf("%s\n", "Connection accepted");

		pthread_t server_thread;
		int *new_sock = malloc(1);
		*new_sock = client_socket_fd;

		//for each client request creates a thread and assign the client request to it to process
        //so the main thread can entertain next request
		pthread_create(&server_thread, NULL, newRequest, (void*) new_sock);
				
	}
}

int main(int argc , char *argv[])
{
	startServer(atoi(argv[1]));
	return 0;	
}


