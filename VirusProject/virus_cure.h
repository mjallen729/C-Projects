#ifndef HEADER
#define HEADER

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>

void* phonecall(void *arg);

int main(int argc, char const *argv[]);

#endif
