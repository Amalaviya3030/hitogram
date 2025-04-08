#include "../include/histogram.h"

void initialize_shared_memory(shared_memory_t** shm_ptr, int* shm_id) {

    *shm_id = shmget(SHM_KEY, sizeof(shared_memory_t), IPC_CREAT | 0666);
    if (*shm_id == -1) {
        perror("shmget failed");
        exit(1);
    }
    
    *shm_ptr = (shared_memory_t*)shmat(*shm_id, NULL, 0);
    if (*shm_ptr == (void*)-1) {
        perror("shmat failed");
        exit(1);
    }

    (*shm_ptr)->read_index = 0;
    (*shm_ptr)->write_index = 0;
}