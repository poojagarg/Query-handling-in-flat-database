#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>

int table_print(int argc, char *argv[], char* collist[],char* id_cond[], int cl_len)
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
		strcpy (str,cwd);
		strcat (str, dirsmbl);
	        strcat (str, argv[file_var]);
   	        strcat (str, txtcontent);

	   	char table_ch, table_names[50];
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
		fgets ( line, sizeof line, fp );
		char *pos;
		if((pos=strchr(line,'\n'))!=NULL)
		{
			*pos='\0';
		}

		//to find index of selected columns
		char *pointer_colnames;
		
		int collist_len=sizeof(collist)/sizeof(collist[0]);
		int i=0,ptr_colnames=0;
		char header[128];
		strcpy(header,line);
		for(i=0;i<cl_len;i++)
		{
			ptr_colnames=0;
			strcpy(line,header);
			printf("\n0...%s\n%s",collist[i],line);
			pointer_colnames = (char*) strtok(line, ","); 		
			while(pointer_colnames!=NULL)
			{
			
				printf("\n1a....%s...%s",collist[i],pointer_colnames);
				if(strcmp(collist[i],pointer_colnames)==0)
				{
					printf("\n%s .1.. %s...%d\n",collist[i],pointer_colnames,ptr_colnames);
					index_col[i]=ptr_colnames;
					break;
				}
				pointer_colnames = (char*)strtok(NULL, ",");
				ptr_colnames++;
			}
		}

		for(i=0;i<cl_len;i++)
			printf("\n4.....%d",index_col[i]);
		int sel_length=sizeof(index_col)/sizeof(index_col[0]);
		printf("\ncoll %d",sel_length);
		while(fgets(line,sizeof line,fp))
		{
			printf("\n");
			char *pointer;
			pointer=(char*)strtok(line,",");
			int index=0;
			while(pointer!=NULL)
			{
				for(i=0;i<cl_len;i++)
				{
					if(index_col[i]==index)
						printf("%s\t",pointer);
				}
				pointer = (char*)strtok(NULL, ",");
				index++;
			}
			
		}	
		
	
	return 0;
}
}

	int checkSelectedColumn(char* collist[],char* colname){
		int length_col=sizeof(collist)/sizeof(collist[0]);
		int i;
		
		length_col=i;
		printf("\nlen_col %d",length_col);
		printf("\n______%d",length_col);
		int length_var=0;
		while(length_var<=length_col)
		{
			printf("%s.....%d....%s<<<<<\n",collist[length_var],strcmp(collist[length_var],colname),colname);
			if(strcmp(collist[length_var],colname)==0)
				return 1;	
			
			length_var++;
		}
		return 0;
	}	
