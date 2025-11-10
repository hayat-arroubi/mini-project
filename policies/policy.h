
#ifndef POLICY_H
#define POLICY_H

#include "../include/process.h"

void fifo(Process* processes, int count);
void sjf(Process* processes, int count);
void rr(Process* processes, int count, int quantum);

#endif
