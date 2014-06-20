#ifndef MEMORY_H
#define MEMORY_H

typedef struct MemBloc MemBloc;

struct MemBloc
{
    MemBloc *previous;
    int memorySize;
    int occupy; //True(1) or false(0)
    MemBloc *next;
};

typedef struct Memory Memory;

struct Memory
{
    MemBloc *first;
    int totalMemory;
    //1=First-fit, 2=Best-fit, 3=Worst-fit, 4=next-fit
    int strategy;
};

#endif
