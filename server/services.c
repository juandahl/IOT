#include<stdio.h>
#include<string.h>    //strlen
#include<unistd.h>    //write
#include <stdlib.h>
#include <time.h>
#include <curl/curl.h>
#include "repository.c"


//SEGURAMENTE SE BORRA. VER
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

void parseWeather(char *buff)
{
	strcat(buff,"</td>\n");
    strcat(buff,"<td>");

}

//Implementation apiREST get
int getWheather(char *response){
  CURL *curl;
  CURLcode res;
  char *s;
 
  curl = curl_easy_init();
  if(curl) {
	//simple request commented
	//curl_easy_setopt(curl, CURLOPT_URL, "https://api.openweathermap.org/data/2.5/weather?q=Paris,fr&appid=0e1b6106f4fb2445ef36c66b4dddbeda");
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openweathermap.org/data/2.5/forecast?q=Paris&mode=json&units=metric&cnt=5&appid=0e1b6106f4fb2445ef36c66b4dddbeda");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

    // Perform the request, res will get the return code  
    res = curl_easy_perform(curl);

    // Check for errors  
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    // always cleanup  
    curl_easy_cleanup(curl);
  }
	printf("%s\n", s);

  return 0;

}

//HTML METHODS
void completePageState(FILE *fp, char *buff)
{
	fp=fopen("../client/state.html", "r");
	strcat(buff, "Content-Type: text/html\n\n");

	//Load the html in buff string
	char line[256];
	while (fgets(line, sizeof(line), fp))  {
		if(strncmp(line, "  <tbody>", 9) == 0)
		{
			strcat(buff, (char*)line);
			getSensorsDB(buff);
		}
		else
			strcat(buff, (char*)line);
	}
	fclose(fp);
}

void completePageConsumption(FILE *fp, char *buff)
{
	strcat(buff, "Content-Type: text/html\n\n");
	fp=fopen("../client/Consumption.html", "r");

	//Load the html in buff string
	char line[256];
	while (fgets(line, sizeof(line), fp))  {
		if(strncmp(line, "  <tbody>", 9) == 0) //upadte
		{
			strcat(buff, (char*)line);
			getSensorsDB(buff);
		}
		else
			strcat(buff, (char*)line);
	}
	fclose(fp);
}

void createPageForm(FILE *fp, char *buff)
{
	strcat(buff, "Content-Type: text/html\n\n");
	fp=fopen("../client/formCapteur.html", "r");

	char line[256];
	while (fgets(line, sizeof(line), fp))
		strcat(buff, (char*)line);
	fclose(fp);
}

void completePageIndex(FILE *fp, char *buff)
{
	strcat(buff, "Content-Type: text/html\n\n");
		fp=fopen("../client/index.html", "r");

	//Load the html in buff string
	char line[256];
	while (fgets(line, sizeof(line), fp))  {
		if(strncmp(line, "	                    <div class=\"w3-cell-row\">", 40) == 0) //upadte
		{
			strcat(buff, (char*)line);
			//parseWeather(buff);
		}
		else
			strcat(buff, (char*)line);
	}
	fclose(fp);
}
