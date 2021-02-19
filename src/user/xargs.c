#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    int i, j;
    char *cmd, *args[MAXARG], buf[512], *p;

    if (argc < 2)
    {
        fprintf(2, "xargs: parameter error\n");
        exit(1);
    }

    i = 0;
    j = 2;
    cmd = argv[1];

    args[i++] = "";
    for (; j < argc; j++)
        args[i++] = argv[j];

    p = buf;
    while (read(0, p, 1) != 0)
    {
        if (*p != '\n')
        {
            p++;
            continue;
        }

        *p = '\0';
        args[i] = buf;

        if (fork() == 0)
            exec(cmd, args);
        else
        {
            wait(0);
            p = buf;
        }
    }

    exit(0);
}
