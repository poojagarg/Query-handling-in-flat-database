#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int arr[100];
int i = 0;

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
    MPI_Offset localsize;
    MPI_Offset globalstart;
    int mysize, count = 0;
    char *chunk;
	char *tempResult[250];	
    
	
    /* read in relevant chunk of file into "chunk",
     * which starts at location in the file globalstart
     * and has size mysize 
     */
    {
        MPI_Offset globalend;
        MPI_Offset filesize;

        /* figure out who reads what */
        MPI_File_get_size(*in, &filesize);
        filesize--;  /* get rid of text file eof */
        mysize = filesize/size;
        globalstart = rank * mysize;
        globalend   = globalstart + mysize - 1;
        if (rank == size-1) globalend = filesize-1;

        /* add overlap to the end of everyone's chunk except last proc... */
        if (rank != size-1)
            globalend += overlap;

        mysize =  globalend - globalstart + 1;

        /* allocate memory */
        chunk = malloc( (mysize + 1)*sizeof(char));

        /* everyone reads in their part */
        MPI_File_read_at_all(*in, globalstart, chunk, mysize, MPI_CHAR, MPI_STATUS_IGNORE);
        chunk[mysize] = '\0';
    }


    /*
     * everyone calculate what their start and end *really* are by going 
     * from the first newline after start to the first newline after the
     * overlap region starts (eg, after end - overlap + 1)
     */

    int locstart=0, locend=mysize-1;
    if (rank != 0) {
        while(chunk[locstart] != '\n') locstart++;
        locstart++;
    }
    if (rank != size-1) {
        locend-=overlap;
        while(chunk[locend] != '\n') locend++;
    }
    mysize = locend-locstart+1;
	
	/* "Process" our chunk by replacing non-space characters with '1' for
     * rank 1, '2' for rank 2, etc... 
     */
	
	/*
	 * Declaring array 'line' which will contain the single record of the input file
	 */
	 char line[500];
	 int recordStart = 0, chunki, j;
	 
    for (chunki=locstart; chunki<=locend+1; chunki++) {
			
			char c = chunk[chunki];
			
			if (chunk[chunki] == '\n') {
				// process the line
				
					line[recordStart] = '\0';
					
					char* tmp1;
					char* tmp2;
					for (j = 0; j < i - 1; j++) {

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
			} else {
				line[recordStart] = chunk[chunki];
				recordStart++;
			}			
			
		}

		MPI_Barrier(MPI_COMM_WORLD);
		int p, t;
		for (p = 0; p < size; p++) {
			if (rank == p) {		
				printf("Process %d", p);		
				printf("\nCount %d\n", count);		
				for (t = 0; t < count; t++) {					
					printf("%s ", tempResult[t]);
					if ((t+1)%(i-1) == 0) {
						printf("\n");
					}				
					
				}				
				MPI_Barrier(MPI_COMM_WORLD);
			}
			
		}
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
                arr[i - 1] = 1;
            else if (strcmp(table[i], "name") == 0)
                arr[i - 1] = 2;
            else if (strcmp(table[i], "category") == 0)
                arr[i - 1] = 3;
            else if (strcmp(table[i], "price") == 0)
                arr[i - 1] = 4;
            else if (strcmp(table[i], "*") == 0){
                arr[i-1] = 1; i++;
                arr[i-1] = 2; i++;
                arr[i-1] = 3; i++;
                arr[i-1] = 4; i++;
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
    parprocess(&in, world_rank, world_size, overlap);
    //printf("After Par Process");	

    MPI_File_close(&in);
    
    
    // Finalize the MPI environment.
	MPI_Finalize();
    
}

