#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include "tablereader.c"


	int main()
	{
    		FILE *ptr_file;
    		char buf[1000];
		char *op,*tables[50],*column[50],*cond[50];
		char from[4],where[5],select[6];
		int flagS,flagF,flagW,n,k,cn=0,cd=0;
		flagS=0;
		flagW=0;
		flagF=0;
		n=0;
		
		strcpy(from,"from");
		strcpy(where,"where");
		strcpy(select,"select");

    		ptr_file =fopen("queries.txt","r");
    		if (!ptr_file)
        		return 1;

		

    		while (fgets(buf,1000, ptr_file)!=NULL)
		{
			printf("%s",buf);
			op=strtok(buf," ,;");
			while(op)
			{
				//printf("token: %s\n", op);
				//printf("::: %d\n",strcmp(op,from));
    				if(flagW==1)
				{
					cond[cd]=(char*)malloc(strlen(op)+1);
					cond[cd]=(char*)op;
					cd++;
				}	
				
				
				if(!strcmp(op,where))
				{
					//printf("set 0\n");
					flagF=0;
					flagW=1;
				}
				
				if(flagF==1)
				{
					tables[n]=(char*)malloc(strlen(op)+1);
					tables[n]=(char*)op;
					n++;
				}
				if(!strcmp(op,from))
				{
					//printf("breaking\n");
					flagF=1;
					flagS=0;
				}
				if(flagS==1)
				{
					column[cn]=(char*)malloc(strlen(op)+1);
					column[cn]=(char*)op;
					cn++;
				}
				if(!strcmp(op,select))
				{
					//printf("breaking\n");
					flagS=1;
				}
				op = strtok(NULL, " ,;");
			}
			
		}
/*
		printf("tables");
		for(k=0;k<n;k++)
			printf("  %s",(char*)tables[k]);
		printf("\ncolumns");

		for(k=0;k<cn;k++)
			printf("  %s...\n",(char*)column[k]);

		printf("\nconditions:::");
		for(k=0;k<n;k++)
			printf("  %s\n",(char*)cond[k]);
		fclose(ptr_file);
		printf("\n");
*/
		table_print(n,tables,column,cond,cn);
    		return 0;
	}
