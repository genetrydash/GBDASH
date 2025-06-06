#include "driverhelper.h"

char nmode = 0;
char macrolist [20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char lengthen [4] = {0,0,0,0};

void clearmacrosforchannel(char channel) {
    int start_index = channel * 5;
    macrolist[start_index] = 0;
    macrolist[start_index + 1] = 0;
    macrolist[start_index + 2] = 0;
    macrolist[start_index + 3] = 0;
    macrolist[start_index + 4] = 0;
}