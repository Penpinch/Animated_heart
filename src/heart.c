# include <stdio.h>
# include "animation.h"
# include "shapes.h"
# include "particles.h"
# include "terminal.h"
# include "text.h"

# ifdef _WIN32
    # include <windows.h>
# else 
    # include <unistd.h> // For macOS and Linux.
# endif

int main(){
    const char *message[] = {
        "                TEXT",
        "                TEXT", 
        "                TEXT", 
        "                TEXT", 
        "                TEXT", 
        "                TEXT", 
        "                TEXT", 
        "                TEXT"
    };
    textSetMessage(message, sizeof(message) / sizeof(message[0]));

    terminalInit();
    terminalHideCursor();

    TerminalDims screen = terminalGetSize();
    Animation animation;

    animationInit(&animation, textTotalChars());
    particlesInit(screen.width, screen.height);

    while(!animation.finished){
        screen = terminalGetSize();
        bufferClear();

        particlesUpdate(screen.width, screen.height);
        drawParticles();

        int heart_x = terminalCenterX(40);
        int heart_y = terminalCenterY(15) - 6;
        drawHeart(animation.pulse, heart_x, heart_y);

        textDraw(animation.revealed_chars);
        bufferFlush();

        animationUpdate(&animation);
    }

    terminalShowCursor();
    terminalReset();

    return 0;
}