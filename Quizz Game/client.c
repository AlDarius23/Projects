#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h> 

int recv_msg(int sd, char* buff) {
    memset(buff, 0, 512); 
    int n = read(sd, buff, 511);
    if(n > 0) buff[n] = 0;
    return n;
}

int main(int argc, char* argv[]) {
    int sd;
    struct sockaddr_in server;
    char buffer[512], raspuns[512];
    char msg[256]; 

    if(argc != 3) { printf("Sintaxa: %s IP PORT\n", argv[0]); return 1; }
if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) return 1;
    server.sin_family = AF_INET; 
server.sin_addr.s_addr = inet_addr(argv[1]);
server.sin_port = htons(atoi(argv[2]));
    if(connect(sd, (struct sockaddr*)&server, sizeof(server)) == -1) return 1;

    system("clear");
    printf("\n=== QUIZ GAME ===\n\n");
    
    while(1) {
        printf("Introdu Numele: ");
        fgets(buffer, 255, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        if (strlen(buffer) == 0) continue; 

        sprintf(msg, "LOGIN %s", buffer);
        write(sd, msg, strlen(msg));
        
        recv_msg(sd, raspuns);
        
        if (strncmp(raspuns, "OK:", 3) == 0) {
            break; 
        } else {
            printf(">> %s (Incearca alt nume)\n\n", raspuns);
        }
    }
    
    system("clear");
    printf("\nSalut %s! Te-ai conectat.\n", buffer);
    printf("Asteptam in Lobby... (Scrie 'START' daca vrei sa incepi)\n");

    
    while(1) {
        write(sd, "CHECK", 5);
        if(recv_msg(sd, raspuns) <= 0) break;

        if(strncmp(raspuns, "LOBBY", 5) == 0) {
            printf("\r[LOBBY] Asteptare... (Scrie START)     ");
            fflush(stdout);
            
            fd_set set; struct timeval timeout;
            FD_ZERO(&set); FD_SET(0, &set);
            timeout.tv_sec = 1; timeout.tv_usec = 0;

            if(select(1, &set, NULL, NULL, &timeout) > 0) {
                memset(buffer, 0, 512);
                fgets(buffer, 255, stdin);
                if(strncmp(buffer, "START", 5) == 0) {
                    write(sd, "START", 5);
                    recv_msg(sd, raspuns); 
                }
            }
        }
        else if(strncmp(raspuns, "WAIT", 4) == 0) {
            printf("\r[JOC] Asteapta-ti randul       ");
            fflush(stdout);
            sleep(1);
        }
        else if(strncmp(raspuns, "TURN", 4) == 0) {
            system("clear");
            printf("\n#############################################\n");
            printf("           E RANDUL TAU!            \n");
            printf("#############################################\n\n");
            printf("%s\n\n", strchr(raspuns, '|') + 1);
            printf("#############################################\n");
            printf("Raspunsul tau (1/2/3): ");
            fflush(stdout);

            fd_set set; struct timeval timeout;
            FD_ZERO(&set); FD_SET(0, &set);
            timeout.tv_sec = 15; timeout.tv_usec = 0; 

            int val = 0;
            if(select(1, &set, NULL, NULL, &timeout) == 0) {
                printf("\n\n[!] TIMP EXPIRAT! [!]\n");
                val = 0; 
            } else {
                memset(buffer, 0, 512);
                fgets(buffer, 255, stdin);
                val = atoi(buffer);
            }

            
            sprintf(msg, "ANSWER %d", val);
            write(sd, msg, strlen(msg));
            recv_msg(sd, raspuns);
            
            printf("\n\n");
            printf("#############################################\n");
            printf(" %s\n", strchr(raspuns, '|') + 1); 
            printf("#############################################\n");
            
            printf("\n(Ai 5 secunde sa vezi raspunsul...)\n");
            sleep(5); 
            system("clear");
        }
        
        else if(strncmp(raspuns, "GAMEOVER", 8) == 0) {
            system("clear");
            printf("\n#############################################\n");
            printf("           JOC TERMINAT           \n");
            printf("#############################################\n\n");
            printf("%s\n\n", strchr(raspuns, '|') + 1);
            break;
        }
    }
    close(sd);
    return 0;
}