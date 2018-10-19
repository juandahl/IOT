// to run: gcc -o server2 server2.c -pthread
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <stdlib.h>
#include<pthread.h>

//buffer declaration
char buffer [256];

struct factures{
	double w;
	double e;
	double d;
}; 
//variables declaration
struct factures *fact; 

void updateValue(struct factures *fact, char type, double value){
	switch(type) {
	   case 'w':
	      (*fact).w += value;
	      break; 
	   case 'e':
	      (*fact).e += value;
		  break; 
	   case 'd':
	      (*fact).d += value;
		  break; 
	}
}


int updateFile(double w, double e, double d){
   FILE *fptr;
//   char dir[255] = "C:\\Users\\juan\\Documents\\EISE 5\\IOT\\TP2\\";
   char name[255] = "factures.txt";
   
//   fptr = fopen( strcat(dir, name),"w");
   fptr = fopen( name,"w");


   fprintf(fptr,"%s%fl\n","eau: ", w);
   fprintf(fptr,"%s%fl\n","electricite: ", e);
   fprintf(fptr,"%s%fl\n","dechets: ", d);

   fclose(fptr);

   return 0;
}

void *server_handler(void *arg){
	//variables declaration
	char buffer[256];
	char val[256];
	char *type;
	char *value;
	double valdouble;
	int newSocket = *((int *)arg);

	//Receive a message from client
  	read(newSocket, buffer, sizeof(buffer)-1);
	printf("%s\n", "Message received");
	printf("%s%s\n","buffer: ", buffer);

	//Code to process the request

	//parsing input
	type = strtok(buffer, " ");
	value = strtok(NULL, " ");
	strcpy(val, value);
	valdouble = atof(val);

	printf("%s%c\n","type: " , type[0]);
	printf("%s%f\n","value: " , valdouble);



	updateValue(fact, *type, valdouble);

	updateFile((*fact).w, (*fact).e, (*fact).d);

	//Send the message back to client
	write(newSocket, value, 255);

	//close
	close(newSocket);
	}


int main(int argc , char *argv[])
{
	fact = (struct factures*)malloc(sizeof(struct factures));

	(*fact).w = 0;
	(*fact).e = 0;
	(*fact).d = 0;

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
		pthread_create(&server_thread, NULL, server_handler, (void*) new_sock);
				
	}
	return 0;	
}


