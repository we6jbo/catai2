#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define FLAG_FILE "/tmp/iefisj.txt"
#define SERVER_IP "192.168.5.146"
#define SERVER_PORT 4495
#define BUFFER_SIZE 1024

int main(void) {
    int fd;

    /* 1. Check if the file exists */
    fd = open(FLAG_FILE, O_RDONLY);
    if (fd >= 0) {
        /* File exists → exit */
        close(fd);
        return 0;
    }

    /* File does not exist → create it */
    fd = open(FLAG_FILE, O_CREAT | O_WRONLY, 0644);
    if (fd < 0) {
        perror("Failed to create flag file");
        return 1;
    }
    close(fd);

    /* 2. Create socket */
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock);
        return 1;
    }

    /* 3. Connect */
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        close(sock);
        return 1;
    }

    /* 4. Send "Hello" */
    const char *hello = "Hello";
    if (send(sock, hello, strlen(hello), 0) < 0) {
        perror("Send failed");
        close(sock);
        return 1;
    }

    /* 5. Receive response */
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes < 0) {
        perror("Receive failed");
        close(sock);
        return 1;
    }

    /* 6. Print response */
    printf("I received: %s\n", buffer);

    /* 7. Quit if response starts with "quit" */
    if (strncmp(buffer, "quit", 4) == 0) {
        close(sock);
        return 0;
    }

    close(sock);
    return 0;
}
