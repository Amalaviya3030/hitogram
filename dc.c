
pid_t dp1_pid = 0;
pid_t dp2_pid = 0;
shared_memory_t* shm_ptr = NULL;
int shm_id = 0;
int sem_id = 0;
int counts[NUM_LETTERS] = { 0 };
int should_exit = 0;

void signal_handler(int signum) {
    if (signum == SIGINT) {
        should_exit = 1;

        kill(dp1_pid, SIGINT);
        kill(dp2_pid, SIGINT);
    }
    else if (signum == SIGALRM) {

        alarm(2);
    }
}
int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <dp2_pid> <dp1_pid> <shm_id>\n", argv[0]);
        exit(1);
    }


    dp2_pid = atoi(argv[1]);
    dp1_pid = atoi(argv[2]);
    shm_id = atoi(argv[3]);


    signal(SIGINT, signal_handler);
    signal(SIGALRM, signal_handler);


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


    alarm(2);


    while (!should_exit) {
        wait_semaphore(sem_id);


        while (shm_ptr->read_index != shm_ptr->write_index) {
            char letter = shm_ptr->buffer[shm_ptr->read_index];
            counts[letter - 'A']++;
            shm_ptr->read_index = (shm_ptr->read_index + 1) % BUFFER_SIZE;
        }

        signal_semaphore(sem_id);
        pause();
    }


    display_histogram(counts);


    if (shm_ptr != NULL) {
        cleanup_shared_memory(shm_id, shm_ptr);
    }

    printf("Shazam !!\n");
    return 0;
}