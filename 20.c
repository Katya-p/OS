#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fnmatch.h>
#include <ftw.h>

char* fix_pattern(const char* pattern){
    char* result = malloc(strlen(pattern) * 2 + 1);
    char* j = result;
    for (const char* i = pattern; *i != '\0'; i++){
        char c = *i;
        if (c == '['){
            *j++ = '\\';
            *j++ = '[';
        }
        else if (c == '\\'){
            *j++ = '\\';
            *j++ = '\\';
        }
        else {
            *j++ = c;
        }
    }
    *j = '\0';
    return result;
}

int matches(const char* pattern, const char* s){
    char* fixed = fix_pattern(pattern);
    int result = fnmatch(fixed, s, FNM_PATHNAME);
    free(fixed);
    if (result == 0){
        return 0;
    }
    if (result == FNM_NOMATCH ){
        return 1;
    }
    perror("matches");
    exit(EXIT_FAILURE);
}

static char* global_pattern;
static int dir_path_length;

int process(const char *file, const struct stat *sb, int flag){
    const char* path = file + dir_path_length + 1;
    if (*path == 0){
        return 0;
    }
    if (matches(global_pattern, path) == 0){
        printf("%s\n", path);
    }
    //printf("%s %d %d\n", path, flag, s->base);
    return 0;
}

void print_dir(char* dir_path, char* pattern){
    global_pattern = pattern;
    dir_path_length = (int)strlen(dir_path);
    if (ftw(dir_path, process, 20) == -1){
        perror("print_dir");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[]){
    //printf("%d", matches("a*g", "asd/gfg"));
    if (argc != 2){
        fprintf(stderr, "usage: %s PATTERN\n", argv[0]);
        return EXIT_FAILURE;
    }
    print_dir(".", argv[1]);
    return EXIT_SUCCESS;
}
