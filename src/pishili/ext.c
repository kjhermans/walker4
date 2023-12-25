#include <stdio.h>
#include <stdlib.h>

extern void
sys_kill(void);

extern void *
EXT_calloc(unsigned n, unsigned esz)
{
    return calloc(n, esz);
}

extern void
EXT_free(void *p)
{
    free(p);
}

extern void
EXT_error(int err_id, char *modname, char *msg)
{
    printf("vx error 0x%x in %s: %s\n", err_id, modname, msg);
    sys_kill();
    getchar();
    exit(0);
}

