// gcc -o remplissage remplissage.c -lsqlite3
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int readDB(){
  sqlite3 *db;
  char *err_msg;
  sqlite3_stmt *stmt;
  char req[255];
  int rc, s;

  // initialisation de la graine pour les nbs aléatoires
  srand((unsigned int)time(NULL));

  // ouverture de la base de données
  rc=sqlite3_open("database.db", &db);

  // lecture dans la base
  
  char reqSelect[255] = "SELECT * FROM Mesure;";
  
  // requête SQL select
  sprintf(req,"SELECT * FROM Mesure;");

  // préparation de la requête
  rc=sqlite3_prepare_v2(db, req, -1, &stmt, 0);
  // parcours des enregistrements
  while(1){
    // lecture de l'enregistrement suivant
    s=sqlite3_step(stmt);
    // enregistrement existant
    if (s==SQLITE_ROW){
      // récupération de la valeur de la colonne valeur
      const unsigned char *val=sqlite3_column_text(stmt, 2);
      //add to web graphic
    }
    // parcours terminé
    else if (s==SQLITE_DONE)
      break;
  }
  // fermeture de la base de données
  sqlite3_close(db);
  return 0;

}

static int callback(void * sensor, int argc, char **argv, char **azColName){
    strcat(sensor,"<tr>\n");
    char coord[20];
    char x[4],y[4],z[4];
    for(int i=0; i<argc; i++){
      if(strcmp(azColName[i], "id_capteur") == 0)
      {
          strcat(sensor,"<th scope=\"row\">");
          strcat(sensor,argv[i] ? argv[i] : NULL);
          strcat(sensor,"</th>\n");
      }
      if ( (strcmp(azColName[i], "port") == 0) || (strcmp(azColName[i], "date_insertion") == 0) ) 
      {
          strcat(sensor,"<td>");
          strcat(sensor,argv[i] ? argv[i] : NULL);
          strcat(sensor,"</td>\n");
      }
      if (strcmp(azColName[i], "id_piece") == 0)  
      {
          strcat(sensor,"<td>");
          getPieceDB(argv[i], sensor);
          strcat(sensor,"</td>\n");
      }

      if (strcmp(azColName[i], "id_type") == 0)  
      {
          strcat(sensor,"<td>");
          getTypeDB(argv[i], sensor);
          strcat(sensor,"</td>\n");
      }

    }

   strcat(sensor,"</tr>\n");  

    return 0;
}


static int responsePiece(void * sensor, int argc, char **argv, char **azColName){
    char coord[20];
    char x[4],y[4],z[4];
    for(int i=0; i<argc; i++){
      if(strcmp(azColName[i], "x") == 0)
          strcpy(x, argv[i]);

      if(strcmp(azColName[i], "y") == 0)
          strcpy(y, argv[i]);

      if(strcmp(azColName[i], "z") == 0)
          strcpy(z, argv[i]);
    }
    strcpy(coord, "(");
    strcat(coord, x);
    strcat(coord, ", ");
    strcat(coord, y);
    strcat(coord, ", ");
    strcat(coord, z);
    strcat(coord, ")");
    strcat(sensor, coord);
    return 0;
}


static int responseType(void * sensor, int argc, char **argv, char **azColName){
    char unite[10],precision[4];
    for(int i=0; i<argc; i++){
      if(strcmp(azColName[i], "unite") == 0)
          strcpy(unite, argv[i]);

      if(strcmp(azColName[i], "precision") == 0)
          strcpy(precision, argv[i]);
    }
    strcat(sensor, unite);
    strcat(sensor,"</td>\n");
    strcat(sensor,"<td>");
    strcat(sensor, precision);


    return 0;
}


int getSensorsDB(char *data){
  sqlite3 *db;
  char *err_msg;
  sqlite3_stmt *stmt;
  char req[255];
  int rc, s;

  // initialisation de la graine pour les nbs aléatoires
  srand((unsigned int)time(NULL));

  // ouverture de la base de données
  rc=sqlite3_open("database.db", &db);
  
  // requête SQL select
  sprintf(req,"SELECT * FROM Capteurs;");

  // préparation de la requête
  rc=sqlite3_prepare_v2(db, req, -1, &stmt, 0);
  rc = sqlite3_exec(db, req, callback, (void*)data, &err_msg);
  // fermeture de la base de données
  sqlite3_close(db);
  return 0;

}


int getTypeDB(char *id, char *sensor){
  sqlite3 *db;
  char *err_msg;
  sqlite3_stmt *stmt;
  char req[255];
  int rc, s;
  const char* data = "Callback function called";


  // initialisation de la graine pour les nbs aléatoires
  srand((unsigned int)time(NULL));

  // ouverture de la base de données
  rc=sqlite3_open("database.db", &db);
  
  // requête SQL select
  strcpy(req,"SELECT * FROM TypeCapteurs WHERE id_type=" );
  strcat(req, id);
  strcat(req, ";");

  // préparation de la requête
  rc=sqlite3_prepare_v2(db, req, -1, &stmt, 0);
  rc = sqlite3_exec(db, req, responseType, (void*)sensor, &err_msg);

  // fermeture de la base de données
  sqlite3_close(db);
  return 0;

}

int getPieceDB(char *id, char *sensor){
  sqlite3 *db;
  char *err_msg;
  sqlite3_stmt *stmt;
  char req[255];
  int rc, s;


  // initialisation de la graine pour les nbs aléatoires
  srand((unsigned int)time(NULL));

  // ouverture de la base de données
  rc=sqlite3_open("database.db", &db);
  
  // requête SQL select
  strcpy(req,"SELECT * FROM Pieces WHERE id_piece=" );
  strcat(req, id);
  strcat(req, ";");

  // préparation de la requête
  rc = sqlite3_exec(db, req, responsePiece, (void*)sensor, &err_msg);

  // fermeture de la base de données
  sqlite3_close(db);
  return 0;

}


void updateDB(int id,int val){
  sqlite3 *db;
  char *err_msg;
  sqlite3_stmt *stmt;
  char req[255];
  int rc, s;

  // initialisation de la graine pour les nbs aléatoires
  srand((unsigned int)time(NULL));

  // ouverture de la base de données
  rc=sqlite3_open("database.db", &db);

  // insertion de données dans la base
  
      
  // requête SQL insert
  
  sprintf(req,"%s%d%s%d%s","INSERT INTO Mesure(valeur, id_capteur) VALUES(", val,",", id, ");");
  printf("sql=|%s|\n",req);
  // exécution de la requête
  rc = sqlite3_exec(db, req, 0, 0, &err_msg);
}
