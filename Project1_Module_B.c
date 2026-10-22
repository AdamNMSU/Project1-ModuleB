#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#include "Project1_Module_B.h"   

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_processes>\n", argv[0]);
        return 1;
    }

    int num_processes = atoi(argv[1]);
    if (num_processes <= 0) {
        printf("Invalid number of processes: %d\n", num_processes);
        return 1;
    }


    struct timespec start, end;
    long long seconds, nano_seconds;


    /* ToDo1
     * Create and attach shared memory region 1 to store the matrix
     * 
     * Hint: Google shmget() and shmat()
     */
    //Unique key identifier for region 1
    key_t key1 = 0x1234;
    //initalize matrix size
    size_t matrix_size = (size_t)ROWS_COUNT * COLUMN_COUNT * sizeof(int);
    //create key if missing in parrent
    int shmid1 = shmget(key1, matrix_size, IPC_CREAT | 0666);
    //error handling
    if (shmid1 < 0) {
        printf("Error with smget matrix");
        exit(1);
    }
    //initalize marix, with error handling
    int *matrix = (int *)shmat(shmid1, NULL, 0);
    if(matrix == (int*) -1){
        printf("Error with shmat matrix");
        exit(1);
    }
    //parrent process initalized with shared memory region 1 stored matrix

    /* ToDo2
     * create and attach shared memory region 2 to store the partial sums that
     * are returned by children processes.
    
     */
     //unique key identifier for region 2
    key_t key2 = 0x5678;
    //partial sums bytes
    size_t sums_bytes = (size_t)num_processes * sizeof(long);
    int shmid2 = shmget(key2, sums_bytes, IPC_CREAT | 0666);
    if(shmid2 < 0)
    {
        printf("Error with smghet key2 partial sums");
        exit(1);
    }

    long *partial_sums = (long *)shmat(shmid2, NULL, 0);
    if(partial_sums == (long*)-1){
        printf("Error with shmat key2 partial sums");
        exit(1);
    }
    //partial sums initialized in memory region 2

    /* ToDo3
     * Initialize the shared memory region 1 to hold
     * the matrix of size ROW_COUNT x COLUMN_COUNT.
     * Value of each element is set to i+j, where i is index of row
     * and j is index of column.
     * 
     * Hint: Treat starting address of shared memory region as first
     * elemnt of a 2D matrix i.e.,  matrix[0][0]
    */

    for(int i = 0; i < ROWS_COUNT; i++)
    {
        for(int j = 0; j < COLUMN_COUNT; j++)
        {
            matrix[i * COLUMN_COUNT + j] = i + j;
        }
    }


    clock_gettime(CLOCK_REALTIME, &start);


    /* ToDo4:
     * 1) Create num_processes count of children processes. (using fork). Note that each child process runs a new binary 
     * (i.e., binary of child_proc.c) using execlp()
     * 2) Specify for each process the start_row, end_row, and
     * index in shared memory region 2 where a child process is supposed to store its
     * calculated partial sum.
     *
     *
     * Hints:
     *
     * Google "man fork" and "man execlp"
     *
    */



    /* ToDo5
     * Wait for all children processes to complete
    */



    /* ToDo6:
     * Sum all the partial-sums stored in sharred memory region 2 (by children processes)
     * and print the sum.
    */


    clock_gettime(CLOCK_REALTIME, &end);
    // Calculate execution time
    seconds=end.tv_sec - start.tv_sec;
    nano_seconds=end.tv_nsec - start.tv_nsec;
    long double executio_time=(long double)seconds+((long double)nano_seconds/1000000000);
    printf("Execution Time with %d process(es): %Lf\n",num_processes,executio_time);


    /* ToDo7:
     * Detach and destroy both shared memory regions.
     * Hint:
     * Google "man shmdt" and "man shmctl"
    */




    return 0;
}


