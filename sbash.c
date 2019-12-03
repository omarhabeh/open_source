
#include <stdio.h>
#include "structures.h"
#include <string.h>

void ls(FILE *file);
void mkdir(FILE *file,char command[]);
void get_inode_struct(FILE *f, struct inode *inode,int inode_num);
void cd (FILE *fin , char command[]);
void write_dir_entry(FILE *fin,struct dir_entry* de,int db_num,int dir_entry_num);
void ifDIr(FILE *fin,char command[]);
void mkfile(FILE *fin,char f[],char s[]);
void write_string(FILE *fin , char str[],int blokno);
void cat (FILE *fin , char file[]);
void ifFile(FILE *fin,char command[],char ans[128]);
void addIno(FILE *fin);
int c=1;
int size=1;
char current[]="/";
int ino=2;
int main(){
	FILE *fin = fopen("simplefs.bin", "r+");

	char command[32];
	char file[32];
	char string[128];
	while(1){
		if(strcmp(current,"/")!=0){
		printf("%s> ",current);}
		else {printf("> ");}
		scanf("%s",command);
		if(strcmp(command,"ls") == 0){
			ls(fin);}
		else if(strcmp(command,"mkdir") == 0){
			scanf("%s",file);
			mkdir(fin,file);}
		else if (strcmp(command,"cd")==0){
			scanf("%s",file);
			cd(fin,file);}
		else if (strcmp(command,"mkfile")==0){
			scanf("%s",file);
			fgets(string,128,stdin);
			mkfile(fin,file,string);
		}
		else if (strcmp(command,"cat")==0){
			scanf("%s",file);
			cat(fin,file);
		}
		
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
void write_dir_entry(FILE *fin,struct dir_entry* de,int db_num,int dir_entry_num){
	fseek(fin,sizeof(struct sb)+sizeof(struct inode)*32+512*db_num+dir_entry_num*sizeof(struct dir_entry),SEEK_SET);
		fwrite(de,sizeof(struct dir_entry),1,fin);
	fflush(fin);
	
}
void ifDIr(FILE *fin,char command[]){
		
	
		int la=0;
		struct dir_entry de;
		for (int i=0;i<size;i++){
		get_dir_entry(fin,&de,1,i);
		if(strcmp(command,de.name)==0){
		la++;
			if(de.inode_num>=2){
				if(strcmp(current,"/")!=0){
					strcat(current,(" %s> ",command));
				}
				else{
				strcpy(current,("\\/%s\\/",command));	
				}			
}
			else {
			printf("%s is not a drectory\n",command);
			break;
			}
		break;
		}
			
	}
if(la==0){printf("%s file is not found \n",command);}				
}
void ifFile(FILE *fin,char command[],char ans[128]){
		int la=0;
		struct dir_entry de;
		for (int i=0;i<size;i++){
		get_dir_entry(fin,&de,1,i);
		if(strcmp(command,de.name)==0){
		la++;
			if(de.inode_num==1){
			fseek(fin,sizeof(struct sb)+sizeof(struct inode)*32+512*2+i*sizeof(struct dir_entry),SEEK_SET);
			fread(ans,128,1,fin);
			}
			else {
			printf("%s is a directory",command);
			break;
			}
		break;
		}
			
	}
if(la==0){printf("%s file is not found ",command);}				
}
void ls(FILE *fin){
	struct inode inostr;
	struct dir_entry de;
	get_inode_struct(fin,&inostr,0);
	if(strcmp(current,"/")==0){
	printf("%s\n",current);
			for ( int j =0 ; j <=1;j++){
				get_dir_entry(fin,&de,0,j);
				printf("\t%s\n ",de.name);
			}
			for(int i = 0; i <=size-1; i++){
				get_dir_entry(fin,&de,1,i);
				if(strcmp(de.name,"\0")!=0){
				printf("\t%s\n ",de.name);
				}		
			}
	}
	else{
	for (int i=0;i<size-1;i++){
		get_dir_entry(fin,&de,1,i);
			if(de.inode_num>=2){
				int c=de.inode_num;
				if(c!=ino){c=ino;};
				for (int j =1;j<=c;j++){
				get_dir_entry(fin,&de,1,i+j);
				printf("%s \n",de.name);
				}	
			}
	}			
		
	}
}
void mkdir(FILE *fin,char command[]){
	fseek(fin,sizeof(struct sb),SEEK_SET);
	struct inode inostr;
	inostr.datablocks[1]=1;
	struct dir_entry de;
	strcpy(de.name, command);
	de.inode_num = 2;
	struct dir_entry dot;
	strcpy(dot.name, ".");
	dot.inode_num = 0;
	struct dir_entry dotdot;
	strcpy(dotdot.name, "..");
	dotdot.inode_num = 0;
	write_dir_entry(fin,&de,1,c);c++;
	write_dir_entry(fin,&dot,1,c);c++;
	write_dir_entry(fin,&dotdot,1,c);c++;
	size+=3;
	if(strcmp(current,"/")!=0){
	addIno(fin);}
}
void cd (FILE *fin , char command[]){
	if(strcmp(command,"..")==0){
		strcpy(current,"/");

	}
	else if (strcmp(command,".")==0){
	}
	else{
	ifDIr(fin,command);	
	}
}
void mkfile(FILE *fin,char f[],char s[]){
	fseek(fin,sizeof(struct sb),SEEK_SET);
	struct inode inostr;
	inostr.datablocks[2]=1;
	struct dir_entry de;
	strcpy(de.name, f);
	de.inode_num = 1;
	write_dir_entry(fin,&de,1,c);
	size++;
	write_string(fin,s,c);c++;
	if(strcmp(current,"/")!=0){
	addIno(fin);}
}
void write_string(FILE *fin , char str[],int blokno){
fseek(fin,sizeof(struct sb)+sizeof(struct inode)*32+512*2+blokno*sizeof(struct dir_entry),SEEK_SET);
fwrite(str,128,1,fin);
fflush(fin);
}
void addIno(FILE *fin){
		struct dir_entry de;
		for (int i=0;i<size;i++){
		get_dir_entry(fin,&de,1,i);
		if(strcmp(current,de.name)==0){
			de.inode_num++;
			ino++;
			
			
		}
			
	}

}
void cat (FILE *fin , char file[]){
	char str[128];
	ifFile(fin,file,str);
	printf("%s",str);
}
