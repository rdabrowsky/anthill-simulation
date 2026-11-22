#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "log.h"

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
    
    // Zaloguj start
    log_write("anthill", "start M=%d K=%d G=%d", cfg.M, cfg.K, cfg.G);
    
    // Tymczasowo: wypisz config
    printf("Config: M=%d, K=%d, G=%d, T_c=%d, T_ink=%d, T_K=%d\n",
           cfg.M, cfg.K, cfg.G, cfg.T_c, cfg.T_ink, cfg.T_K);
    printf("        D=[%d,%d], E=[%d,%d]\n",
           cfg.D_min, cfg.D_max, cfg.E_min, cfg.E_max);
    
    log_write("anthill", "shutdown");
    log_close();
    
    return 0;
}