#include <mpi.h>
#ifndef MPIDEF
	#define MPIDEF
	#include"mpiDef.h"
#endif
#ifndef databaseDriverDef
	#define databaseDriverDef
	#include "databaseDriverDef.h"
#endif
void master_function(int *numberOfTables, database *d){
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
	
	tFp[0]=ftell(fp);
	for(int i=1; i<P; i++){
	
		for(int j=0; j<(n)/P; j++){
			char c;	
			do{
				c=fgetc(fp);
			}while(c!='\n');
		}
		tFp[i]=ftell(fp);
		//printf("%x",*fp);
	}
	/*for(int i=0; i<P; i++){
		printf("%d\n",tFp[i]);
	}
	for(int i=0; i<P-1; i++){
		printf("%d\n",tFp[i+1]-tFp[i]);
	}*/
}
int main(int argc, char *argv[]){//argv[1]= Path of database directory
	MPI_Init(NULL, NULL);
	int world_rank;
	int numberOfTables;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	database d;
	if(world_rank==0){
			master_function(&numberOfTables,&d);//populatesAdvAdjList from input 
	}
	//MPI_Scatter(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data, int recv_count, MPI_Datatype recv_datatype,int root, MPI_Comm communicator)
	
	MPI_Scatter(tFp, 1, MPI_INT, &localTFp, 1, MPI_INT, 0, MPI_COMM_WORLD);

	
	//debug();
	char tableFilePath[50];
	string tableName;
	
	strcpy(tableName,"tableName");
	
	filePathFromTableName(tableFilePath,tableName);
	//printf("%s",tableFilePath);
	
	FILE* fp=fopen(tableFilePath,"r");
	//if(world_size==world_rank-1)
		//populateHashTable(fp+localTFp,(numberOfTables/P)+(numberOfTables%P),d);
	if(!fp)
		printf("File not found");
	else{
		char c;
		fseek ( fp ,localTFp  , SEEK_SET );
		c=fgetc(fp);
		printf("World: %d %d %d %c\n",world_size, world_rank,localTFp,c);
		//printf("%c",c);
	}
	//else{
	//	populateHashTable(fp+localTFp,(numberOfTables/P),d);
	//}
	
	/*do{
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
		
		do{c=getchar();}while(c!='\n'&&c!='^'); //for enter in the input
		if(c=='^')
			break;
	}while(1);*/
	MPI_Finalize();
}