/*
Meta-data of all tables of the database is written in a file Input/birt/tablename.txt 
*/
#ifndef databaseDriverDef
	#define databaseDriverDef_1
	#include "databaseDriverDef_1.h"
#endif

char pathOfDatabaseDirectory[100];

int main(int argc, char *argv[]){//argv[1]= Path of database directory
	
	database d=createDatabase();
	char tableFilePath[50];
	string tableName;
	
	strcpy(tableName,"tableName");
	strcpy(pathOfDatabaseDirectory,argv[1]);

	filePathFromTableName(tableFilePath,tableName);
	printf("%s",tableFilePath);
	
	FILE* fp=fopen(tableFilePath,"r");
	if(!fp){
		printf("File Not Found");
		debug();
	}
	int numberOfTables;
	fscanf(fp,"%d", &numberOfTables);
	populateHashTable(fp,numberOfTables,d);
	

	
	do{
		printf("\nInput query, and type ; to quit\n");
		char s[queryLength];
		int ind=0;
		char c;
		do{
			c=getchar();
			s[ind++]=c;
		}while(c!=';');
			
		s[ind++]='\0';
		printf("%s\n",s);
		if(strlen(s)<4)
			break;
		
		query q;
		
		copyQuery(&q,extract(s));
		execute(q,d);
		//display(q);
		/*if(q.type==-1)
			break;
		if(q.type==0){
			select(q);
		}
		else{
			join(q);
		}*/
		do{c=getchar();}while(c!='\n'&&c!='^'); //for enter in the input
		if(c=='^')
			break;
	}while(1);
}