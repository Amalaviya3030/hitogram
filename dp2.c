#include "../../include/histogram.h"

pid_t dc_pid = 0;
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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <shm_id>\n", argv[0]);
        exit(1);
    }


    shm_id = atoi(argv[1]);


    signal(SIGINT, signal_handler);


    srand(time(NULL));


    pid_t dp2_pid = getpid();
    pid_t dp1_pid = getppid();


    dc_pid = fork();
    if (dc_pid == -1) {
        perror("fork failed");
        exit(1);
    }

    if (dc_pid == 0) {

        char dp2_pid_str[10];
        char dp1_pid_str[10];
        char shm_id_str[10];
        sprintf(dp2_pid_str, "%d", dp2_pid);
        sprintf(dp1_pid_str, "%d", dp1_pid);
        sprintf(shm_id_str, "%d", shm_id);
        execl("./bin/dc", "dc", dp2_pid_str, dp1_pid_str, shm_id_str, NULL);
        perror("execl failed");
        exit(1);
    }


    shm_ptr = (shared_memory_t*)shmat(shm_id, NULL, 0);
    if (shm_ptr == (void*)-1) {
        perror("shmat failed");
        exit(1);
    }


    sem_id = semget(SEM_KEY, 1, 0);
    if (sem_id == -1) {
        perror("semget failed");
        exit(1);
    }

    while (1) {
        wait_semaphore(sem_id);


        int available_space = BUFFER_SIZE - ((shm_ptr->write_index - shm_ptr->read_index + BUFFER_SIZE) % BUFFER_SIZE);

        if (available_space > 0) {

            shm_ptr->buffer[shm_ptr->write_index] = generate_random_letter();
            shm_ptr->write_index = (shm_ptr->write_index + 1) % BUFFER_SIZE;
        }

        signal_semaphore(sem_id);
        usleep(50000);
    }

    return 0;
}