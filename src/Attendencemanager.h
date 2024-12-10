#ifndef ATTENDENCEMANAGER_H
#define ATTENDENCEMANAGER_H


#include <stdio.h>

#define MAX_ENTRIES 1000  // Maximum number of entries

 // Number of enrolled numbers

int attendencemanager_check(int num);
void attendencemanager_add(int num);
void attendencemanager_remove(int num);
int attendencemanager_auth();

#endif