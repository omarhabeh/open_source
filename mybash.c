#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structures.h"


void ls() {
FILE * fin = fopen("simplefs.bin", "r");
struct inode slash;
	fseek(fin, sizeof(struct sb), SEEK_SET);
	fread(&slash,sizeof(slash),1,fin);
	struct dir_entry entry ;
	int datablocknum = slash.datablocks[0];
	printf("files :\n");
	for ( int i =0;i<DIRENTRYSIZE *4;i=i+32 ){
	fseek(fin,sizeof(struct sb)+sizeof(struct inode)*32,SEEK_SET+i);//32++
	fread(&entry,sizeof(entry),1,fin);
	printf(" %s\n",entry.name );
	}
//+datablocknum*512
	
	struct inode test;
	fseek(fin, sizeof(struct sb)+sizeof(slash), SEEK_SET);
	fread(&test,sizeof(test),1,fin);
	int datablocknum2 = test.datablocks[0];
	printf("%d= \n",datablocknum2);
	struct dir_entry entry2;
	
	for ( int i =0;i<DIRENTRYSIZE *4;i=i+32 ){
	fseek(fin,sizeof(struct sb)+sizeof(struct inode)*32,SEEK_SET+512+i);//512++
	fread(&entry2,sizeof(entry2),1,fin);
	printf("%s\n", entry2.name );
}




}

int main() {
char command[128];
	while(1){
	printf("prompt>");
	scanf("%s",command);
	if(strcmp(command,"ls")==0) {
	ls();
		}
if(strcmp(command,"check")==0) {
printf("you here ");	

	
		}


}

}

