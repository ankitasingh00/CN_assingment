#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8081

int count_words(char *s) {
    int c = 0, w = 0;
    while (*s) {
        if (*s == ' ' || *s == '\n')
            w = 0;
        else if (!w) {
            c++;
            w = 1;
        }
        s++;
    }
    return c;
}

int count_vowels(char *s) {
    int c = 0;
    while (*s) {
        if (strchr("aeiouAEIOU", *s))
            c++;
        s++;
    }
    return c;
}

int main() {
    int s, n;
    char b[1024], cmd[20], t[1000], r[100];
    struct sockaddr_in a = {0};

    s = socket(AF_INET, SOCK_STREAM, 0);
    a.sin_family = AF_INET;
    a.sin_port = htons(PORT);
    a.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr *)&a, sizeof(a));
    listen(s, 1);
    n = accept(s, NULL, NULL);

    while ((n = read(n, b, sizeof(b)-1)) > 0) {
        b[n] = '\0';
        b[strcspn(b, "\r\n")] = 0;

        if (!strcmp(b, "bye")) break;

        if (sscanf(b, " %19[^|]| %999[^\n]", cmd, t) == 2
            && !strcmp(cmd, "ANALYZE")) {
            sprintf(r, "Chars=%d, Words=%d, Vowels=%d",
                    (int)strlen(t), count_words(t),
                    count_vowels(t));
        } else {
            strcpy(r, "Invalid Command");
        }

        send(n, r, strlen(r), 0);
    }

    close(s);
    return 0;
}
