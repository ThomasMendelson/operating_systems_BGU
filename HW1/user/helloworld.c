#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    char* Hello_World_xv6 = "Hello World xv6\n";
    write(1, Hello_World_xv6, strlen(Hello_World_xv6));
    exit(0,0);
}