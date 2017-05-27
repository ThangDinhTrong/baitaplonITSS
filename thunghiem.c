#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int generateNumToSeek(int num) {
	int count = 0;
	while(num != 0) {
        num /= 10;
        ++count;
    }
    return count+1;
}
int main() {
	FILE *f;
	int num_seek,num_seek_last;
	char xau[1000], may[1000], hang[1000],soluong[1000];
	printf("nhap ten may:\n");scanf("%s",may);
	printf("nhap ten hang\n");scanf("%s",hang);
	printf("may: %s; hang:%s\n\n\n",may,hang );
	f = fopen("inventory.txt","r+");
	while(!feof(f)) {
		fgets(xau, 1000, f);
		if(feof(f)) {
			break;
		}
		printf("%s\n", xau);
		char *sysmay = strtok(xau,";");
		char *syshang = strtok(NULL,";");
		char *sysnum = strtok(NULL,"");
		printf("May: %s\n", sysmay);
		printf("Hang: %s\n", syshang );
		printf("So luong %s\n",sysnum );
		if(strcmp(may,sysmay) == 0 && strcmp(hang,syshang)==0) {
			int num = atoi(sysnum);
			printf("FOUND %d\n", --num );
			num_seek = generateNumToSeek(num);
			num_seek_last = generateNumToSeek(num+1);
			sprintf(soluong,"%d",num);
			if(num_seek_last>num_seek) {
				strcat(soluong," ");
			}
			printf("size of num: %d\n",sizeof(num) );
			fseek(f,-num_seek_last,SEEK_CUR);
			fputs(soluong,f);

		}
		printf("\n\n\n\n");
	}
}