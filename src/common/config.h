#ifndef CONFIG_H
#define CONFIG_H

/**
 * Parametry symulacji mrowiska.
 * Wszystkie wartości są walidowane przy parsowaniu.
 */
typedef struct {
    int M;          // początkowa liczba mrówek (M >= 3)
    int K;          // pojemność spichlerza (1 <= K < M/2)
    int G;          // limit populacji gniazda (G >= M)
    int T_c;        // timeout wejścia [s] (T_c > 0)
    int T_ink;      // okres inkubacji [s] (T_ink > 0)
    int T_K;        // czas po zamknięciu szybów [s] (T_K > 0)
    int D_min;      // min czas w spichlerzu [s]
    int D_max;      // max czas w spichlerzu [s] (D_min <= D_max)
    int E_min;      // min liczba cykli
    int E_max;      // max liczba cykli (E_min <= E_max)
} Config;

/**
 * Parsuje argumenty linii poleceń do struktury Config.
 * 
 * @param argc liczba argumentów
 * @param argv tablica argumentów
 * @param cfg wskaźnik na strukturę do wypełnienia
 * @return 0 przy sukcesie, -1 przy błędzie (komunikat na stderr)
 */
int config_parse(int argc, char *argv[], Config *cfg);

#endif // CONFIG_H