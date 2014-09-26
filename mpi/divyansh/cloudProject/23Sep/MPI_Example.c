#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,char* argv[]){
	int NProcs,ProcRank;
	int NCols,NRows,NScatter;
	int index,index_row,index_col,index_proc;
	int Bcast_root=0;
	char ***Table,*TableBuffer,*ProcessTableBuffer;
	FILE *data_fp;
	int DataFileFlag=1;
	
	
	
		char cwd[1024];
		char *dirsmbl = "/input/";
		char *txtcontent = ".txt";
		char str[1024];
		strcpy (str,cwd);
		strcat (str, dirsmbl);
	        strcat (str, argv[1]);
   	        strcat (str, txtcontent);
		
		if((data_fp=fopen(str,"r"))==NULL)
		{
			DataFileFlag=0;						
		}

		if(DataFileFlag!=0){
		fscanf(data_fp,"%d %d\n",&NRows,&NCols);
		//printf("\n****%d %d",NRows,NCols);
		Table=(char**)malloc(NRows*sizeof(char));
		char line[1028];
		char *data;

		for(index_row=0;index_row<NRows;index_row++){
		Table[index_row]=(char*)malloc(NCols*sizeof(char));
			fgets(line,sizeof line,data_fp);
			data = (char*) strtok(line, ",");
			for(index_col=0;index_col<NCols;index_col++){
				strcpy(Table[index_row][index_col],data);
				//fscanf(data_fp,"%s",&Table[index_row][index_col]);
				
				printf("\t:::%s===",&Table[index_row][index_col]);
				data = (char*)strtok(NULL, ",");
				//printf("\tdata %s",data);
			}printf("\n");	
				
		}
		fclose(data_fp);		
		}

		for(index_row=0;index_row<NRows;index_row++){
		
			fgets(line,sizeof line,data_fp);
			
			for(index_col=0;index_col<NCols;index_col++){
				
				
				printf("\t:::%s===",*Table[index_row][index_col]);
				
			}printf("\n");	
				
		}
		fclose(data_fp);		
		}

}
