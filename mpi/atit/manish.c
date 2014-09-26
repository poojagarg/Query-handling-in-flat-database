#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<mpi.h>
int main(int argc,char *argv[])
{
MPI_Init(NULL, NULL);
int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

int world_rank,i;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

//printf("\n %s",argv[1]);
//printf("\n %s",argv[2]);

//printf("\n %s",argc);
int cnt=0;
char mid[128];
char a[10][20],*pch;
strcpy(mid,argv[3]);
	pch=strtok(mid,",");

	while(pch!=NULL)
	{
		strcpy(a[cnt],pch);
		pch=strtok(NULL,",");		
		cnt++;
	}
//printf("\n cnt %d",cnt);
int t=cnt-1;

for(i=0;i<cnt;i++)
{
char string[128]={""};
//printf("\n %s",a[i]);

if(i==world_rank)
{
printf("\n %s rabk %d",processor_name,world_rank);
strcat(string,"scp ");
strcat(string,argv[1]);
strcat(string," hduser@");
strcat(string,a[i]);
strcat(string,":~/");
strcat(string,argv[2]);
strcat(string,"\0");
}
printf("\n %s ",string);
system(string);
}
 MPI_Finalize();



return 0;
}

