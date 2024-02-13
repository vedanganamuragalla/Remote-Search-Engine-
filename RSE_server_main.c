#include "server_common.h"
#include "server_file_operations.h"

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char search_word[WORD_LEN];

    printf("Server started.\n");
    log_message(INFO, "Server started.\n");

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        log_message(FATA, "Socket creation failed.");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        log_message(WARNING, "setsockopt failed.");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        log_message(FATA, "Bind failed.\n");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        log_message(FATA, "Listen failed.");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        log_message(FATA, "Accept failed.");
        exit(EXIT_FAILURE);
    }
    log_message(INFO, "Client connected.\n");
    valread = read(new_socket, search_word, WORD_LEN);
    log_message(INFO, "Client sent search word.");
    printf("Client sent: %s\n", search_word);

    char **found_files = NULL;
    int num_found_files = 0;

    search_word_in_directory(".", search_word, &found_files, &num_found_files, new_socket);

    if (num_found_files == 0) {
        const char *no_files_message = "No files were found containing the word.";
        if (send(new_socket, no_files_message, strlen(no_files_message), 0) == -1) {
            perror("Error sending message to client");
        }
        log_message(INFO, "No files found containing the word.");
        close(new_socket); // Close socket if no files found
    } else {
        send_file_list(new_socket, found_files, num_found_files);
        log_message(INFO, "List of files sent to client.");
        // Wait for file selection from client and send file content
        int choice;
        read(new_socket, &choice, sizeof(choice));
        if (choice > 0 && choice <= num_found_files) {
            print_file_content(found_files[choice - 1], new_socket);
        }
    }

    // Free dynamically allocated memory
    for (int i = 0; i < num_found_files; i++) {
        free(found_files[i]);
    }
    free(found_files);

    // Close socket
    close(new_socket);
    close(server_fd);
    return 0;
}
