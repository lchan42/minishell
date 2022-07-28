#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

int built_pwd(void)
{
    char buf[PATH_MAX + 1];

    if (!getcwd(buf, PATH_MAX))
    {
        perror("getcwd");
        return (1);
    }
    else
    {
        printf("%s\n", buf);
        return (0);
    }
}
