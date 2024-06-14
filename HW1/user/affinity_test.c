#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int mask = 0;
    set_affinity_mask(mask);
    printf("after mast set to: %d\n",mask);
    printf("getPid: %d\n",getpid());   
    for (int i = 0; i<30 ; i++){
        int chiledpid;
        if((chiledpid = fork()) == 0){
            exit(0,0);
        }
        else{
            wait(0,0);
            printf("getPid: %d\n",getpid());   
        }
    }
    exit(0,0);
}