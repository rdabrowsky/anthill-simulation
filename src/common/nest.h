#ifndef NEST_H
#define NEST_H

#include <pthread.h>

/**
 * Kierunek ruchu w szybie.
 */
typedef enum {
    SHAFT_FREE,     // szyb wolny
    SHAFT_INWARD,   // ruch do środka (do spichlerza)
    SHAFT_OUTWARD   // ruch na zewnątrz
} ShaftDirection;

/**
 * Stan pojedynczego szybu.
 */
typedef struct {
    ShaftDirection direction;  // aktualny kierunek
    int transit;               // ile mrówek w tranzycie
    int is_open;               // czy szyb otwarty (1) czy zamknięty (0)
} Shaft;

/**
 * Stan gniazda - struktura w pamięci dzielonej.
 */
typedef struct {
    // Parametry (ustawiane raz na starcie)
    int K;          // aktualna pojemność spichlerza
    int K_max;      // maksymalna pojemność (2 * K_initial)
    int G;          // limit populacji
    
    // Stan dynamiczny
    int occupancy;  // ile mrówek w spichlerzu
    int alive;      // ile mrówek żyje
    
    // Szyby
    Shaft shafts[3];
    
    // Flaga zakończenia
    int shutdown_flag;
    
    // Synchronizacja (PTHREAD_PROCESS_SHARED)
    pthread_mutex_t mutex;
    pthread_cond_t cond_entry;   // czekanie na wejście do spichlerza
    pthread_cond_t cond_inside;  // czekanie w środku (dla ewakuacji)
} Nest;

#endif // NEST_H