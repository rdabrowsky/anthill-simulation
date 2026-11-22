#include "shm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>

/**
 * Inicjalizuje mutex z atrybutem PTHREAD_PROCESS_SHARED.
 * @return 0 przy sukcesie, -1 przy błędzie
 */
static int init_mutex(pthread_mutex_t *mutex) {
    pthread_mutexattr_t attr;
    int ret;
    
    if ((ret = pthread_mutexattr_init(&attr)) != 0) {
        errno = ret;
        perror("pthread_mutexattr_init");
        return -1;
    }
    
    if ((ret = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)) != 0) {
        errno = ret;
        perror("pthread_mutexattr_setpshared");
        pthread_mutexattr_destroy(&attr);
        return -1;
    }
    
    if ((ret = pthread_mutex_init(mutex, &attr)) != 0) {
        errno = ret;
        perror("pthread_mutex_init");
        pthread_mutexattr_destroy(&attr);
        return -1;
    }
    
    pthread_mutexattr_destroy(&attr);
    return 0;
}

/**
 * Inicjalizuje zmienną warunkową z atrybutem PTHREAD_PROCESS_SHARED.
 * @return 0 przy sukcesie, -1 przy błędzie
 */
static int init_cond(pthread_cond_t *cond) {
    pthread_condattr_t attr;
    int ret;
    
    if ((ret = pthread_condattr_init(&attr)) != 0) {
        errno = ret;
        perror("pthread_condattr_init");
        return -1;
    }
    
    if ((ret = pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)) != 0) {
        errno = ret;
        perror("pthread_condattr_setpshared");
        pthread_condattr_destroy(&attr);
        return -1;
    }
    
    if ((ret = pthread_cond_init(cond, &attr)) != 0) {
        errno = ret;
        perror("pthread_cond_init");
        pthread_condattr_destroy(&attr);
        return -1;
    }
    
    pthread_condattr_destroy(&attr);
    return 0;
}

Nest *shm_create(int cfg_K, int cfg_G, int *shmid) {
    // 1. Utwórz plik klucza (jeśli nie istnieje)
    int fd = open(SHM_KEY_PATH, O_CREAT | O_RDWR, 0600);
    if (fd == -1) {
        perror(SHM_KEY_PATH);
        return NULL;
    }
    close(fd);
    
    // 2. Generuj klucz IPC
    key_t key = ftok(SHM_KEY_PATH, SHM_PROJECT_ID);
    if (key == -1) {
        perror("ftok");
        unlink(SHM_KEY_PATH);
        return NULL;
    }
    
    // 3. Utwórz segment pamięci dzielonej
    *shmid = shmget(key, sizeof(Nest), IPC_CREAT | IPC_EXCL | 0600);
    if (*shmid == -1) {
        perror("shmget");
        unlink(SHM_KEY_PATH);
        return NULL;
    }
    
    // 4. Dołącz segment do przestrzeni adresowej
    Nest *nest = shmat(*shmid, NULL, 0);
    if (nest == (void *)-1) {
        perror("shmat");
        shmctl(*shmid, IPC_RMID, NULL);
        unlink(SHM_KEY_PATH);
        return NULL;
    }
    
    // 5. Inicjalizuj strukturę Nest
    memset(nest, 0, sizeof(Nest));
    nest->K = cfg_K;
    nest->K_max = 2 * cfg_K;
    nest->G = cfg_G;
    nest->occupancy = 0;
    nest->alive = 0;
    nest->shutdown_flag = 0;
    
    // Inicjalizuj szyby
    for (int i = 0; i < 3; i++) {
        nest->shafts[i].direction = SHAFT_FREE;
        nest->shafts[i].transit = 0;
        nest->shafts[i].is_open = 1;  // wszystkie otwarte na start
    }
    
    // 6. Inicjalizuj mutex i zmienne warunkowe
    if (init_mutex(&nest->mutex) != 0) {
        shmdt(nest);
        shmctl(*shmid, IPC_RMID, NULL);
        unlink(SHM_KEY_PATH);
        return NULL;
    }
    
    if (init_cond(&nest->cond_entry) != 0) {
        pthread_mutex_destroy(&nest->mutex);
        shmdt(nest);
        shmctl(*shmid, IPC_RMID, NULL);
        unlink(SHM_KEY_PATH);
        return NULL;
    }
    
    if (init_cond(&nest->cond_inside) != 0) {
        pthread_cond_destroy(&nest->cond_entry);
        pthread_mutex_destroy(&nest->mutex);
        shmdt(nest);
        shmctl(*shmid, IPC_RMID, NULL);
        unlink(SHM_KEY_PATH);
        return NULL;
    }
    
    return nest;
}

Nest *shm_attach(int shmid) {
    Nest *nest = shmat(shmid, NULL, 0);
    if (nest == (void *)-1) {
        perror("shmat");
        return NULL;
    }
    return nest;
}

void shm_detach(Nest *nest) {
    if (shmdt(nest) == -1) {
        perror("shmdt");
    }
}

void shm_destroy(int shmid, Nest *nest) {
    // Zniszcz synchronizację
    pthread_cond_destroy(&nest->cond_inside);
    pthread_cond_destroy(&nest->cond_entry);
    pthread_mutex_destroy(&nest->mutex);
    
    // Odłącz i usuń segment
    shmdt(nest);
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl IPC_RMID");
    }
    
    // Usuń plik klucza
    if (unlink(SHM_KEY_PATH) == -1) {
        perror("unlink");
    }
}