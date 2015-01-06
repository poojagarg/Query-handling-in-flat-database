#include <mpi.h>
#ifndef MPIDEF
	#define MPIDEF
	#include"mpiDef.h"
#endif
#ifndef databaseDriverDef
	#define databaseDriverDef
	#include "databaseDriverDef.h"
#endif
char pathOfDatabaseDirectory[100];
void master_function(int *numberOfTables, database *d, int world_rank, int world_size){
	*d=createDatabase();
	char tableFilePath[50];
	string tableName;
	
	strcpy(tableName,"tableName");
	
	filePathFromTableName(tableFilePath,tableName);
	//printf("%s",tableFilePath);
	
	FILE* fp=fopen(tableFilePath,"r");
	if(!fp){
		printf("File Not Found");
		return;
	}
	int n;
	fscanf(fp,"%d\n", &n);
	*numberOfTables=n;
	populateHashTable(fp, n,d, world_rank, world_size);
}
int main(int argc, char *argv[]){//argv[1]= Path of database directory
	MPI_Init(NULL, NULL);
	int world_rank;
	int numberOfTables;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	database d;
	master_function(&numberOfTables,&d,world_rank, world_size);
	//viewDatabase(d);
	char c;
	strcpy(pathOfDatabaseDirectory,argv[1]);
	FILE* fp=fopen("query.txt","r");

	do{
		char s[queryLength];
		int ind=0;
		
		int count=0;
		do{
			count++;
			c=fgetc(fp);
			s[ind++]=c;
			//printf("%c",c);
			
		}while(c!=';');
		
		
		s[ind]='\0';
		printf("%s %d %d\n",s, count, world_rank);
		MPI_Barrier(MPI_COMM_WORLD);
		
		if(count<4){
			//printf("I will break");
			break;
		}
		
		query q;
		
		copyQuery(&q,extract(s));
		/*	if(world_rank==0){
				printf("I am %d\n",world_rank);*/
		execute(q,d, world_rank);
			/*}*/
		//display(q);
		
		/*do{c=getchar();
			printf("%c",c);
		}while(c!='\n'&&c!='^'); //for enter in the input
		if(c=='^'){
			printf("^");
			break;
		}
		else
			printf("Easy");*/
		c=fgetc(fp);
		
		if(c=='^'){
			
			MPI_Finalize();
			exit(0);
			
			}	
	}while(c!='^');
	//printf("I am done : %d", world_rank);
	MPI_Finalize();
}