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