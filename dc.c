/*
FILE          : dc.c
PROJECT       : ASSIGNMENT 05
PROGRAMMER    : ANCHITA KAKRIA, UTTAM ARORA, ARYAN KUMAR
FIRST VERSION : 7 april 2025
DESCRIPTION   : This files is going to act as a consumer that will read shared memory and update
                histogram and handle signals and inter process communication
*/

#include "../../include/histogram.h"

pid_t dp1_pid = 0; //id of dp1
pid_t dp2_pid = 0; //id of dp2
shared_memory_t* shm_ptr = NULL;
int shm_id = 0;//shared memory identifier
int sem_id = 0;//identifier
int counts[NUM_LETTERS] = { 0 }; //array to hold characters
int should_exit = 0;

// FUNCTION    : signal_handler
// DESCRIPTION : This function will shutdown the process in a clean manner and SIGALRM will keep alarm active
// PARAMETERS  : int signum
// RETURNS     : none


void signal_handler(int signum) {
    if (signum == SIGINT) {
        should_exit = 1; //exit flag

        kill(dp1_pid, SIGINT); //dignit sends to dps to close and terminate
        kill(dp2_pid, SIGINT);
    }
    else if (signum == SIGALRM) {

        alarm(2); //reset in 2 secs
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <dp2_pid> <dp1_pid> <shm_id>\n", argv[0]); //usage message
        exit(1);
    }


    dp2_pid = atoi(argv[1]);
    dp1_pid = atoi(argv[2]);
    shm_id = atoi(argv[3]);


    signal(SIGINT, signal_handler); //signals
    signal(SIGALRM, signal_handler);


    shm_ptr = (shared_memory_t*)shmat(shm_id, NULL, 0); //attach to shared memory
    if (shm_ptr == (void*)-1) {
        perror("shmat failed");
        exit(1);
    }


    sem_id = semget(SEM_KEY, 1, 0);
    if (sem_id == -1) {
        perror("semget failed"); //error if not found
        exit(1);
    }


    alarm(2);// starts again in 2 secs


    while (!should_exit) { //main loop
        wait_semaphore(sem_id);


        while (shm_ptr->read_index != shm_ptr->write_index) { //read characters from memory
            char letter = shm_ptr->buffer[shm_ptr->read_index]; //reads
            counts[letter - 'A']++;   //increasing the counts for every letter
            shm_ptr->read_index = (shm_ptr->read_index + 1) % BUFFER_SIZE; //moves on to next
        }

        signal_semaphore(sem_id);
        pause();   //its stopped until a new signal is given
    }


    display_histogram(counts); //diaplays the final letter frequency


    if (shm_ptr != NULL) { //clean up
        cleanup_shared_memory(shm_id, shm_ptr);
    }

    printf("Shazam !!\n");
    return 0;
}