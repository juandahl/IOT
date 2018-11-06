#include<stdio.h>
#include<string.h>    //strlen
#include<unistd.h>    //write
#include <stdlib.h>
#include <time.h>
#include <curl/curl.h>
#include "repository.c"

void getSensors(int newSocket){
	getSensorsDB();
	//Send the message back to client
	write(newSocket, "Cest fini", 255);

}

void getTypes(int newSocket){
	getTypesDB();
	//Send the message back to client
	write(newSocket, "Cest fini", 255);

}

void getPieces(int newSocket){
	getPiecesDB();
	//Send the message back to client
	write(newSocket, "Cest fini", 255);

}


void parseEntryDB(int newSocket, char buffer[256]){
	char val[256];
	char *id_capteur;
	char *value;
	int valint;
	int id;

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
	write(newSocket, buffer, 255);

}



//Implementation apiREST get
int getWheather(){
  CURL *curl;
  CURLcode res;
 
  curl = curl_easy_init();
  if(curl) {
	//simple request commented
	//curl_easy_setopt(curl, CURLOPT_URL, "https://api.openweathermap.org/data/2.5/weather?q=Paris,fr&appid=0e1b6106f4fb2445ef36c66b4dddbeda");
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openweathermap.org/data/2.5/forecast?q=Paris&mode=json&units=metric&cnt=5&appid=0e1b6106f4fb2445ef36c66b4dddbeda");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
    // Perform the request, res will get the return code  
    res = curl_easy_perform(curl);
    // Check for errors  
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    // always cleanup  
    curl_easy_cleanup(curl);
  }
  return 0;

}

