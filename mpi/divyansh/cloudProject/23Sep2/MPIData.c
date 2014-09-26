#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,char* argv[]){
	int NProcs,ProcRank;
	int NCols,NRows,NScatter;
	int index,index_row,index_col,index_proc;
	int Bcast_root=0;
	int **Table,*TableBuffer,*ProcessTableBuffer;
	FILE *data_fp;
	int DataFileFlag=1;
	
	//MPI Initialize
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&ProcRank);
	MPI_Comm_size(MPI_COMM_WORLD,&NProcs);
	
	if(ProcRank==0){
		
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		//int index_col=(int*)malloc(sizeof(int)*argc);
		char *dirsmbl = "/input/";
		char *txtcontent = ".txt";
		char str[1024];
		strcpy (str,cwd);
		strcat (str,dirsmbl);
	        strcat (str,argv[1]);
   	        strcat (str,txtcontent);
		
		if((data_fp=fopen(str,"r"))==NULL)
		{
			DataFileFlag=0;						
		}

		if(DataFileFlag!=0){
		fscanf(data_fp,"%d %d\n",&NRows,&NCols);
		//printf("\n****%d %d",NRows,NCols);
		Table=(int**)malloc(NRows*sizeof(int*));
		char line[1028];
		char *data;

		for(index_row=0;index_row<NRows;index_row++){
		Table[index_row]=(int*)malloc(NCols*sizeof(int));
			fgets(line,sizeof line,data_fp);
			data = (char*) strtok(line, ",");
			for(index_col=0;index_col<NCols;index_col++){
				Table[index_row][index_col]=atoi(data);
				//fscanf(data_fp,"%s",&Table[index_row][index_col]);
				
				
				printf("\t:::%d===%s",Table[index_row][index_col-1],data);
				data = (char*)strtok(NULL, ",");
				
			}printf("\n");	
				
		}
		printf("\n...1....");
		fclose(data_fp);
		perror("\n....1a...\n");		
		}
		printf("\n...2....");
		TableBuffer=(int*)malloc(NRows*NCols*sizeof(int));
		index=0;
		printf("\n....3....");
		for(index_row=0;index_row<NRows;index_row++){
			for(index_col=0;index_col<NCols;index_col++){
			TableBuffer[index]=Table[index_row][index_col];
			printf("+++++:::%d",TableBuffer[index]);
			index++;
		}
		printf("\n");
		}
		printf("\n...2....");
		}//end if
	
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Bcast(&DataFileFlag,1,MPI_INT,Bcast_root,MPI_COMM_WORLD);
		if(DataFileFlag==0){
			if(ProcRank==Bcast_root){
			printf("Table Not Found...\n");					
			}
		MPI_Finalize();
		exit(-1);		
		}

		MPI_Bcast(&NRows,1,MPI_INT,Bcast_root,MPI_COMM_WORLD);
		if(NRows<NProcs){
		if(ProcRank==0)	{		
		printf("No. of Rows should be greater than No. of Processors");
		}
		MPI_Finalize();
		exit(0);
		}		
		MPI_Bcast(&NCols,1,MPI_INT,Bcast_root,MPI_COMM_WORLD);
		
		NScatter=NRows/NProcs;
		ProcessTableBuffer=(int*) malloc(sizeof(int)*NRows*NCols);
		MPI_Scatter(TableBuffer,NScatter * NCols,MPI_INT,ProcessTableBuffer,NScatter*NCols,MPI_INT,0,MPI_COMM_WORLD);

		for(index_row=0;index_row<NScatter;index_row++){			
		index=index_row*NCols;
			for(index_col=0;index_col<NCols;index_col++){			
			printf("\nRank=%d, Content=%d\n",ProcRank,ProcessTableBuffer[index++]);			
			}	
		}

	MPI_Finalize();
}
