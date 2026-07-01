# ifndef ANIMATION
# define ANIMATION

# include <stdbool.h>

typedef struct{
    int frame, pulse;
    int total_chars, revealed_chars;
    int typing_delay, pulse_delay;
    bool finished;
} Animation;

void animationInit(Animation *animation, int total_chars);
void animationUpdate(Animation *animation);

# endif