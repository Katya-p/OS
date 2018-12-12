#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fnmatch.h>

char* fix_pattern(char* pattern){
    char* result = malloc(strlen(pattern) * 2);
    char* j = result;
    for (char* i = pattern; *i != '\0'; i++){
        char c = *i;
        if (c == '['){
            *j++ = '\\';
            *j++ = '[';
        }
        else if (c == '\\'){
            *j++ = '\\';
            *j++ = '\\';
        }
        else if (c == '/'){
            fprintf(stderr, "invalid pattern");
            exit(EXIT_FAILURE);
        }
        else {
            *j++ = c;
        }
    }
    *j = '\0';
    return result;
}

int matches(char* pattern, char* s){
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

void print_dir(char* dir_path, char* pattern){
    DIR* dir = opendir(dir_path);
    if (dir == NULL){
        perror("print_dir");
        exit(EXIT_FAILURE);
    }
    errno = 0;
    while (1){
        struct dirent *entry = readdir(dir);
        if (entry == NULL){
            if (errno != 0){
                perror("print_dir");
                exit(EXIT_FAILURE);
            }
            break;
        }
        if (matches(pattern, entry->d_name) == 0){
            printf("%s\n", entry->d_name);
        }
    }
    closedir(dir);
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
