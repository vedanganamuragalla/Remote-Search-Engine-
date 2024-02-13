#include "client_file_operations.h"

void display_file_content(int sock) {
    char buffer[MAX_LEN];
    int valread;

    // Read file content from server
    while ((valread = read(sock, buffer, MAX_LEN)) > 0) {
        printf("%s", buffer);
    }
    if (valread < 0) {
        perror("Error reading file content from server");
        log_message(FATA, "Error reading file content from server.");
    }
}
