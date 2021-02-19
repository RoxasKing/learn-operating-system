#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void process(int fd);

int main()
{
    int num, p[2];

    pipe(p);

    if (fork() == 0)
    {
        close(p[0]);

        for (num = 2; num <= 35; num++)
        {
            write(p[1], &num, 4);
        }

        num = -1;
        write(p[1], &num, 4);

        close(p[1]);
    }
    else
    {
        close(p[1]);

        process(p[0]);
    }

    exit(0);
}

void process(int fd)
{
    int prime, num, p[2];

START:

    read(fd, &prime, 4);
    if (prime == -1)
    {
        close(fd);

        exit(0);
    }

    printf("prime %d\n", prime);

    pipe(p);

    if (fork() == 0)
    {
        close(p[0]);
        while (1)
        {
            read(fd, &num, 4);

            if (num == -1)
            {
                break;
            }

            if (num % prime)
            {
                write(p[1], &num, 4);
            }
        }
        close(fd);

        write(p[1], &num, 4);
        close(p[1]);
    }
    else
    {
        close(fd);
        close(p[1]);

        fd = p[0];

        goto START;
    }
}
