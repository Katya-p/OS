#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <libgen.h>

void print_attributes(char* path){
    //printf("%s\n", path);
    struct stat sb;
    if (stat(path, &sb) == -1) {
        perror("print_attributes");
        exit(EXIT_FAILURE);
        //sb.st_mode = 0;
    }
    mode_t mode = sb.st_mode;
    switch (mode & S_IFMT) {
    case S_IFDIR:
        printf("d"); break;
    case S_IFREG:
        printf("-"); break;
    default:
        printf("?"); break;
    }

    putchar((mode & S_IRUSR) ? 'r' : '-');
    putchar((mode & S_IWUSR) ? 'w' : '-');
    putchar((mode & S_IXUSR) ? 'x' : '-');
    putchar((mode & S_IRGRP) ? 'r' : '-');
    putchar((mode & S_IWGRP) ? 'w' : '-');
    putchar((mode & S_IXGRP) ? 'x' : '-');
    putchar((mode & S_IROTH) ? 'r' : '-');
    putchar((mode & S_IWOTH) ? 'w' : '-');
    putchar((mode & S_IXOTH) ? 'x' : '-');

    struct passwd* pwd;
    pwd = getpwuid(sb.st_uid);
    printf(" %16s", pwd->pw_name);

    struct group* grp;
    grp = getgrgid(sb.st_gid);
    printf(" %16s", grp->gr_name);

    if((mode & S_IFMT) == S_IFREG){
        printf(" %10ld", sb.st_size);
    }
    else {
        printf(" %10c", ' ');
    }

    printf(" %.24s", ctime(&sb.st_mtime));

    char* real_path = realpath(path, NULL);
    assert(real_path != NULL);
    printf(" %s", basename(real_path));
    free(real_path);

    printf("\n");
}

int main(int argc, char* argv[]){
    for (int i = 1; i < argc; i++){
        print_attributes(argv[i]);
    }
    return EXIT_SUCCESS;
}
