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
    const char* i;
    for (i = pattern; *i != '\0'; i++){
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

int matches(const char* fixed_pattern, const char* s){
    int result = fnmatch(fixed_pattern, s, FNM_PATHNAME);
    if (result == 0){
        return 0;
    }
    if (result == FNM_NOMATCH){
        return 1;
    }
    perror("matches");
    exit(EXIT_FAILURE);
}

int search(char* dir_path, char* pattern){
    char* slash = strchr(pattern, '/');
    if (slash != NULL){
        *slash = '\0';
    }
    char* fixed_pattern = fix_pattern(pattern);
    if (fixed_pattern == NULL){
        fprintf(stderr, "malloc\n");
        exit(EXIT_FAILURE);
    }
    DIR* dir = opendir(dir_path);
    if (dir == NULL){
        perror(dir_path);
        return 0;
    }
    int found = 0;
    while (1){
        errno = 0;
        struct dirent *entry = readdir(dir);
        if (entry == NULL){
            if (errno != 0){
                perror(dir_path);
            }
            break;
        }
        char* name = entry->d_name;
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0){
            continue;
        }
        if (matches(fixed_pattern, name) != 0){
            continue;
        }
        size_t dir_path_length = strlen(dir_path);
        size_t name_length = strlen(name);
        char* child_path = (char*)malloc(dir_path_length + 1 + name_length + 1);
        if (child_path == NULL){
            fprintf(stderr, "malloc\n");
            exit(EXIT_FAILURE);
        }
        memcpy(child_path, dir_path, dir_path_length);
        child_path[dir_path_length] = '/';
        strcpy(child_path + dir_path_length + 1, name);
        struct stat sb;
        if (stat(child_path, &sb) != 0){
            perror(child_path);
            free(child_path);
            continue;
        }
        if (slash == NULL){
            if (1 || S_ISREG(sb.st_mode)){
                char* res = (char*)malloc(strlen(dir_path) + strlen(name) + 2);
                strcpy(res, dir_path);
                res[strlen(dir_path)] = '/';
                strcpy(res + strlen(dir_path) + 1, name);
                if ((*res == '.') && (*(res+1) == '/') ){
                    printf("%s\n", res + 2);
                } else {
                    printf("%s\n", res + 1);
                }
                free(res);
                found++;
            }
            free(child_path);
            continue;
        }
        if (!S_ISDIR(sb.st_mode)){
            free(child_path);
            continue;
        }
        found += search(child_path, slash + 1);
        free(child_path);
    }
    closedir(dir);
    if (slash != NULL){
        *slash = '/';
    }
    free(fixed_pattern);
    return found;
}

int main(int argc, char* argv[]){
    if (argc != 2){
        fprintf(stderr, "usage: %s PATTERN\n", argv[0]);
        return EXIT_FAILURE;
    }
    char* pattern = argv[1];
    if (*pattern == '/') {
        if (search("/", pattern + 1) == 0){
           printf("%s\n", pattern);
        }
    } else {
        if (search(".", pattern) == 0){
            printf("%s\n", pattern);
        }
    }    
    return EXIT_SUCCESS;
}
