#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <ulimit.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/resource.h>

extern char **environ;

void print_user_ids(){
    uid_t euid = geteuid();
    uid_t egid = getegid();
    uid_t uid = getuid();
    uid_t gid = getgid();
    printf("euid: %d, egid: %d, uid: %d, gid: %d\n", euid, egid, uid, gid);
}

void become_leader(){
    printf("becoming a leader\n");
    int rv = setpgid(0, 0);
    if (rv != 0){
        perror(NULL);
        exit(EXIT_FAILURE);
    }
}

void print_process_ids(){
    pid_t pid = getpid();
    pid_t ppid = getppid();
    pid_t pgid = getpgrp();
    printf("pid: %d, ppid: %d, pgid: %d\n", pid, ppid, pgid);
}

void print_ulimit(){
    long limit = ulimit(UL_GETFSIZE, 0);
    if (limit == -1){
        perror(NULL);
        exit(EXIT_FAILURE);
    }
    printf("ulimit: %ld\n", limit);
}

void set_ulimit(){
    char *temp;
    long new_limit = strtol(optarg, &temp, 10);
    if (temp == optarg || *temp != '\0' || errno == ERANGE){
        fprintf(stderr, "bad new limit %s\n", optarg);
        exit(EXIT_FAILURE);
    }
    printf("setting ulimit to: %ld\n", new_limit);
    long limit = ulimit(UL_SETFSIZE, new_limit);
    if (limit == -1){
        perror("set_ulimit");
        exit(EXIT_FAILURE);
    }
}

void print_core_limit(){
    struct rlimit rlim;
    if (getrlimit(RLIMIT_CORE, &rlim) != 0){
        perror("print_core_limit");
        exit(EXIT_FAILURE);
    }
    printf("soft rlimit: %lu, hard rlimit: %lu \n", rlim.rlim_cur, rlim.rlim_max);
}

void set_core_limit(){
    char *temp;
    long new_limit = strtol(optarg, &temp, 10);
    if (temp == optarg || *temp != '\0' || errno == ERANGE
            || new_limit < 0){
        fprintf(stderr, "bad new core limit %s\n", optarg);
        exit(EXIT_FAILURE);
    }
    printf("setting core limit to: %ld\n", new_limit);
    struct rlimit rlim;
    if (getrlimit(RLIMIT_CORE, &rlim) != 0){
        perror("set_core_limit");
        exit(EXIT_FAILURE);
    }
    rlim.rlim_cur = (rlim_t)new_limit;
    if (setrlimit(RLIMIT_CORE, &rlim) != 0){
        perror("set_core_limit");
        exit(EXIT_FAILURE);
    }
}

void print_cwd(){
    size_t path_size = PATH_MAX;
    //size_t path_size = 1; //for debug
    char* path;
    while (1){
        path = malloc(path_size);
        if (path == NULL){
            fprintf(stderr, "malloc\n");
            exit(EXIT_FAILURE);
        }
        if (getcwd(path, path_size) != NULL){
            break;
        }
        if (errno != ERANGE){
            perror("print_cwd");
            free(path);
            exit(EXIT_FAILURE);
        }
        free(path);
        path = NULL;
        path_size = path_size * 2;
    }
    printf("cwd: %s\n", path);
    free(path);
}

void print_env(){
    printf("env:\n");
    char** s = environ;
    while (*s != NULL){
        printf("\t%s\n", *s);
        s++;
    }
}

void set_env(){
    printf("set env %s\n", optarg);
    if (putenv(optarg) != 0){
        perror("set_env");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    while (1){
        int opt = getopt(argc, argv, "ispuU:cC:dvV:qw");
        if (opt == -1){
            break;
        }
        switch (opt){
        case 'i':
            print_user_ids();
            break;
        case 's':
            become_leader();
            break;
        case 'p':
            print_process_ids();
            break;
        case 'u':
            print_ulimit();
            break;
        case 'U':
            set_ulimit();
            break;
        case 'c':
            print_core_limit();
            break;
        case 'C':
            set_core_limit();
            break;
        case 'd':
            print_cwd();
            break;
        case 'v':
            print_env();
            break;
        case 'V':
            set_env();
            break;
        case 'q': abort(); break;
        case 'w': while(1){} break;
        default:
            return EXIT_FAILURE;
        }
    }
    if (optind != argc){
        fprintf(stderr, "unsupported positional arguments\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
