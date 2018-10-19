#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <stdlib.h>


int main(int argc , char *argv[])
{
    //declaration and initialization
    struct sockaddr_in server_ad, client_ad;

    int portno = atoi(argv[2]); 

    //Prepare the sockaddr_in structure
    server_ad.sin_family = AF_INET;
    //server_ad.sin_addr.s_addr = "127.0.0.1"; //ip by parameter
    server_ad.sin_port = htons(portno);

    //assign IP to the sockaddrr structure
    if(inet_pton(AF_INET, argv[1], &server_ad.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 


    //Create socket
    printf("%s\n", "Creating socket...");
    int socket_fd = socket (AF_INET , SOCK_STREAM,  0);

    int serlen = sizeof(struct sockaddr_in);

    //Connect to remote server
    printf("%s\n", "Connecting to remote server...");
    if  (connect(socket_fd, (struct sockaddr *)&server_ad, sizeof(server_ad) ) < 0 ){
	printf("%s\n", "error al conectar con el servidor");
	return 1;
     }
     
    printf("%s\n", "connected to the server");


    char *buffer[256];
    *buffer = argv[3]; // id_capteur
    char *symbol[1];
    char number[256]; //valeur
    *symbol = " ";

    strncpy(number, argv[4], sizeof(argv[4]));
    strcat(*buffer, *symbol);
    strcat(*buffer, number);
    
    //Send data
    printf("%s%s\n", "data sent: ", *buffer);     
    write(socket_fd, *buffer, sizeof(*buffer)-1) ;

    //Receive a reply from the server
    read ( socket_fd , *buffer ,255);         
    printf("%s%s\n", "Server reply :", *buffer);
    
     
    close(socket_fd);
    return 0;
};