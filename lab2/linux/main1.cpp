//
// Created by oem on 3/6/18.
//

#include <sys/types.h>
#include <dirent.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
void readInDepth(int depth , const char * path) {
    struct dirent *entry;

    if (!(DIR * dir = opendir(path)))
        return;

    printf("\n");
    for (int i = 0; i < depth; i++)
        printf("%t");


    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        if (entry->d_type == DT_DIR) {
            readInDepth(depth + 1,strcat(path,entry->d_name));
        }
        else {
            printf("%s",entry->d_name);
        }
    }

}
int main() {
    readInDepth(0,".");
}