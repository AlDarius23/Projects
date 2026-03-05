#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sqlite3.h> 

#define DB_NAME "quiz_game.db"
#define TOTAL_INTREBARI 5 

int exec_sql(sqlite3 *db, char *sql) {
    char *err = 0;
    int rc, i = 0;
    do {
        rc = sqlite3_exec(db, sql, 0, 0, &err);
        if (rc == SQLITE_BUSY || rc == SQLITE_LOCKED) { usleep(50000); i++; } 
        else break;
    } while (i < 100);
    if (rc != SQLITE_OK) { sqlite3_free(err); }
    return rc;
}

int prep_sql(sqlite3 *db, const char *sql, sqlite3_stmt **stmt) {
    int rc, i = 0;
    do {
        rc = sqlite3_prepare_v2(db, sql, -1, stmt, 0);
        if (rc == SQLITE_BUSY || rc == SQLITE_LOCKED) { usleep(50000); i++; } 
        else break;
    } while (i < 100);
    return rc;
}

void alege_intrebari_noi(sqlite3 *db) {
    exec_sql(db, "DELETE FROM Sesiune;");
    exec_sql(db, "DELETE FROM sqlite_sequence WHERE name='Sesiune';");
    char sql[512];
    sprintf(sql, "INSERT INTO Sesiune (IntrebareID) SELECT ID FROM Intrebari ORDER BY RANDOM() LIMIT %d;", TOTAL_INTREBARI);
    exec_sql(db, sql);
    printf("[Server] Am generat intrebarile noi.\n");
}

int get_id_real(sqlite3 *db, int runda) {
    sqlite3_stmt *res;
    int id_real = -1;
    char sql[256];
    sprintf(sql, "SELECT IntrebareID FROM Sesiune WHERE Ord=%d;", runda);
    if(prep_sql(db, sql, &res) == SQLITE_OK) {
        if(sqlite3_step(res) == SQLITE_ROW) id_real = sqlite3_column_int(res, 0);
    }
    sqlite3_finalize(res);
    return id_real;
}

void init_bd() {
    sqlite3 *db;
    if (sqlite3_open(DB_NAME, &db)) {
        printf("[Eroare] Nu merge DB-ul (%s)!\n", DB_NAME);
        exit(1);
    }
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", 0, 0, 0); 
    sqlite3_busy_timeout(db, 5000); 

    exec_sql(db, "UPDATE Stare SET CineRaspunde=1, IntrebareCurenta=1, JocInceput=0 WHERE Cheie=1;");
    exec_sql(db, "DELETE FROM Jucatori;");
    exec_sql(db, "DELETE FROM sqlite_sequence WHERE name='Jucatori';");
    alege_intrebari_noi(db);

    sqlite3_close(db);
    printf("[Server] DB gata.\n");
}

void auto_reset() {
    sqlite3 *db;
    sqlite3_stmt *res;
    if (sqlite3_open(DB_NAME, &db)) return;
    
    int q_curr = 1;
    if(prep_sql(db, "SELECT IntrebareCurenta FROM Stare WHERE Cheie=1;", &res) == SQLITE_OK) {
        if(sqlite3_step(res) == SQLITE_ROW) q_curr = sqlite3_column_int(res, 0);
    }
    sqlite3_finalize(res);

    if (q_curr > TOTAL_INTREBARI) {
        printf("[Info] Joc nou - resetare.\n");
        exec_sql(db, "UPDATE Stare SET CineRaspunde=1, IntrebareCurenta=1, JocInceput=0 WHERE Cheie=1;");
        exec_sql(db, "DELETE FROM Jucatori;");
        exec_sql(db, "DELETE FROM sqlite_sequence WHERE name='Jucatori';");
        alege_intrebari_noi(db);
    }
    sqlite3_close(db);
}

int logare(char* nume) {
    sqlite3 *db;
    sqlite3_stmt *res;
    int id = -1;
    if (sqlite3_open(DB_NAME, &db)) return -1;
    sqlite3_busy_timeout(db, 5000);

    char sql[256];
    int exista = 0;
    sprintf(sql, "SELECT COUNT(*) FROM Jucatori WHERE Nume='%s';", nume);
    if(prep_sql(db, sql, &res) == SQLITE_OK) {
        if(sqlite3_step(res) == SQLITE_ROW) exista = sqlite3_column_int(res, 0);
    }
    sqlite3_finalize(res);

    if (exista > 0) { sqlite3_close(db); return -2; } 

    sprintf(sql, "INSERT INTO Jucatori (Nume, Scor) VALUES ('%s', 0);", nume);
    exec_sql(db, sql);

    sprintf(sql, "SELECT ID FROM Jucatori WHERE Nume='%s';", nume);
    if(prep_sql(db, sql, &res) == SQLITE_OK) {
        if(sqlite3_step(res) == SQLITE_ROW) id = sqlite3_column_int(res, 0);
    }
    sqlite3_finalize(res);
    sqlite3_close(db);
    return id;
}
void start_joc() {
    sqlite3 *db;
    if (sqlite3_open(DB_NAME, &db)) return;
    exec_sql(db, "UPDATE Stare SET JocInceput=1 WHERE Cheie=1;");
    sqlite3_close(db);
}

void urmatorul(int id_curent) {
    sqlite3 *db;
    sqlite3_stmt *res;
    if (sqlite3_open(DB_NAME, &db)) return;
    sqlite3_busy_timeout(db, 5000);

    int next = -1;
    char sql[256];

    sprintf(sql, "SELECT ID FROM Jucatori WHERE ID > %d ORDER BY ID ASC LIMIT 1;", id_curent);
    if(prep_sql(db, sql, &res) == SQLITE_OK) {
        if(sqlite3_step(res) == SQLITE_ROW) {
            next = sqlite3_column_int(res, 0);
        }
    }
    sqlite3_finalize(res);

    if (next != -1) {
        sprintf(sql, "UPDATE Stare SET CineRaspunde=%d WHERE Cheie=1;", next);
    } else {
        int first = 1;
        if(prep_sql(db, "SELECT MIN(ID) FROM Jucatori;", &res) == SQLITE_OK) {
             if(sqlite3_step(res) == SQLITE_ROW) first = sqlite3_column_int(res, 0);
        }
        sqlite3_finalize(res);
        if (first == 0) first = 1; 

        sprintf(sql, "UPDATE Stare SET CineRaspunde=%d, IntrebareCurenta=IntrebareCurenta+1 WHERE Cheie=1;", first);
    }

    exec_sql(db, sql);
    sqlite3_close(db);
}

void jucator_iesit(int id) {
    if (id == -1) return;
    
    printf("[Info] Client %d a iesit.\n", id);

    sqlite3 *db;
    sqlite3_stmt *res;
    if (sqlite3_open(DB_NAME, &db)) return;
    sqlite3_busy_timeout(db, 5000);

   
    int q_curr = 0;
    if(prep_sql(db, "SELECT IntrebareCurenta FROM Stare WHERE Cheie=1;", &res) == SQLITE_OK) {
        if(sqlite3_step(res) == SQLITE_ROW) q_curr = sqlite3_column_int(res, 0);
    }
    sqlite3_finalize(res);

    
    if (q_curr > TOTAL_INTREBARI) {
        sqlite3_close(db);
        return; 
    }

    int cine_era = 0;
    if(prep_sql(db, "SELECT CineRaspunde FROM Stare WHERE Cheie=1;", &res) == SQLITE_OK) {
        if(sqlite3_step(res) == SQLITE_ROW) cine_era = sqlite3_column_int(res, 0);
    }
    sqlite3_finalize(res);

    
    char sql[256];
    sprintf(sql, "DELETE FROM Jucatori WHERE ID=%d;", id);
    exec_sql(db, sql);
    sqlite3_close(db);

    
    if (cine_era == id) {
        urmatorul(id);
    }
}

int vezi_stare(int id, char* buff) {
    sqlite3 *db;
    sqlite3_stmt *res;
    int cod = 1; 

    if (sqlite3_open(DB_NAME, &db)) return 0;
    sqlite3_busy_timeout(db, 5000);

    int cine = 0, q_curr = 0, start = 0;

    if(prep_sql(db, "SELECT CineRaspunde, IntrebareCurenta, JocInceput FROM Stare WHERE Cheie=1;", &res) == SQLITE_OK) {
        if(sqlite3_step(res) == SQLITE_ROW) {
            cine = sqlite3_column_int(res, 0);
            q_curr = sqlite3_column_int(res, 1);
            start = sqlite3_column_int(res, 2);
        }
    }
    sqlite3_finalize(res);

    if (start == 0) {
        cod = 0; sprintf(buff, "LOBBY");
    }
    else if (q_curr > TOTAL_INTREBARI) {
        cod = 3; 
        
        
        int scor_max = 0;
        char lista_castigatori[256] = ""; 

        
        if(prep_sql(db, "SELECT MAX(Scor) FROM Jucatori;", &res) == SQLITE_OK) {
            if(sqlite3_step(res) == SQLITE_ROW) scor_max = sqlite3_column_int(res, 0);
        }
        sqlite3_finalize(res);

        
        char sql[256];
        sprintf(sql, "SELECT Nume FROM Jucatori WHERE Scor=%d;", scor_max);
        
        int first = 1;
        if(prep_sql(db, sql, &res) == SQLITE_OK) {
            while(sqlite3_step(res) == SQLITE_ROW) {
                if (!first) strcat(lista_castigatori, ", ");
                strcat(lista_castigatori, (char*)sqlite3_column_text(res, 0));
                first = 0;
            }
        }
        sqlite3_finalize(res);
        
        if (strlen(lista_castigatori) == 0) strcpy(lista_castigatori, "Nimeni");

        sprintf(buff, "GAMEOVER|Castigatori: %s (%d puncte)", lista_castigatori, scor_max);
    }
    else if (cine == id) {
        cod = 2; 
        int id_real = get_id_real(db, q_curr);
        
        char sql[256];
        sprintf(sql, "SELECT Text, R1, R2, R3 FROM Intrebari WHERE ID=%d;", id_real);
        if(prep_sql(db, sql, &res) == SQLITE_OK) {
            if(sqlite3_step(res) == SQLITE_ROW) {
                sprintf(buff, "TURN|%s\n\n   [1] %s\n   [2] %s\n   [3] %s", 
                    sqlite3_column_text(res, 0), sqlite3_column_text(res, 1), 
                    sqlite3_column_text(res, 2), sqlite3_column_text(res, 3));
            }
        }
        sqlite3_finalize(res);
    }
    else {
        cod = 1; sprintf(buff, "WAIT");
    }
    sqlite3_close(db);
    return cod;
}

void raspunde(int id, int val, char* msg) {
    sqlite3 *db;
    sqlite3_stmt *res;
    if (sqlite3_open(DB_NAME, &db)) return;
    sqlite3_busy_timeout(db, 5000);

    int q_curr = 1;
    if(prep_sql(db, "SELECT IntrebareCurenta FROM Stare WHERE Cheie=1;", &res) == SQLITE_OK) {
        if(sqlite3_step(res) == SQLITE_ROW) q_curr = sqlite3_column_int(res, 0);
    }
    sqlite3_finalize(res);

    int id_real = get_id_real(db, q_curr);

    int corect = -1;
    char sql[256];
    sprintf(sql, "SELECT Corect FROM Intrebari WHERE ID=%d;", id_real);
    if(prep_sql(db, sql, &res) == SQLITE_OK) {
        if(sqlite3_step(res) == SQLITE_ROW) corect = sqlite3_column_int(res, 0);
    }
    sqlite3_finalize(res);

    if (val == corect) {
        sprintf(msg, "CORECT!");
        sprintf(sql, "UPDATE Jucatori SET Scor = Scor + 10 WHERE ID=%d;", id);
        exec_sql(db, sql);
    } else {
        sprintf(msg, "GRESIT! (Raspuns corect: %d)", corect);
    }
    sqlite3_close(db);

    urmatorul(id);
}

int main(int argc, char* argv[]) {
    struct sockaddr_in serv, from;
    int sd, client, opt = 1;

    if(argc != 2) { printf("Sintaxa: %s <port>\n", argv[0]); return 1; }
    init_bd();

    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) return 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    serv.sin_family = AF_INET; serv.sin_addr.s_addr = htonl(INADDR_ANY); serv.sin_port = htons(atoi(argv[1]));
    if(bind(sd, (struct sockaddr*)&serv, sizeof(serv)) == -1) return 1;
    if(listen(sd, 5) == -1) return 1;

    printf("[Server] Pornit pe portul %d.\n", atoi(argv[1]));

    while(1) {
        socklen_t len = sizeof(from);
        client = accept(sd, (struct sockaddr*) &from, &len);
        if(client < 0) continue;
        
        int pid = fork();
        if(pid == 0) { 
            close(sd);
            char buff[256], rasp[512];
            int r, my_id = -1;

            while((r = read(client, buff, 255)) > 0) {
                buff[r] = 0;
                while(r > 0 && (buff[r-1] == '\n' || buff[r-1] == '\r')) buff[--r] = 0;
                memset(rasp, 0, 512); 

                if(strncmp(buff, "LOGIN", 5) == 0) {
                    auto_reset(); 
                    
                    char nume[50]; sscanf(buff + 6, "%s", nume);
                    my_id = logare(nume);
                    
                    if (my_id == -2) {
                        sprintf(rasp, "ERR: Nume deja folosit!");
                        my_id = -1;
                    } else if (my_id != -1) {
                        sprintf(rasp, "OK: Logat ID %d", my_id);
                    } else sprintf(rasp, "ERR: Baza de date busy");
                }
                else if(strncmp(buff, "START", 5) == 0) {
                    start_joc(); sprintf(rasp, "OK: START");
                }
                else if(strncmp(buff, "CHECK", 5) == 0) {
                    if (my_id == -1) sprintf(rasp, "LOGIN_NEEDED");
                    else vezi_stare(my_id, rasp);
                }
                else if(strncmp(buff, "ANSWER", 6) == 0) {
                    int val = atoi(buff + 7);
                    char msg[100];
                    raspunde(my_id, val, msg);
                    sprintf(rasp, "RESULT|%s", msg); 
                }
                else if(strncmp(buff, "QUIT", 4) == 0) break;
                else sprintf(rasp, "ERR");

                write(client, rasp, strlen(rasp));
            }
            jucator_iesit(my_id);
            close(client);
            exit(0);
        }
        close(client);
        while(waitpid(-1, NULL, WNOHANG) > 0);
    }
}