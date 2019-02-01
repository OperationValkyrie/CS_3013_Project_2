#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "tester.h"

#define __NR_read 3
#define __NR_open 5
#define __NR_close 6
#define __NR_helloworld 377

#define O_RDONLY 00

int main(int argc, char **argv) {
    printf("%30s %ld\n\n", "Return Code from helloworld", helloworld());
    printf("%s\n", "test.txt");
    printf("%30s %ld\n", "Return Code from open", openFile("test.txt"));
    printf("%30s %ld\n", "Return Code from read", readFile());
    printf("%30s %ld\n\n", "Return Code from close", closeFile());
    printf("%s\n", "virus.txt");
    printf("%30s %ld\n", "Return Code from open", openFile("virus.txt"));
    printf("%30s %ld\n", "Return Code from read", readFile());
    printf("%30s %ld\n", "Return Code from close", closeFile());
    return 0;
}

long helloworld() {
    return (long) syscall(__NR_helloworld);
}

long openFile(char *filename) {
    test_file = syscall(__NR_open, filename, O_RDONLY);
    return test_file;
}

long readFile() {
    if(!test_file) {
        return -1;
    }
    char buf[1024];
    ssize_t count = 1024;
    long result = syscall(__NR_read, test_file, buf, count);
    //long result = read(test_file, buf, count);
    return result;
}

long closeFile() {
    return syscall(__NR_close, test_file);
}