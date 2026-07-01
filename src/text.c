# include <stdio.h>
# include <string.h>
# include "text.h"
# include "terminal.h"

# define RESET "\033[0m"

# define GRAY "\033[38;2;185;185;185m"
# define WHITE "\033[38;2;245;245;245m"
# define PINK "\033[38;2;255;105;180m"
# define RED "\033[38;2;255;70;90m"

# define BOX_WIDTH 42
# define MAX_MESSAGES 20

static const char *message[MAX_MESSAGES];
static int message_counter = 0;

void textSetMessage(const char *new_message[], int counter){
    if(counter > MAX_MESSAGES){ counter = MAX_MESSAGES; }

    message_counter = counter;

    for(int i = 0; i < counter; i++){ message[i] = new_message[i]; }
}

int textTotalChars(){
    int total = 0;
    for(int i = 0; i < message_counter; i++){ total += strlen(message[i]) + 1; }

    return total;
}

static void progress_bar(int revealed){
    int total = textTotalChars();
    if(total == 0){ total = 1; }

    int porcent = (revealed * 100) / total;
    if(porcent > 100){ porcent = 100; }

    int filled = porcent / 4;
    bufferPrint(GRAY "[");
    for(int i = 0; i < 25; i++){ (i < filled) ? bufferPrint("=") : bufferPrint(">"); }
    bufferPrint("] %3d%%" RESET, porcent);
}

static int printPartial(const char *text, int *counter, int revealed){
    int length = strlen(text);

    if(*counter >= revealed){ bufferPrint(" "); return 1 ; }

    int visible = revealed - *counter;
    if(visible > length){ visible = length; }

    for(int i = 0; i < visible; i++){ bufferPrint("%c", text[i]); }
    if(visible < length){ bufferPrint("_"); }
    *counter += length + 1;

    return (visible < length) ? visible + 1 : visible;
}

void textDraw(int revealed){
    int x = terminalCenterX(BOX_WIDTH);
    int y = terminalCenterY(10) + 10;
    int counter = 0;

    terminalMove(y, x);
    bufferPrint(WHITE "+----------------------------------------+" RESET);
    terminalMove(y + 1, x);
    bufferPrint("|");

    progress_bar(revealed);
    bufferPrint("        |");

    terminalMove(y + 2, x);
    bufferPrint("+----------------------------------------+");

    for(int i = 0; i < message_counter; i++){
        terminalMove(y + 3 + i, x);
        bufferPrint("| ");

        (i >= message_counter - 2) ? bufferPrint(PINK) : bufferPrint(GRAY);

        int printed = printPartial(message[i], &counter, revealed);
        bufferPrint(RESET);

        int padding = (BOX_WIDTH - 4) - printed;
        if(padding < 0){ padding = 0; }

        bufferPrint("%*s", padding, "");
        bufferPrint(" |");
    }
    terminalMove(y + 3 + message_counter, x);
    bufferPrint(WHITE "+----------------------------------------+" RESET);
}