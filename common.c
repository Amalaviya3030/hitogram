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


void cleanup_shared_memory(int shm_id, shared_memory_t* shm_ptr) {

    if (shmdt(shm_ptr) == -1) {
        perror("shmdt failed");
    }


    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("shmctl failed");
    }
}

void initialize_semaphore(int* sem_id) {

    *sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (*sem_id == -1) {
        perror("semget failed");
        exit(1);
    }


    union semun {
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

void cleanup_semaphore(int sem_id) {
    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        perror("semctl failed");
    }
}

void wait_semaphore(int sem_id) {
    struct sembuf sb = { 0, -1, 0 };
    if (semop(sem_id, &sb, 1) == -1) {
        perror("semop wait failed");
    }
}
