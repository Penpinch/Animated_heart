# include <stdio.h>
# include "shapes.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main(){

    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    draw_heart(0);
    printf("\n");

    draw_heart(1);
    printf("\n");

    draw_heart(2);
    printf("\n");

    return 0;
}