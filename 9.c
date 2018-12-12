#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static char* some_string = "some string";
int main(int argc, char* argv[])
{
    printf("Parent: %s\n", some_string);
//    intptr_t sz2 = getpagesize();
    long sz = sysconf(_SC_PAGESIZE);
    char* aligned = (char*)(((intptr_t) some_string) / sz * sz);
    if (mprotect(aligned, 1, PROT_READ | PROT_WRITE | PROT_EXEC) == -1)
    {
        perror(NULL);
        return 1;
    }
    some_string[0] = 'Z';

    printf("Parent: %s\n", some_string);
/*
    char* shared = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (shared == MAP_FAILED){
        perror(NULL);
        return 1;
    }
    strcpy(shared, "shared string");
    printf("Parent: %s\n", shared);
*/
    printf("Parent: before\n");
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Parent: after (error)");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        some_string[0] = '!';

        printf("Child: %s\n", some_string);
        //strcpy(shared, "lol zZZZ shared string");
        //printf("Child: %s\n", shared);
        char* path = NULL;
        if (argc > 1){
            path = argv[1];
        }
        char* argv1[] = {"cat", path, NULL};

//        if(execvp("cat", argv1) == -1)
//        {
//            perror("Child: in process (error)");
//            exit(EXIT_FAILURE);
//        }
    }
    else
    {
        printf("Parent: in process\n");

        if (waitpid(pid, NULL, 0) == -1)
        {
            perror("Parent: in process (error)");
            exit(EXIT_FAILURE);
        }
        printf("Parent: %s\n", some_string);
        //printf("Parent: %s\n", shared);
        printf("Parent: after\n");
        exit(EXIT_SUCCESS);
    }
}

