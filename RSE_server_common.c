#include "server_common.h"

void log_message(int level, const char *message) {
    switch (level) {
        case FATA:
            printf("[FATAL]: %s\n", message);
            break;
        case INFO:
            printf("[INFO]: %s\n", message);
            break;
        case WARNING:
            printf("[WARNING]: %s\n", message);
            break;
        case DEBUG:
            printf("[DEBUG]: %s\n", message);
            break;
        default:
            printf("[UNKNOWN]: %s\n", message);
            break;
    }
}
