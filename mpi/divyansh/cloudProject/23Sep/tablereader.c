#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
 
int table_print(int argc, char *argv[], char* collist[],char* id_cond[])
{

int file_var=0;
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	
	int *index_col=(int*)malloc(sizeof(int)*argc);
	char *dirsmbl = "/input/";
	char *txtcontent = ".txt";
	
	
   while(file_var<argc)
	{
		//Here table names are being read 		
		char str[1024];
		int nc=0,i;
		strcpy (str,cwd);
		strcat (str, dirsmbl);
	        strcat (str, argv[file_var]);
   	        strcat (str, txtcontent);

	   	char table_ch, table_names[50],*columns[50];
	        FILE *fp;
		fp = fopen(str,"r");
 		
	   	if( fp == NULL )
   		{
		printf("The contents of %s file are :\n", str);
      		perror("Error while opening the file.\n");
	        exit(EXIT_FAILURE);
   		}
 	        //printf("The contents of %s file are :\n", argv[file_var]);
		char line[128];
		fgets ( line, sizeof(line), fp );
		printf("%s********\n",line);
		char *colNames;
		colNames = (char*)strtok(line,",");

		while(colNames)
		{
			columns[nc]=(char*)malloc(strlen(colNames)+1);
			columns[nc]=(char*)colNames;
			nc++;
			//printf("\n\n****%s********\n",colNames);
			colNames = strtok(NULL, ",");
		}

		int collist_size=sizeof(collist)/sizeof(index_col[0]);
		printf("\n****%d\n",collist_size);

		while(fgets (line, sizeof line, fp ) != NULL)



		/*char *pointer_colnames;		
		pointer_colnames = (char*) strtok(line, ","); 
		int colvar=0;
		while(pointer_colnames != NULL)
		{
			if(checkSelectedColumn(collist,pointer_colnames)>0)
			{
				index_col[sizeof(index_col)/sizeof(index_col[0])]=colvar;
				printf("%d,%s,%s...\n",colvar,"\t",pointer_colnames);							
			}		
			pointer_colnames = (char*) strtok(NULL, ",");
			colvar++;
		}
		
		int sel_length=sizeof(index_col)/sizeof(index_col[0]);

		while (fgets (line, sizeof line, fp ) != NULL ) // read a line 
		{
		printf("%s",line);
		char *pointer;
		
		pointer = (char*) strtok(line, ","); //skip only spaces
		int col_select_var=0,j=0;		
		while(col_select_var<sel_length)
		   {
		//printf("ArrEle%d\n",index_col[col_select_var]);
		if(col_select_var==index_col[col_select_var]);
		printf("%s\n",pointer);
		pointer = (char*) strtok(NULL, ",");
			
   		col_select_var++;   
		}
		}*/
		fclose ( fp );
		
      	file_var++;
	}	   
	return 0;
}

	int checkSelectedColumn(char* collist[],char* colname){
	int length_col=sizeof(collist)/sizeof(collist[0]);
		

	printf("______%d",length_col);
	int length_var=0;
	while(length_var<=length_col)
	{
	printf("%s.....%d....%s<<<<<\n",collist[length_var],strcmp(collist[length_var],colname),colname);
	if(strcmp(collist[length_var],colname)==0){

	return 1;	
	}
	length_var++;
	}
	return 0;
	}	
