#include "register.h"

unsigned char length = 0;
void (*initfunclist[255])(void);  // array of 255 function pointers taking void and returning void
void (*cleanfunclist[255])(void); // same here
const char *names[255];

void registermodule(const char *name, void (*init)(void), void (*stopandclean)(void))
{
    initfunclist[length] = init;
    cleanfunclist[length] = stopandclean;
    names[length] = name;
    length++;
}

void play(char idx)
{
    if (initfunclist[idx])
    {
        initfunclist[idx]();
    }
}

void stop(char idx)
{
    if (cleanfunclist[idx])
    {
        cleanfunclist[idx]();
    }
}

char exists(char idx) {
    return cleanfunclist[idx] && initfunclist[idx];
}