#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

int main() {
    DIR *dp;
    struct dirent *entry;

    if(NULL == (dp = opendir("."))) {
        printf("error.\n");
    }

    printf("%20s%10s%10s%10s\n", "name", "inode", "len", "offset");

    while(entry = readdir(dp)) {
        printf("%20s%10ld%10d%10ld\n", entry->d_name, entry->d_ino, entry->d_reclen, entry->d_namlen);
    }
    closedir(dp);

    return 0;
}