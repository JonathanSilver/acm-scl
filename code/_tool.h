#ifndef _TOOL_H
#define _TOOL_H

#include <cstdlib>

using namespace std;

int times = 0;
int times_of_new = 0;
int times_of_delete = 0;

void * operator new(size_t n)
{
    times++;
    times_of_new++;
    return malloc(n);
}

void operator delete(void * p)
{
    times--;
    times_of_delete++;
    free(p);
}

#endif // _TOOL_H
