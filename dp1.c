#include "../../include/histogram.h"

pid_t dp2_pid = 0; //id for dp2
shared_memory_t* shm_ptr = NULL; //pointer to shared memory
int shm_id = 0;
int sem_id = 0;

// FUNCTION    : signal_handler
// DESCRIPTION : This function will shutdown the process in a clean manner
// PARAMETERS  : int signum
// RETURNS     : none


void signal_handler(int signum) {
    if (signum == SIGINT) {
        if (shm_ptr != NULL) { //clean up before memory is allocated
            cleanup_shared_memory(shm_id, shm_ptr);
        }
        if (sem_id != 0) { //clean up semaphore if started
            cleanup_semaphore(sem_id);
        }
        exit(0); //exits
    }
}

int main() {

    signal(SIGINT, signal_handler); //registers handlers for SIGNIT


    srand(time(NULL)); //generates random number


    initialize_shared_memory(&shm_ptr, &shm_id);
    initialize_semaphore(&sem_id);


    dp2_pid = fork(); //fork the process for dp2
    if (dp2_pid == -1) {
        perror("fork failed"); //if failed exists
        exit(1);
    }

    if (dp2_pid == 0) {

        char shm_id_str[10]; //for dp2
        sprintf(shm_id_str, "%d", shm_id); //converts id to string
        execl("./bin/dp2", "dp2", shm_id_str, NULL);
        perror("execl failed");
        exit(1);
    }


    while (1) { //main loop
        wait_semaphore(sem_id); //gets the semaphore before accessing memory

        //calculate the space and decides how many letter can be written
        int available_space = BUFFER_SIZE - ((shm_ptr->write_index - shm_ptr->read_index + BUFFER_SIZE) % BUFFER_SIZE);
        int letters_to_write = (available_space >= 20) ? 20 : available_space;

        if (letters_to_write > 0) { //random letter written

            for (int i = 0; i < letters_to_write; i++) {
                shm_ptr->buffer[shm_ptr->write_index] = generate_random_letter();
                shm_ptr->write_index = (shm_ptr->write_index + 1) % BUFFER_SIZE;
            }
        }

        signal_semaphore(sem_id);
        sleep(2);   //2 sec wait
    }

    return 0; //if not reached loop keep running
}