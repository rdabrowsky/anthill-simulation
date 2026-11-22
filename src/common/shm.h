#ifndef SHM_H
#define SHM_H

#include "nest.h"

/**
 * Ścieżka do pliku klucza dla ftok().
 */
#define SHM_KEY_PATH "/tmp/anthill_shm_key"
#define SHM_PROJECT_ID 'A'

/**
 * Tworzy segment pamięci dzielonej i inicjalizuje strukturę Nest.
 * 
 * @param cfg_K pojemność spichlerza (K)
 * @param cfg_G limit populacji (G)
 * @param shmid [out] identyfikator segmentu (do przekazania dzieciom)
 * @return wskaźnik do Nest lub NULL przy błędzie (perror wywoływane)
 */
Nest *shm_create(int cfg_K, int cfg_G, int *shmid);

/**
 * Dołącza się do istniejącego segmentu (dla procesów potomnych).
 * 
 * @param shmid identyfikator segmentu
 * @return wskaźnik do Nest lub NULL przy błędzie
 */
Nest *shm_attach(int shmid);

/**
 * Odłącza się od segmentu (dla procesów potomnych).
 * 
 * @param nest wskaźnik do odłączenia
 */
void shm_detach(Nest *nest);

/**
 * Niszczy segment i usuwa plik klucza (tylko właściciel - anthill).
 * 
 * @param shmid identyfikator segmentu
 * @param nest wskaźnik do struktury (do zniszczenia mutexów)
 */
void shm_destroy(int shmid, Nest *nest);

#endif // SHM_H