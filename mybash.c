#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structures.h"


void ls() {
FILE * fin = fopen("simplefs.bin", "r");
struct inode slash;
	fseek(fin, sizeof(struct sb), SEEK_SET);
	fread(&slash,sizeof(slash),1,fin);

	int datablocknum = slash.datablocks[0];
	fseek(fin,sizeof(struct sb)+sizeof(struct inode)*32,SEEK_SET+datablocknum*512);
	struct dir_entry entry;
	fread(&entry,sizeof(entry),1,fin);
	printf("files :\n");
	for ( int i =0;i<DIRENTRYSIZE *3;i=i+32 ){
	fseek(fin,sizeof(struct sb)+sizeof(struct inode)*32,SEEK_SET+datablocknum*512+i);
	fread(&entry,sizeof(entry),1,fin);
	
		if (entry.inode_num == 1){
	fread(&entry,sizeof(slash),1,fin);
	printf("test \n");
	printf("|_ %s\n", entry.name );

	
	}
	else {
	printf(" %s\n",entry.name );
	}
	

	}	
	/*struct inode test;
	fseek(fin, sizeof(struct sb)+sizeof(slash), SEEK_SET);
	fread(&test,sizeof(test),1,fin);
	int datablocknum2 = test.datablocks[0];
	printf("%d= \n",datablocknum2);
	struct dir_entry entry2;
	//for ( int i =48;i<DIRENTRYSIZE * 49;i=i+32 ){
	fseek(fin,sizeof(struct sb)+sizeof(struct inode)*32,SEEK_SET+datablocknum*512+96);
	fread(&entry2,sizeof(entry2),1,fin);
	printf("%d  %s\n", entry2.inode_num,entry2.name );
printf(" -- %d ",i);
}*/




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

