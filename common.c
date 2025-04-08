/*
FILE          : common.c
PROJECT       : ASSIGNMENT 05
PROGRAMMER    : ANCHITA KAKRIA, UTTAM ARORA, ARYAN KUMAR
FIRST VERSION : 7 Apr 2025
DESCRIPTION   :
*/

#include "../include/histogram.h"

// FUNCTION    : initialize_shared_memory
// DESCRIPTION :  gets the shared memory and initializes it reads it and then writes it
// PARAMETERS  : shared_memory_t **shm_ptr , int *shm_id
// RETURNS     : none

void initialize_shared_memory(shared_memory_t** shm_ptr, int* shm_id) {

    *shm_id = shmget(SHM_KEY, sizeof(shared_memory_t), IPC_CREAT | 0666); //allocates memory with key and size
    if (*shm_id == -1) {
        perror("shmget failed");
        exit(1);
    }


    *shm_ptr = (shared_memory_t*)shmat(*shm_id, NULL, 0); //attaches memory
    if (*shm_ptr == (void*)-1) {
        perror("shmat failed");
        exit(1);
    }

    //initialized
    (*shm_ptr)->read_index = 0;
    (*shm_ptr)->write_index = 0;
}

// FUNCTION    : cleanup_shared_memory
// DESCRIPTION : Detaches and deallocates the shared memory segment.
// PARAMETERS  : int shm_id , shared_memory_t *shm_ptr 
// RETURNS     : none

void cleanup_shared_memory(int shm_id, shared_memory_t* shm_ptr) {

    if (shmdt(shm_ptr) == -1) {
        perror("shmdt failed");
    }


    if (shmctl(shm_id, IPC_RMID, NULL) == -1) { //marks the memory
        perror("shmctl failed");
    }
}

// FUNCTION    : initialize_semaphore
// DESCRIPTION : creates semaphore and startes its value to 1
// PARAMETERS  : int *sem
// RETURNS     : none

void initialize_semaphore(int* sem_id) {

    *sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666); //creates a single semaphore
    if (*sem_id == -1) {
        perror("semget failed");
        exit(1);
    }


    union semun { //sets the initial value to 1
        int val;
        struct semid_ds* buf;
        unsigned short* array;
    } arg;
    arg.val = 1;
    if (semctl(*sem_id, 0, SETVAL, arg) == -1) {
        perror("semctl failed");
        exit(1);
    }
}

// FUNCTION    : cleanup_semaphore
// DESCRIPTION : removes semaphore 
// PARAMETERS  : int sem_id 
// RETURNS     : none

void cleanup_semaphore(int sem_id) {
    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        perror("semctl failed");
    }
}

// FUNCTION    : wait_semaphore
// DESCRIPTION : This function performs a wait operation
// PARAMETERS  : int sem_id 
// RETURNS     : none

void wait_semaphore(int sem_id) {
    struct sembuf sb = { 0, -1, 0 }; //decreases the value
    if (semop(sem_id, &sb, 1) == -1) {
        perror("semop wait failed");
    }
}

// FUNCTION    : signal_semaphore
// DESCRIPTION : This function persorms the signal operation
// PARAMETERS  : int sem_id 
// RETURNS     : none

void signal_semaphore(int sem_id) {
    struct sembuf sb = { 0, 1, 0 }; //increases the value
    if (semop(sem_id, &sb, 1) == -1) {
        perror("semop signal failed");
    }
}

// FUNCTION    : generate_random_letter
// DESCRIPTION : This function generates a random letter
// PARAMETERS  : void
// RETURNS     : char 

char generate_random_letter(void) {
    return 'A' + (rand() % NUM_LETTERS); //returns a charc from A to Z
}

// FUNCTION    : clear_screen
// DESCRIPTION : clears the screen
// PARAMETERS  : void
// RETURNS     : none

void clear_screen(void) {
    system("clear");
}

// FUNCTION    : display_histogram
// DESCRIPTION : this function displays the histogram based on letter frequency
// PARAMETERS  : int counts[NUM_LETTERS] 
// RETURNS     : none

void display_histogram(int counts[NUM_LETTERS]) {
    clear_screen(); //clears screen before showing it
    for (int i = 0; i < NUM_LETTERS; i++) {
        printf("%c-%03d ", 'A' + i, counts[i]); //prints the letter and its count


        int count = counts[i];
        while (count >= 100) { //prints * for 100 sets
            printf("*");
            count -= 100;
        }
        while (count >= 10) { //prints + for 10
            printf("+");
            count -= 10;
        }
        while (count > 0) { //prints - for 1
            printf("-");
            count--;
        }
        printf("\n"); //new line for new charc
    }
    printf("\n");
}