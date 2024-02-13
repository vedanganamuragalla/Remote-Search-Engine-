/* SERVER_COMMON_H */

#ifndef SERVER_COMMON_H
#define SERVER_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 4343
#define WORD_LEN 256
#define MAX_LEN 1000

#define FATA 0
#define INFO 1
#define WARNING 2
#define DEBUG 3

void log_message(int level, const char *message);

#endif 
