// Joanthan Chang
// CS 3013 C02
// Project 2

#ifndef TESTER_H_
#define TESTER_H_

struct ancestry {
    pid_t ancestors[10];
    pid_t children[100];
    pid_t siblings[100];
};

long getAncestry(unsigned short pid);
void printArray(pid_t *list, int length, unsigned short pid, char *type);
void clearAncestry(struct ancestry *data);

#endif