#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1024

int main() {
    FILE *fp;
    char buf[MAX_SIZE];
    if (!(fp = fopen("/proc/tasklist", "r"))) {
        printf("Open File Error!\n");
        exit(-1);
    }
    while (fgets(buf, MAX_SIZE, fp) != NULL) {
        printf("%s", buf);
    }
    return 0;
}
