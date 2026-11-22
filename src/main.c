#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "log.h"
#include "shm.h"

int main(int argc, char *argv[]) {
    printf("[anthill] pid=%d: start\n", getpid());
    
    // Parsuj argumenty
    Config cfg;
    if (config_parse(argc, argv, &cfg) != 0) {
        return 1;
    }
    
    // Otwórz log
    if (log_open("logs/simulation.log") != 0) {
        return 1;
    }
    
    // Utwórz pamięć dzieloną
    int shmid;
    Nest *nest = shm_create(cfg.K, cfg.G, &shmid);
    if (nest == NULL) {
        log_close();
        return 1;
    }
    
    log_write("anthill", "start M=%d K=%d G=%d shmid=%d", cfg.M, cfg.K, cfg.G, shmid);
    
    // Wypisz stan początkowy
    printf("Shared memory created: shmid=%d\n", shmid);
    printf("Nest: K=%d, K_max=%d, G=%d, occupancy=%d, alive=%d\n",
           nest->K, nest->K_max, nest->G, nest->occupancy, nest->alive);
    printf("Shafts: [%d,%d,%d] (0=FREE, 1=IN, 2=OUT)\n",
           nest->shafts[0].direction,
           nest->shafts[1].direction,
           nest->shafts[2].direction);
    
    // TODO: tutaj będzie fork+exec mother i ant
    
    log_write("anthill", "shutdown");
    
    // Sprzątanie
    shm_destroy(shmid, nest);
    log_close();
    
    printf("Cleanup complete.\n");
    return 0;
}