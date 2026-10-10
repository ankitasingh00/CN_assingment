#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8081

int main() {
    int s, n;
    char b[1024];
    struct sockaddr_in a = {0};

    s = socket(AF_INET, SOCK_STREAM, 0);
    a.sin_family = AF_INET;
    a.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &a.sin_addr);
    connect(s, (struct sockaddr *)&a, sizeof(a));

    while (1) {
        printf("Enter request: ");
        fgets(b, sizeof(b), stdin);
        b[strcspn(b, "\n")] = 0;
        send(s, b, strlen(b), 0);

        if (!strcmp(b, "bye")) break;

        n = read(s, b, sizeof(b)-1);
        if (n <= 0) break;
        b[n] = '\0';
        printf("%s\n", b);
    }

    close(s);
    return 0;
}
