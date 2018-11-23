#include<stdio.h>
#include<string.h>    //strlen
#include<unistd.h>    //write
#include <stdlib.h>
#include <time.h>
#include <curl/curl.h>
#include <time.h>

#include "repository.c"

#define GMT (+1)

//API response
struct string {
  char *ptr;
  size_t len;
};


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
	//get current day and convert it to string
	char *week[7] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
	time_t rawtime;
	struct tm *info;

	time(&rawtime);
	/* Get GMT time */
	info = gmtime(&rawtime);

	int day = info->tm_wday;

	//call weather API
	int *response[5];
	getWheather(response);
	char strNumber[2];

	//finally create code html
	strcat(buff,"<div class=\"w3-xlarge w3-display-bottomleft w3-padding\">PARIS ");
	sprintf(strNumber, "%d", response[0]);
	strcat(buff,strNumber);
	strcat(buff,"&deg; C</div>\n");	
	strcat(buff,"</div>\n");
	strcat(buff,"<div class=\"w3-cell-row\">\n");

	//create the code html for next days
	for (int i = 1; i < 5; i++)
	{
		strcat(buff,"<div class=\"w3-third w3-center\">\n");
		strcat(buff,"<h3>");
		strcat(buff, week[(day+i)%7]);
		strcat(buff,"</h3>\n");
		strcat(buff,"<p>PARIS ");
		sprintf(strNumber, "%d", response[i]);
		strcat(buff, strNumber);
		strcat(buff,"&deg; C</p>\n");		
		strcat(buff,"</div>\n");
	}
	strcat(buff,"</div>\n");

}


void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

//Implementation apiREST get
int getWheather(int *temp[5]){
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if(curl) {
	struct string s;
	init_string(&s);

	curl_easy_setopt(curl, CURLOPT_URL, "https://api.openweathermap.org/data/2.5/forecast?q=Paris&mode=json&units=metric&cnt=5&appid=0e1b6106f4fb2445ef36c66b4dddbeda");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
	res = curl_easy_perform(curl);
	int j = 0;
	for (int i=0; i< s.len; i++)
	{
		if ( strncmp(s.ptr+i, "\"temp\"", 6) == 0){
			char aux[2];
			if (isdigit((s.ptr+i)[8]))
			{
				aux[0] = (s.ptr+i)[7];
				aux[1] = (s.ptr+i)[8];
			}
			else
			{
				aux[0] = '0';
				aux[1] = (s.ptr+i)[7];
			}
			int number = atoi(aux);
			temp[j] = number;
			j++;
		} 
	}
	free(s.ptr);

	/* always cleanup */
	curl_easy_cleanup(curl);
	}
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
		if(strncmp(line, "<div class=\"w3-xlarge w3-display-bottomleft w3-padding\">PARIS 20&deg; C</div>", 10) == 0) 
		{
			parseWeather(buff);
		}
		else
			strcat(buff, (char*)line);
	}
	fclose(fp);
}
