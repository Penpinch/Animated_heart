# include <stdbool.h>
# include "animation.h"
# include "shapes.h"

# ifdef _WIN32
    # include <windows.h>
# else 
    # include <unistd.h> // For macOS and Linux.
# endif

static void sleep_ms(int ms){
    # ifdef _WIN32
        Sleep(ms);
    # else
        usleep(ms * 1000);
    # endif
}

void animationInit(Animation *animation, int total_chars){
    animation->frame = 0;
    animation->pulse = SMALL;
    animation->revealed_chars = 0;
    animation->total_chars = total_chars;
    animation->typing_delay = 2;
    animation->pulse_delay = 0;
    animation->finished = false;
}

void animationUpdate(Animation *animation){
    sleep_ms(35);
    animation->frame++;

    if(animation->frame % 12 == 0){ // --- PULSE ---
        animation->pulse++;
        if(animation->pulse > 2){ animation->pulse = 0; }
    }

    animation->typing_delay++; // --- DELAY ---
    if(animation->typing_delay >= 2){
        animation->typing_delay = 0;

        if(animation->revealed_chars < animation->total_chars){ animation->revealed_chars++; } 
        else { animation->finished = true; }
    }
}