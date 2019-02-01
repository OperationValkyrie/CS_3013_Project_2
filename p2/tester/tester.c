#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "tester.h"

struct ancestry {
    pid_t ancestors[10];
    pid_t siblings[100];
    pid_t children[100];
};

#define __NR_procAncestry 378

int main(int argc, char **argv) {
    if(argc == 0) {
        argc = 1;
        sprintf(argv[0], "%d", getpid());
    } else {
        int i;
        for(i = 0; i < argc; i++) {
            pid_t pid = atoi(argv[i]);
            printf("PID:%d %30s %ld\n", pid, "Return Code from procAncestry", getAncestry(pid));
        }
    }
    return 0;
}

long getAncestry(pid_t pid) {
    struct ancestry *data;
    data = malloc(sizeof(struct ancestry));
    long result = syscall(__NR_procAncestry, pid, data);
    return result;
}