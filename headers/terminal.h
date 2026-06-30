# ifndef TERMINAL
# define TERMINAL

# include <stdarg.h>

typedef struct{
    int width, height;
} TerminalDims;

# define SCREEN_BUFFER_SIZE (1024 * 64) // Double buffer.

// --- DOUBLE BUFFER FUNCTIONS ---
void bufferClear();
void bufferFlush();
void bufferPrint(const char *fmt, ...);
void bufferMove(int row, int column);

void terminalInit();
void terminalClear();
void terminalMove(int row, int column);
void terminalHideCursor();
void terminalShowCursor();
void terminalReset();
TerminalDims terminalGetSize();
int terminalCenterX(int text_width);
int terminalCenterY(int text_height);
void terminalSetColor(int r, int g, int b);

# endif