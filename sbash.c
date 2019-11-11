#include <stdio.h>
#include "structures.h"
#include <string.h>

void ls(FILE *file);
void mkdir(FILE *file,char command[]);
void get_inode_struct(FILE *f, struct inode *inode,int inode_num);
void new_dir(FILE *fin);
int main(){
	FILE *fin = fopen("simplefs.bin", "r+");

	char command[32];
	char file[32];
	while(1){
		printf("> ");
		scanf("%s",command);
		if(strcmp(command,"ls") == 0){
			ls(fin);}
		else if(strcmp(command,"mkdir") == 0){
			printf("enter file name ");
			scanf("%s",file);
			mkdir(fin,file);}
	}
}
void get_inode_struct(FILE *fin, struct inode *inostr, int inode_num){
 
	fseek(fin,sizeof(struct sb),SEEK_SET);
	fseek(fin,sizeof(struct inode)*inode_num, SEEK_CUR);
	fread(inostr,sizeof(struct inode),1,fin);	
}

void get_dir_entry(FILE *fin,struct dir_entry* de,int db_num,int dir_entry_num){
	fseek(fin,sizeof(struct sb)+sizeof(struct inode)*32+512*db_num+dir_entry_num*sizeof(struct dir_entry),SEEK_SET);
	fread(de,sizeof(struct dir_entry),1,fin);
	
}
void ls(FILE *fin){
	struct inode inostr;
	struct dir_entry de;
	get_inode_struct(fin,&inostr,0);
	printf("/\n");
	int i;
	for(i = 0; i <inostr.size/32; i++){
		get_dir_entry(fin,&de,0,i);
		printf("\t%s\n",de.name);
	}	
}
void new_dir(FILE *fin,int no ) { 
	printf("%d",no);
	struct dir_entry dot;
	strcpy(dot.name, ".");
	dot.inode_num = 0;

	struct dir_entry dotdot;
	strcpy(dotdot.name, "..");
	dotdot.inode_num = 0;
	
	fseek(fin,sizeof(struct sb)+NUMOFINODES*sizeof(struct inode),SEEK_SET+96);
	fwrite(&dot,sizeof(dot),1,fin);
	fwrite(&dotdot,sizeof(dotdot),1,fin);
	fflush(fin);
		
}
void mkdir(FILE *fin,char command[]){
	struct inode inostr;
	int no=1;
	struct dir_entry de;
	strcpy(de.name, command);
	de.inode_num = 2;
	get_inode_struct(fin,&inostr,0);
	if (fread(de,sizeof(struct dir_entry),1,fin)==0){
	
}
	fwrite(&de,sizeof(de),1,fin);
	fflush(fin);
}
