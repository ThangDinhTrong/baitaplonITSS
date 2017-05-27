#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

typedef struct Inventory_t {
	char ten_may[100];
	char ten_hang[100];
	int soluong;
} Inventory;

int generateNumToSeek(int num) {
	int count = 0;
	while(num != 0) {
        num /= 10;
        ++count;
    }
    return count+1;
}

int main(int argc, char const *argv[])
{
	/* code */
	char kitu;
	int c, pid, message_size = 0, number = 0;
	int socket_server, socket_client;
	struct sockaddr_in server,client;
	char *ten_hang, *ten_may, *soluong;
	char message_client[200];
	char thanhcong[] = "success",thatbai[] ="failed";
	char entry[] = "";
	char info[2000];

	socket_server = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);
	bind(socket_server, (struct sockaddr *) &server, sizeof(server));

	listen(socket_server,3);

	while(1) {
		int socket_client = accept(socket_server, (struct sockaddr *)&client, (socklen_t*)&c );
		pid = fork();
		if(pid<0) {
			close(socket_client);
			continue;
		} else if(pid>0){
            close(socket_client);
            // counter++;
            printf("here2\n");
            continue;
        } 
		else {
			if(socket_client < 0) {
				perror("accept failed");
			}
			while(1) {
				int xacnhan = 0;
				FILE *f;
				int num_seek,num_seek_last;
				char xau[1000], may[1000], hang[1000],soluong[1000]="0";

				message_size = recv(socket_client, message_client, 32, 0);
				ten_may = strtok(message_client,";");
				ten_hang = strtok(NULL,"\n");
				// printf("%s %s\n", ten_may, ten_hang);

				f = fopen("inventory.txt","r+");
				while(!feof(f)) {
					fgets(xau, 1000, f);
					if(feof(f)) {
						break;
					}
					// printf("%s\n", xau);
					char *sysmay = strtok(xau,";");
					char *syshang = strtok(NULL,";");
					char *sysnum = strtok(NULL,"");
					// printf("May: %s\n", sysmay);
					// printf("Hang: %s\n", syshang );
					// printf("So luong %s\n",sysnum );
					if(strcmp(ten_may,sysmay) == 0 && strcmp(ten_hang,syshang)==0) {
						int num = atoi(sysnum);
						char currnum[1000];
						--num;

						//chuyen hang den may
						if(num<3) {
							char client_message[1000];
							printf("Delivery....\n");
                            sprintf(client_message,"increase 10 %s in %s\n",syshang,sysmay);
                            send(socket_client,client_message,strlen(client_message),0);
                            sleep(5);

                            num+=10;
							strcpy(soluong,"");

                            printf("Done!......\n");
						}
						if(num>=10) {
							strcpy(soluong,"");
						}
						sprintf(currnum,"%d",num);
						strcat(soluong,currnum);
						fseek(f,-3,SEEK_CUR);
						fputs(soluong,f);


						xacnhan = 1;
						write(socket_client, thanhcong, strlen(thanhcong)+1);
						
						//Ghi vao file history
						printf("Ban %s o may %s, con lai %s\n",syshang, sysmay, soluong );
						FILE *history;
						time_t t = time(NULL);
    					struct tm tm;
						history = fopen("history.txt","a");
						tm = *localtime(&t);
                        fprintf(history,"%s\t%s\t%d-%d-%d %d:%d:%d\n",sysmay,syshang,  tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
						fclose(history);
						printf("Da in xong\n");
						break;
					}
					printf("\n\n\n\n");
				}
				fclose(f);
				if(xacnhan == 0) {
					write(socket_client,thatbai,strlen(thatbai)+1);
					break;
				}
			}
			close(socket_client);
		}
	}

	return 0;
}