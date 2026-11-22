#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

/**
 * Pomocnicza funkcja do parsowania pojedynczego argumentu.
 * @return 0 przy sukcesie, -1 przy błędzie
 */
static int parse_int(const char *str, const char *name, int *out) {
    char *endptr;
    errno = 0;
    long val = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') {
        fprintf(stderr, "Błąd: %s musi być liczbą całkowitą (podano: '%s')\n", name, str);
        return -1;
    }
    if (errno == ERANGE || val < INT_MIN || val > INT_MAX) {
        fprintf(stderr, "Błąd: %s poza zakresem (podano: '%s')\n", name, str);
        return -1;
    }
    
    *out = (int)val;
    return 0;
}

int config_parse(int argc, char *argv[], Config *cfg) {
    // Oczekujemy: program M K G T_c T_ink T_K D_min D_max E_min E_max
    if (argc != 11) {
        fprintf(stderr, "Użycie: %s M K G T_c T_ink T_K D_min D_max E_min E_max\n", argv[0]);
        fprintf(stderr, "  M      - początkowa liczba mrówek (>= 3)\n");
        fprintf(stderr, "  K      - pojemność spichlerza (1 <= K < M/2)\n");
        fprintf(stderr, "  G      - limit populacji (>= M)\n");
        fprintf(stderr, "  T_c    - timeout wejścia [s] (> 0)\n");
        fprintf(stderr, "  T_ink  - okres inkubacji [s] (> 0)\n");
        fprintf(stderr, "  T_K    - czas zamknięcia [s] (> 0)\n");
        fprintf(stderr, "  D_min  - min czas w spichlerzu [s] (> 0)\n");
        fprintf(stderr, "  D_max  - max czas w spichlerzu [s] (>= D_min)\n");
        fprintf(stderr, "  E_min  - min liczba cykli (> 0)\n");
        fprintf(stderr, "  E_max  - max liczba cykli (>= E_min)\n");
        return -1;
    }
    
    // Parsuj wszystkie argumenty
    if (parse_int(argv[1], "M", &cfg->M) != 0) return -1;
    if (parse_int(argv[2], "K", &cfg->K) != 0) return -1;
    if (parse_int(argv[3], "G", &cfg->G) != 0) return -1;
    if (parse_int(argv[4], "T_c", &cfg->T_c) != 0) return -1;
    if (parse_int(argv[5], "T_ink", &cfg->T_ink) != 0) return -1;
    if (parse_int(argv[6], "T_K", &cfg->T_K) != 0) return -1;
    if (parse_int(argv[7], "D_min", &cfg->D_min) != 0) return -1;
    if (parse_int(argv[8], "D_max", &cfg->D_max) != 0) return -1;
    if (parse_int(argv[9], "E_min", &cfg->E_min) != 0) return -1;
    if (parse_int(argv[10], "E_max", &cfg->E_max) != 0) return -1;
    
    // Walidacja wartości
    if (cfg->M < 3) {
        fprintf(stderr, "Błąd: M musi być >= 3 (podano: %d)\n", cfg->M);
        return -1;
    }
    if (cfg->K < 1) {
        fprintf(stderr, "Błąd: K musi być >= 1 (podano: %d)\n", cfg->K);
        return -1;
    }
    if (cfg->K >= cfg->M / 2) {
        fprintf(stderr, "Błąd: K musi być < M/2 (K=%d, M/2=%d)\n", cfg->K, cfg->M / 2);
        return -1;
    }
    if (cfg->G < cfg->M) {
        fprintf(stderr, "Błąd: G musi być >= M (G=%d, M=%d)\n", cfg->G, cfg->M);
        return -1;
    }
    if (cfg->T_c <= 0) {
        fprintf(stderr, "Błąd: T_c musi być > 0 (podano: %d)\n", cfg->T_c);
        return -1;
    }
    if (cfg->T_ink <= 0) {
        fprintf(stderr, "Błąd: T_ink musi być > 0 (podano: %d)\n", cfg->T_ink);
        return -1;
    }
    if (cfg->T_K <= 0) {
        fprintf(stderr, "Błąd: T_K musi być > 0 (podano: %d)\n", cfg->T_K);
        return -1;
    }
    if (cfg->D_min <= 0) {
        fprintf(stderr, "Błąd: D_min musi być > 0 (podano: %d)\n", cfg->D_min);
        return -1;
    }
    if (cfg->D_max < cfg->D_min) {
        fprintf(stderr, "Błąd: D_max musi być >= D_min (D_min=%d, D_max=%d)\n", cfg->D_min, cfg->D_max);
        return -1;
    }
    if (cfg->E_min <= 0) {
        fprintf(stderr, "Błąd: E_min musi być > 0 (podano: %d)\n", cfg->E_min);
        return -1;
    }
    if (cfg->E_max < cfg->E_min) {
        fprintf(stderr, "Błąd: E_max musi być >= E_min (E_min=%d, E_max=%d)\n", cfg->E_min, cfg->E_max);
        return -1;
    }
    
    return 0;
}
