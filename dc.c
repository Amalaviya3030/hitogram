
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
