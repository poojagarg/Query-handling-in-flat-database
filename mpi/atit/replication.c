#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int arr[100];
int i = 0;
int cnti=0;
char* getfield(char* line, int num) {
    char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n")) {
        if (!--num) {			
            return tok;
		}
    }
    return NULL;
}


/**
 * Function to pre-process the input text file
 * */
 void parprocess(MPI_File *in, const int rank, const int size, const int overlap) {
   
   
    int mysize, count = 0;
    char *chunk;
	char *tempResult[250];	
    
	MPI_Offset filesize;
    /* read in relevant chunk of file into "chunk",
     * which starts at location in the file globalstart
     * and has size mysize 
     */
    
       
        

        /* figure out who reads what */
        MPI_File_get_size(*in, &filesize);
        
printf("\n here file %d",filesize);

        /* allocate memory */
        chunk = (char *)malloc( (filesize + 1)*sizeof(char));

        /* everyone reads in their part */
        MPI_File_read_at(*in, 0, chunk, filesize, MPI_CHAR, MPI_STATUS_IGNORE);
        chunk[filesize] = '\0';
    


    //printf("\n worf %c",chunk[0]);
	 
	 char line[500];
	int recordStart = 0, chunki, j;
	 
    for (chunki=0; chunki<=filesize; chunki++) {
			
			char c = chunk[chunki];
			//printf("\n char %c ",c);
			if (chunk[chunki] == '\n') 
			{
				// process the line
				
					line[recordStart] = '\0';
					
					char* tmp1;
					char* tmp2;
					printf("\n %s",line);					
					
					for (j = 0; j < cnti; j++)
					 {

						tmp1 = strdup(line);
						tmp2 = strdup(line);
						
						char *temporary = (char *)malloc(sizeof(char)*strlen(getfield(tmp1, arr[j])));
						temporary = getfield(tmp2, arr[j]);
						strcat(temporary, "\0");
						tempResult[count++] = temporary;
						printf("\n word %s",temporary);		
					}	
				
				//processing ends
				recordStart = 0;
			} else 
			{
				line[recordStart] = chunk[chunki];
				recordStart++;
			}			
			
		}

		
		
		
				/*	
				printf("Process %d", p);		
				printf("\nCount %d\n", count);		
				for (t = 0; t < count; t++) {					
					printf("%s ", tempResult[t]);
					if ((t+1)%(i-1) == 0) {
						printf("\n");
					}				
					
				}*/				
				
			
			
		
    return;
}
// Function ends

int main(int argc, char** argv) {
    
    
    
    
    char line[1024]="select id from product";
    //gets(line);
    
    const char *tok;
    char *table[100];
    
    int j, overlap = 50, ierr;
    
  // MPI File Handle
  MPI_File in;  
  // Initialize the MPI environment
  MPI_Init(&argc, &argv);
 
  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
 
  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_File out[world_rank];  
  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);
 
  // Print off a hello world message
  printf("Query Result from processor %s, rank %d out of %d processors\n",processor_name, world_rank, world_size);
    
    for (tok = strtok(line, " "); tok && *tok; tok = strtok(NULL, " \n"))
        table[i++] = tok;

    if (strcmp(table[0], "select") == 0) {
        i = 1;
        while (strcmp(table[i], "from") != 0) {
            if (strcmp(table[i], "id") == 0)
                {arr[i - 1] = 1;cnti++;}
            else if (strcmp(table[i], "name") == 0)
                {arr[i - 1] = 2;cnti++;}
            else if (strcmp(table[i], "category") == 0)
                {arr[i - 1] = 3;cnti++;}
            else if (strcmp(table[i], "price") == 0)
                {arr[i - 1] = 4;cnti++;}
            else if (strcmp(table[i], "*") == 0){
                arr[i-1] = 1; i++;
                arr[i-1] = 2; i++;
                arr[i-1] = 3; i++;
                arr[i-1] = 4; i++;
		cnti+=4;                
		break;
            }
            i++;
        }
    } else {
        printf("Fire Appropiate Query\n");
        exit(1);
    }

    
	/*
	 * File opening for Reading 
	 */
		
		 ierr = MPI_File_open(MPI_COMM_WORLD, "product.csv", MPI_MODE_RDONLY, MPI_INFO_NULL, &in);
		 //printf("ierr = %d", ierr);
		if (ierr) {
			if (world_rank == 0) fprintf(stderr, "%s: Couldn't open file %s\n", argv[0], argv[1]);
			MPI_Finalize();
			exit(2);
		}
   
   
		
	//printf("Before Par Process");	
	if(world_rank==0)
{
printf("hre ");
    parprocess(&in, world_rank, world_size, overlap);
}


    //printf("After Par Process");	
MPI_Barrier(MPI_COMM_WORLD);
    MPI_File_close(&in);
    
    
    // Finalize the MPI environment.
	MPI_Finalize();
    
}

