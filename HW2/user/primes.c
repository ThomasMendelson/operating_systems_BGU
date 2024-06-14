#include <kernel/types.h>
#include <kernel/stat.h>
#include <user/user.h>

int i_need_to_finish(){
    char answer[3];
    printf("Do you wany to play again? (Y/N)");
    gets(answer, 3);
    if ((strcmp(answer,"Y\n") == 0) || (strcmp(answer,"y\n") == 0))
        return 0;
    return 1;
}

void prime_number_gen(int cd1, int num_of_checker){
    int gen = 2;
    int status;
    while(channel_put(cd1, gen) == 0)
        gen++;
    for(int i = 0; i < num_of_checker + 1; i++)
        wait(&status);
}

int check_prime(int data){
    for (int i= 2; i*i <= data; i++){
        if (data % i == 0)
            return 0;
    }
    return 1;
}

void prime_number_checker(int cd1, int cd2){
    int data;
    while (channel_take(cd1, &data) == 0) {
        int is_prime = check_prime(data);
        if (is_prime)
            if (channel_put(cd2, data) < 0){
                printf("process %d (checker)found channel 2 is destroyed and exited\n",getpid());
                channel_destroy(cd1);
                exit(0);
            }
    }
    printf("process %d (Checker)found channel 1 is destroyed and exited\n",getpid());
    exit(0);
}

void prime_number_printer(int cd2){
    int counter = 0;
    int data;
    while (counter < 100){
        if (channel_take(cd2, &data) != 0){
            printf("something got wrong coudnt get data from channel 2\n");
            exit(1);
        }
        printf("found: %d\n", data);
        counter++;
    }
    printf("process %d (printer)found 100 prime numbers and destroying  channel 2 and exited\n",getpid());
    channel_destroy(cd2);
    exit(0);
}

int
main(int argc, char* argv[]){
    int cd1;
    int cd2;

    int num_of_checker = 3;
    if (argc > 2){
        printf("wrong number of arguments\n");
        exit(1);
    }
    if (argc > 1)
        if((num_of_checker = atoi(argv[1]))==0){
            printf("wrong number of checker\n");
            exit(1);
        }
    int finish = 0;
    while(!finish){
        if ((cd1 = channel_create()) < 0) {
            printf("Failed to create channel 1\n");
            exit(1);
        }

        if ((cd2 = channel_create()) < 0) {
            printf("Failed to create channel 2\n");
            exit(1);
        }

        if (fork() != 0){
            for(int i = 0; i < num_of_checker; i++)
                if (fork() == 0)
                    prime_number_checker(cd1, cd2);
        }   
        else
            prime_number_printer(cd2);

        prime_number_gen(cd1, num_of_checker);
        finish = i_need_to_finish();
    }
    printf("main process is exting\n");
    return 0;
}