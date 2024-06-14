#include <kernel/types.h>
#include <kernel/stat.h>
#include <user/user.h>


int
main(int argc, char* argv[]){
    int cd = channel_create();
    if (cd < 0) {
        printf("Failed to create channel\n");
        exit(1);
    }
    if (fork() == 0) {
        if (channel_put(cd, 42) < 0) {
            printf("Failed to put data in channel\n");
            exit(1);
        }
        if (channel_put(cd, 43) < 0) // Sleeps until cleared
            printf("Handle error 1\n"); // Handle error
        if (channel_destroy(cd) < 0)
            printf("Handle error 2\n"); // Handle error
    } else {
        int data;
        if (channel_take(cd, &data) < 0) { // 42
            printf("Failed to take data from channel\n");
            exit(1);
        }
        if (channel_take(cd, &data) < 0) // 43
            printf("Handle error 3\n"); // Handle error
        if (channel_take(cd, &data) < 0) // Sleep until child destroys channel
            printf("goodddddddddd\n"); // Handle error
    }
    return 0;
}


