#ifndef LOG_H
#define LOG_H

/**
 * Otwiera plik logu.
 * Tworzy katalog logs/ jeśli nie istnieje.
 * 
 * @param path ścieżka do pliku logu (np. "logs/simulation.log")
 * @return 0 przy sukcesie, -1 przy błędzie (komunikat przez perror)
 */
int log_open(const char *path);

/**
 * Zapisuje linię do logu.
 * Format: [HH:MM:SS] [role] pid=PID: message
 * 
 * @param role nazwa roli (np. "anthill", "mother", "ant")
 * @param fmt format printf
 * @param ... argumenty
 */
void log_write(const char *role, const char *fmt, ...);

/**
 * Zamyka plik logu.
 */
void log_close(void);

#endif // LOG_H