#include "../../include/histogram.h"

pid_t dp2_pid = 0;
shared_memory_t* shm_ptr = NULL;
int shm_id = 0;
int sem_id = 0;

void signal_handler(int signum) {
    if (signum == SIGINT) {
        if (shm_ptr != NULL) {
            cleanup_shared_memory(shm_id, shm_ptr);
        }
        if (sem_id != 0) {
            cleanup_semaphore(sem_id);
        }
        exit(0);
    }
}

int main() {

    signal(SIGINT, signal_handler);


    srand(time(NULL));


    initialize_shared_memory(&shm_ptr, &shm_id);
    initialize_semaphore(&sem_id);


    dp2_pid = fork();
    if (dp2_pid == -1) {
        perror("fork failed");
        exit(1);
    }

    if (dp2_pid == 0) {

        char shm_id_str[10];
        sprintf(shm_id_str, "%d", shm_id);
        execl("./bin/dp2", "dp2", shm_id_str, NULL);
        perror("execl failed");
        exit(1);
    }


    while (1) {
        wait_semaphore(sem_id);


        int available_space = BUFFER_SIZE - ((shm_ptr->write_index - shm_ptr->read_index + BUFFER_SIZE) % BUFFER_SIZE);
        int letters_to_write = (available_space >= 20) ? 20 : available_space;

        if (letters_to_write > 0) {

            for (int i = 0; i < letters_to_write; i++) {
                shm_ptr->buffer[shm_ptr->write_index] = generate_random_letter();
                shm_ptr->write_index = (shm_ptr->write_index + 1) % BUFFER_SIZE;
            }
        }

        signal_semaphore(sem_id);
        sleep(2);
    }

    return 0;
}