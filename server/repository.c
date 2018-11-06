// gcc -o remplissage remplissage.c -lsqlite3
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// retourne un entier aléatoire entre 0 et n
int int_rand(int n){
  return (rand()%n);
}

// return un float aléatoire entre 0 et f
float float_rand(float f){
  return (((float)rand()/(float)(RAND_MAX)) * f);
}

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

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;

   char result[512] = "";


   for(i = 0; i<argc; i++){
      strcat(data, azColName[i]);
      strcat(result, " = ");
      strcat(result, argv[i] ? argv[i] : "NULL");
      strcat(result, "\n");  
   }
   
   strcat(result,"\n");
   printf("%s\n", result );
//   fprintf(stderr, "%s: ", (const char*)data);
   return 0;
}


int getSensorsDB(){
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
  sprintf(req,"SELECT * FROM Capteurs;");

  // préparation de la requête
  rc=sqlite3_prepare_v2(db, req, -1, &stmt, 0);
  rc = sqlite3_exec(db, req, callback, (void*)data, &err_msg);

  // fermeture de la base de données
  sqlite3_close(db);
  return 0;

}


int getTypesDB(){
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
  sprintf(req,"SELECT * FROM TypeCapteurs;");

  // préparation de la requête
  rc=sqlite3_prepare_v2(db, req, -1, &stmt, 0);
  rc = sqlite3_exec(db, req, callback, (void*)data, &err_msg);

  // fermeture de la base de données
  sqlite3_close(db);
  return 0;

}

int getPiecesDB(){
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
  sprintf(req,"SELECT * FROM Pieces;");

  // préparation de la requête
  rc = sqlite3_exec(db, req, callback, (void*)data, &err_msg);

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
