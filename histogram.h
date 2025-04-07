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

#define BUFFER_SIZE 256
#define NUM_LETTERS 20  
#define SEM_KEY 1234
#define SHM_KEY 5678

typedef struct {
    char buffer[BUFFER_SIZE];
    int read_index;
    int write_index;
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