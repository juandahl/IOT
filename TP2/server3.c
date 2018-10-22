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
#include "repository.c"

//buffer declaration
char buffer [256];


void *insert_DB(void *arg){
	//variables declaration
	char buffer[256];
	char val[256];
	char *id_capteur;
	char *value;
	int valint;
	int id;
	int newSocket = *((int *)arg);

	//Receive a message from client
  	read(newSocket, buffer, sizeof(buffer)-1);
	printf("%s\n", "Message received");
	printf("%s%s\n","buffer: ", buffer);

	//Code to process the request

	//parsing input
	id_capteur = strtok(buffer, " ");
	value = strtok(NULL, " ");
	strcpy(val, value);
	id = atoi(id_capteur);
	valint = atoi(val);

	printf("%s%d\n","type: " , id);
	printf("%s%d\n","value: " , valint);

	updateDB(id, valint);

	//Send the message back to client
	write(newSocket, "Value added successfully", 255);

	//close
	close(newSocket);
	}


int main(int argc , char *argv[])
{
	int portno = atoi(argv[1]); 


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
		pthread_create(&server_thread, NULL, insert_DB, (void*) new_sock);
				
	}
	return 0;	
}


