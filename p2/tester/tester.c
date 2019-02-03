#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "tester.h"

#define __NR_procAncestry 378

int main(int argc, char **argv) {
    if(argc == 1) {
        unsigned short pid = getpid();
        long result = getAncestry(pid);
        printf("PID:%d %30s %ld\n", pid, "Return Code from procAncestry", result);
    } else {
        int i;
        for(i = 1; i < argc; i++) {
            unsigned short pid = atoi(argv[i]);
            long result = getAncestry(pid);
            printf("PID:%d %30s %ld\n", pid, "Return Code from procAncestry", result);
        }
    }
    return 0;
}

long getAncestry(unsigned short pid) {
    struct ancestry *data;
    data = malloc(sizeof(struct ancestry));
    clearAncestry(data);
    long result = syscall(__NR_procAncestry, &pid, data);
    printArray(data->ancestors, 10, pid, "ancestor");
    printArray(data->siblings, 100, pid, "sibling");
    printArray(data->children, 100, pid, "child");
    return result;
}

void printArray(pid_t *list, int length, unsigned short pid, char *type) {
    int i;
    for(i = 0; i < length; i++) {
        if(list[i] != 0) {
            printf(" %15d is %s of %d\n", list[i], type, pid); 
        }
    }
}

void clearAncestry(struct ancestry *data) {
    int i;
    for(i = 0; i < 10; i ++) {
        data->ancestors[i] = 0;
    }
    for(i = 0; i < 100; i++) {
        data->children[i] = 0;
    }
    for(i = 0 ;i <100; i++) {
        data->siblings[i] = 0;
    }
}