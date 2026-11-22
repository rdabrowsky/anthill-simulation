#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_LOG_LINE 512

static int log_fd = -1;

int log_open(const char *path) {
    // Utwórz katalog logs/ jeśli nie istnieje
    mkdir("logs", 0755);  // ignoruj błąd jeśli istnieje
    
    // Otwórz plik z O_APPEND (atomowe dopisywanie)
    log_fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd == -1) {
        perror(path);
        return -1;
    }
    
    return 0;
}

void log_write(const char *role, const char *fmt, ...) {
    if (log_fd == -1) return;
    
    char buffer[MAX_LOG_LINE];
    int offset = 0;
    
    // Timestamp [HH:MM:SS]
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    offset += strftime(buffer, sizeof(buffer), "[%H:%M:%S] ", tm_info);
    
    // [role] pid=PID:
    offset += snprintf(buffer + offset, sizeof(buffer) - offset,
                       "[%s] pid=%d: ", role, getpid());
    
    // Wiadomość użytkownika
    va_list args;
    va_start(args, fmt);
    offset += vsnprintf(buffer + offset, sizeof(buffer) - offset, fmt, args);
    va_end(args);
    
    // Newline
    if (offset < (int)sizeof(buffer) - 1) {
        buffer[offset++] = '\n';
    }
    
    // Atomowy zapis (O_APPEND gwarantuje że pozycja jest na końcu)
    write(log_fd, buffer, offset);
}

void log_close(void) {
    if (log_fd != -1) {
        close(log_fd);
        log_fd = -1;
    }
}