
#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int myrank, nprocs;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);
 
  // Print off a hello world message
  printf("Hello world from processor %s, rank %d"
         " out of %d processors\n",



         processor_name, myrank, nprocs);
    printf("Hello from processor %d of %d\n", myrank, nprocs);

    MPI_Finalize();
    return 0;

}
