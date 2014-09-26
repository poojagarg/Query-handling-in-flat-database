#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<string.h>

void getTableColName(char *line, int *cl,int *n, int *cd, int *rn, char **column,char **tables, char **condcol, char *operator, int *condval, int *ro);
void selIndex(char *sel_col[], char *collist[], int *sel_index, int cl, int tc);
int eval(int lhs, int rhs, char op);

int main(int argc,char* argv[]){
	int NProcs,ProcRank;
	int NCols,NRows,NScatter,Nq;
	int index,index_row,index_col,index_proc;
	int Bcast_root=0;
	int **Table,*TableBuffer,*ProcessTableBuffer;
	FILE *data_fp,*query_fp;
	int DataFileFlag=1;
	char *col[1000],*table[1000],*condcol[1000],operator[1000];
	int cl,tl,cn,i=0,j,rn;
	int *sel_index, *condindex, condval[1000],ro[1000],condmatch,ptr;
	
	char **colnames;
	char cwd[1024];
	char *dirsmbl = "/input/";
	char *txtcontent = ".txt";
	char str[1024];
	char *query;
	char line[1028],line1[1024];

	//MPI Initialize
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&ProcRank);
	MPI_Comm_size(MPI_COMM_WORLD,&NProcs);

	getcwd(cwd, sizeof(cwd));
		//int index_col=(int*)malloc(sizeof(int)*argc);
	
	if(ProcRank==0)
	{	
		strcpy (str,cwd);
		strcat (str,dirsmbl);
	        strcat (str,argv[1]);
   	        strcat (str,txtcontent);
		printf("\n%s*****",str);
		if((query_fp=fopen(str,"r"))==NULL)
		{
			DataFileFlag=0;						
		}
		
	}


		if(ProcRank==0&&DataFileFlag!=0){
		
			fgets(line1,sizeof line1,query_fp);
			query=line1;
			printf("\n%s....",line1);
			printf("\n....");
			getTableColName(line1,&cl,&tl,&cn,&rn,col,table,condcol,operator,condval,ro);
		
		
		str[0]="\0";
		strcpy (str,cwd);
		strcat (str,dirsmbl);
	        strcat (str,table[0]);
   	        strcat (str,txtcontent);
		printf("\npath:::: %s",str);
		if((data_fp=fopen(str,"r"))==NULL)
		{
			DataFileFlag=0;						
		}

		if(DataFileFlag!=0){
		fscanf(data_fp,"%d %d\n",&NRows,&NCols);
		colnames=(char**)malloc(NCols*sizeof(char*));
		//printf("\n****%d %d",NRows,NCols);

		char *data;
		
		//extracting and storing colnames
		fgets(line,sizeof line,data_fp);
		
		char *pos;
		if((pos=strchr(line,'\n'))!=NULL)
		{
			*pos='\0';
		}
		data = (char*) strtok(line, ",");
		for(index_col=0;index_col<NCols;index_col++)
		{	
			colnames[index_col]=(char*)malloc(strlen(data)+1);
			colnames[index_col]=(char*)data;
			//printf("\ndata  %s----",data);
			data = (char*)strtok(NULL, ",");
		}

		
		sel_index=(int*)malloc(cl*sizeof(int));
		selIndex(col,colnames, sel_index, cl, NCols);
		
		//extract condition col, operator and values
		printf("\n######################################################### %d",cn);
		//condcol=(char**)malloc(cn*sizeof(char*));
		//operator=(char**)malloc(cn*sizeof(char*));
		//condval=(int*)malloc(cn*sizeof(int));
		

		
			
		condindex=(int*)malloc(cn*sizeof(int));
		selIndex(condcol,colnames, condindex, cn, NCols);
		for(i=0;i<cn;i++)
		{
			printf("\n********-%d",condindex[i]);
		}
		
	
		//putting data into table
		Table=(int**)malloc(NRows*sizeof(int*));
		printf("\n");
		for(index_row=0;index_row<NRows;index_row++){
		Table[index_row]=(int*)malloc(NCols*sizeof(int));
			fgets(line,sizeof line,data_fp);
			data = (char*) strtok(line, ",");
			for(index_col=0;index_col<NCols;index_col++){
				Table[index_row][index_col]=atoi(data);
				//fscanf(data_fp,"%s",&Table[index_row][index_col]);
				
				
				//printf("\t:::%d===%s",Table[index_row][index_col-1],data);
				data = (char*)strtok(NULL, ",");
				
			}printf("\n");	
				
		}
		//printf("\n...1....");
		fclose(data_fp);
		//perror("\n....1a...\n");		
		}
		//printf("\n...2....");
		TableBuffer=(int*)malloc(NRows*NCols*sizeof(int));
		index=0;
		//printf("\n....3....");
		for(index_row=0;index_row<NRows;index_row++){
			for(index_col=0;index_col<NCols;index_col++){
			TableBuffer[index]=Table[index_row][index_col];
			printf("+++++:::%d",TableBuffer[index]);
			index++;
		}
		printf("\n");
		}
		//printf("\n...2....");
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
		printf("No. of Rows should be greater than No. of Processors %d %d",NRows,NProcs);
		}
		MPI_Finalize();
		exit(0);
		}		
		MPI_Bcast(&NCols,1,MPI_INT,Bcast_root,MPI_COMM_WORLD);
		MPI_Bcast(&cl,1,MPI_INT,Bcast_root,MPI_COMM_WORLD);
		MPI_Bcast(&cn,1,MPI_INT,Bcast_root,MPI_COMM_WORLD);
		MPI_Bcast(&rn,1,MPI_INT,Bcast_root,MPI_COMM_WORLD);
		if(ProcRank != 0) 
     		{
			sel_index = (int *)malloc(cl*sizeof(int));
			condindex=(int*)malloc(cn*sizeof(int));
			//condval=(int*)malloc(cn*sizeof(int));
			//operator=(char**)malloc(cn*sizeof(char*));

		}
		MPI_Bcast(sel_index,cl,MPI_INT,Bcast_root,MPI_COMM_WORLD);
		MPI_Bcast(condindex,cn,MPI_INT,Bcast_root,MPI_COMM_WORLD);
		MPI_Bcast(condval,cn,MPI_INT,Bcast_root,MPI_COMM_WORLD);
		MPI_Bcast(operator,cn,MPI_CHAR,Bcast_root,MPI_COMM_WORLD);
		MPI_Bcast(ro,rn,MPI_INT,Bcast_root,MPI_COMM_WORLD);
		
		NScatter=NRows/NProcs;
		ProcessTableBuffer=(int*) malloc(sizeof(int)*NRows*NCols);
		MPI_Scatter(TableBuffer,NScatter * NCols,MPI_INT,ProcessTableBuffer,NScatter*NCols,MPI_INT,0,MPI_COMM_WORLD);
		
		
		
		for(index_row=0;index_row<NScatter;index_row++){
			condmatch=1;			
			index=index_row*NCols;
			int cond0=0,curr=0;
			//printf("\n((((((((((((((((((  %d %d",ro[0],ProcRank);
			//match conditions
			
				if(cn==1)
				{
					if(!(eval(ProcessTableBuffer[index+condindex[i]],condval[i],operator[i])))
					{	condmatch=0;
						
						break;
						
					}
				}				
				
				
				if(cn!=1)
				{
					for(i=0;i<cn;i++)
					{
						if(i==0)
						{
							cond0=(eval(ProcessTableBuffer[index+condindex[i]],condval[i],operator[i]));
						}
						else
						{
							curr=(eval(ProcessTableBuffer[index+condindex[i]],condval[i],operator[i]));
							printf("\nProcessTableBuffer[index] = %d cond0 = %d curr = %d",ProcessTableBuffer[index],cond0,curr);
							if(ro[i-1]==0)
							{
								if(!(curr && cond0))
								{
									condmatch=0;
									break;
								}
							}
							else if(ro[i-1]==1)
							{
								if(!(curr || cond0))
								{
									condmatch=0;
									break;
								}
							}
							cond0=curr;
							
						}
					
					
					}
				}
				
		
			if(condmatch==1)
			{
				//printf("\n$$$$$$$$$$$$$$$$$$$$$");
				for(i=0;i<cl;i++)
				{	
						
					printf("\nRank=%d, Content=%d\n",ProcRank,ProcessTableBuffer[index+sel_index[i]]);
				}
			}
			index++;
		}
	printf("\n");
	
	
	
MPI_Finalize();
}

int evalRO(int lhs, int rhs, int op)
{
	printf("@@@@@@ %d %d %d",lhs,rhs,op);
	switch(op)
	{
		
		case 0 : if(lhs&&rhs)
				return 1;
			break;
		case 1 : if(lhs || rhs)
			{
				printf("\nOR");
				return 1;
			}
			break;
		default: return 0;
	
	}
	return 0;
}

int eval(int lhs, int rhs, char op)
{
	
	switch(op)
	{
		case '=' : if(lhs==rhs)
				return 1;
			break;
		case '<' : if(lhs<rhs)
				return 1;
			break;
		case '>' : if(lhs>rhs)
				return 1;
			break;
		case '!' : if(lhs!=rhs)
				return 1;
			break;
		default: return 0;
	
	}
	return 0;
}

void selIndex(char *sel_col[], char *collist[], int sel_index[], int cl, int tc)
{
	int i=0,j=0;
	//printf("\n---in func---%d",cl);
	//printf("&&&&&&&&&&&&");
		
	
	for(i=0;i<cl;i++)
	{
		for(j=0;j<tc;j++)
		{
			//printf("\nsel_col[i] =%s collist[j] %s  ---%d----",sel_col[i],collist[j],(strcmp(sel_col[i],collist[j])));
			if(!(strcmp(sel_col[i],collist[j])))
			{	
				sel_index[i]=j;
				//printf("\n**func*377777***%d %d %d",sel_index[i],i,j);
				break;
			}
			
		}
		//printf("\n**func*333333***%d %d %d",sel_index[i],i,j);
	}

	for(i=0;i<cl;i++)
		printf("\n**func****%d",sel_index[i]);

}

void getTableColName(char *line, int *cl,int *n, int *cd, int *rn, char **column,char **tables, char **condcol, char *operator, int *condval, int *ro)
{
		char *op;
		char from[4],where[5],select[6];
		int flagS,flagF,flagW,flagL=0,flagM=0,flagR=0,flagRO=0;
		//char *cond[1000];
		flagS=0;
		flagW=0;
		flagF=0;

		*cd=0;
		*n=0;
		*cl=0;
		*rn=0;
		int i=0;
		//printf("\nssssssssssssssss");
		
			op=strtok(line," ,;");
			while(op)
			{
				printf("\n----while %s",op);
				
				if(flagR==1)
				{
					
					//flagW=1;
					flagR=0;
					flagRO=1;
					condval[*cd]=atoi(op);
					printf("\n::::val %d  op %c l %s ro=%d",condval[*cd],operator[*cd],condcol[*cd],ro[*rn-1]);
					(*cd)++;

				}
				
				if(flagM==1)
				{
					printf("\n---%s",op);
					flagM=0;
					flagR=1;
					//operator[*cd]=(char*)malloc(sizeof(char));
					operator[*cd]=*op;

				}

    				if(flagW==1)
				{
					printf("\n 1---***%s",op);
					condcol[*cd]=(char*)malloc(strlen(op)+1);
					condcol[*cd]=(char*)op;
					flagM=1;
					flagW=0;

					
				}
	
				if((!strcmp(op,"AND"))||(!strcmp(op,"OR"))&&flagRO==1)
				{
					printf("\n-------LO IDE");
					flagW=1;
					
					if(!strcmp(op,"AND"))
						ro[*rn]=0;
					else if(!strcmp(op,"OR"))
						ro[*rn]=1;
					(*rn)++;
						
				}
				
				if(!strcmp(op,"where"))
				{
					//printf("set 0\n");
					flagF=0;
					flagW=1;
				}
				
				if(flagF==1)
				{
					tables[*n]=(char*)malloc(strlen(op)+1);
					tables[*n]=(char*)op;
					//printf("\n::::%d %s",*n,tables[*n]);
					(*n)++;
				}
				if(!strcmp(op,"from"))
				{
					//printf("breaking\n");
					flagF=1;
					flagS=0;
				}
				if(flagS==1)
				{
					column[*cl]=(char*)malloc(strlen(op)+1);
					column[*cl]=(char*)op;
					printf("\n---%d %s",*cl,column[*cl]);
					(*cl)++;
				}
				if(!strcmp(op,"select"))
				{
					//printf("breaking\n");
					flagS=1;
				}
				
				op = strtok(NULL, " ,;");
				
		}
		if(flagF==1)
			(*n)--;

		if((*rn)!=(*cd)-1&&(*rn)!=0)
		{
			printf("Error in query %d %d",*rn,*cd);
			exit(0);
		}

		//else (*cd)--;
		//printf("\nssssssssssssssssppppppppppppppppppppppp");
}
