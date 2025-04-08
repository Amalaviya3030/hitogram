/*
FILE          : histogram.h
PROJECT       : ASSIGNMENT 05
PROGRAMMER    : ANCHITA KAKRIA, UTTAM ARORA, ARYAN KUMAR
FIRST VERSION : 7 Apr 2025
DESCRIPTION   : This is a header file which has declarations for shared memory and semaphores and also
                function headers.
*/

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <time.h>

#define BUFFER_SIZE 256 //size of buffer
#define NUM_LETTERS 20  //no of letter to track
#define SEM_KEY 1234 //semphore key
#define SHM_KEY 5678 //shared memory


typedef struct {
    char buffer[BUFFER_SIZE]; //buffer to store letters
    int read_index; //index for reading
    int write_index; //index for writing
} shared_memory_t;


void initialize_shared_memory(shared_memory_t** shm_ptr, int* shm_id);
void cleanup_shared_memory(int shm_id, shared_memory_t* shm_ptr);
void initialize_semaphore(int* sem_id);
void cleanup_semaphore(int sem_id);
void wait_semaphore(int sem_id);
void signal_semaphore(int sem_id);
char generate_random_letter(void);
void display_histogram(int counts[NUM_LETTERS]);
void clear_screen(void);

#endif 