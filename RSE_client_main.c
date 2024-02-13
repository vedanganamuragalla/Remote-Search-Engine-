#include "client_common.h"
#include "client_file_operations.h"

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char search_word[WORD_LEN] = {0};
    char buffer[MAX_LEN] = {0};
    int choice = 0;

    printf("******REMOTE SEARCH ENGINE*****\n\n");
    log_message(INFO, "Client started.\n");

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        log_message(FATA, "Socket creation error.");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        log_message(FATA, "Invalid address/ Address not supported.");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        log_message(FATA, "Connection failed.\n");
        exit(EXIT_FAILURE);
    }
    
    log_message(INFO, "Connected to server.\n");

    do {
        // Display menu
        printf("Menu:\n");
        printf("1. Enter a word to search for\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline left in the input buffer after scanf
        log_message(DEBUG, "Choice entered.");

        switch (choice) {
            case 1:
                // Get search word from user
                printf("Kindly input a word to search:\n ");
                fgets(search_word, WORD_LEN, stdin);
                search_word[strcspn(search_word, "\n")] = '\0';

                // Send search word to server
                send(sock, search_word, strlen(search_word), 0);
               
                log_message(INFO, "Word sent to server.\n");

                // Read response from server
                valread = read(sock, buffer, MAX_LEN);
                if (valread < 0) {
                    perror("Error reading response from server");
                    log_message(FATA, "Error reading response from server.");
                    exit(EXIT_FAILURE);
                }

                // Print out the list of files received from the server
                printf("List of files received from the server:\n%s\n\n", buffer);
               
                // Ask user to select a file
                printf("Enter the number of the file you want to view, or 0 to exit: ");
                scanf("%d", &choice);
                log_message(DEBUG, "File selection entered.\n");

                // Send selected file index to server
                send(sock, &choice, sizeof(choice), 0);

                // Display content of the selected file
                if (choice > 0) {
                    display_file_content(sock);
                }
                break;
            case 2:
                // Exit
                send(sock, &choice, sizeof(choice), 0); // Send choice to server
                log_message(INFO, "Exiting...");
                break;
            default:
                log_message(WARNING, "Invalid choice. Please try again.");
                break;
        }
    } while (choice != 2);

    // Close socket
    close(sock);
    log_message(INFO, "Socket closed.");

    return 0;
}
