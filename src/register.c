#include "register.h"
#include "audio_driver.h"

unsigned char length = 0;
void (*initfunclist[255])(void);  // array of 255 function pointers taking void and returning void
void (*cleanfunclist[255])(void); // same here
const char *names[255];
const char *typelist[255];

void registermodule(const char *name, void (*init)(void), void (*stopandclean)(void), const char *type)
{
    initfunclist[length] = init;
    cleanfunclist[length] = stopandclean;
    names[length] = name;
    typelist[length] = type;
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
    panic_sound();
}

char exists(char idx) {
    return cleanfunclist[idx] && initfunclist[idx];
}