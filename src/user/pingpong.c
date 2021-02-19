#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    char buf[512];
    int p1[2], p2[2], n;

    pipe(p1);
    pipe(p2);

    if (fork() == 0)
    {
        close(p1[1]);
        n = read(p1[0], buf, sizeof(buf));
        close(p1[0]);

        buf[n] = '\0';
        printf("%d: received %s\n", getpid(), buf);

        close(p2[0]);
        write(p2[1], "pong", 5);
        close(p2[1]);

        exit(0);
    }
    else
    {
        close(p1[0]);
        write(p1[1], "ping", 5);
        close(p1[1]);

        close(p2[1]);
        n = read(p2[0], buf, sizeof(buf));
        close(p2[0]);

        buf[n] = '\0';
        printf("%d: received %s\n", getpid(), buf);

        exit(0);
    }
}
