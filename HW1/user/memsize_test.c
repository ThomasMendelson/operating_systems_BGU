#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    printf("%d\n",memsize());
    void *nonRelevant = malloc(20000*sizeof(char));
    printf("%d\n",memsize());
    free(nonRelevant);
    printf("%d\n",memsize());
    exit(0,0);
}