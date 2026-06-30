# include <stdio.h>
# include <stdarg.h>
# include <string.h>
# include "terminal.h"

# ifdef _WIN32
    # ifndef WINVER
        # define WINVER 0x0A00
    # endif

    # ifndef _WIN32_WINNT
        # define _WIN32_WINNT 0x0A00
    # endif

    # include <windows.h>

    # ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
        # define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    # endif
# else // I had to prove this in Linux(Fedora probably).
    # include <sys/ioctl.h>
    # include <unistd.h>
# endif

static char screen_buffer[SCREEN_BUFFER_SIZE];
static int buffer_position = 0;

void bufferClear(){
    buffer_position = 0;
    const char *home = "\033[2J\033[H";
    int lenght = 7;
    memcpy(screen_buffer, home, lenght);
    buffer_position = lenght;
}

void bufferFlush(){
    # ifdef _WIN32
        HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD written;
        WriteConsoleA(hout, screen_buffer, buffer_position, &written, NULL);
    # else 
        fwrite(screen_buffer, 1, buffer_position, stdout);
        fflush(stdout);
    # endif
    buffer_position = 0;
}

void bufferPrint(const char *fmt, ...){ // "..." To give it a variable number of arguments.
    va_list args;
    va_start(args, fmt);
    int written = vsnprintf(screen_buffer + buffer_position, SCREEN_BUFFER_SIZE - buffer_position, fmt, args);
    va_end(args);

    if(written > 0){ buffer_position += written; }
}

void bufferMove(int row, int column){ bufferPrint("\033[%d;%dH", row, column); }

void terminalInit(){
    # ifdef _WIN32
        HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        GetConsoleMode(hout, &mode);
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hout, mode);

        CONSOLE_SCREEN_BUFFER_INFO con_scr_buf_info;
        DWORD counter, cell_counter;
        COORD home = {0, 0};
        GetConsoleScreenBufferInfo(hout, &con_scr_buf_info);
        cell_counter = con_scr_buf_info.dwSize.X * con_scr_buf_info.dwSize.Y;
        FillConsoleOutputCharacter(hout, ' ', cell_counter, home, &counter);
        FillConsoleOutputAttribute(hout, con_scr_buf_info.wAttributes, cell_counter, home, &counter);
        SetConsoleCursorPosition(hout, home);
    # endif

    printf("\033[2J\033[H");
    fflush(stdout);
}

void terminalClear(){ 
    bufferPrint("\033[2J\033[H");
    // bufferPrint("\033[H");
    bufferFlush();
}

void terminalMove(int row, int column){ bufferPrint("\033[%d;%dH", row, column); }

void terminalHideCursor(){ printf("\033[?25l"); fflush(stdout); }

void terminalShowCursor(){ printf("\033[?25h"); fflush(stdout); }

void terminalReset(){ printf("\033[0m"); fflush(stdout); }

TerminalDims terminalGetSize(){
    TerminalDims size;

    # ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO con_scr_buf_info;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &con_scr_buf_info);

        size.width = con_scr_buf_info.srWindow.Right - con_scr_buf_info.srWindow.Left + 1;
        size.height = con_scr_buf_info.srWindow.Bottom - con_scr_buf_info.srWindow.Top + 1;
    # else
        struct winsize ws;

        ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

        size.width = ws.ws_col;
        size.height = ws.ws_row;
    # endif

    return size;
}

int terminalCenterX(int text_width){
    TerminalDims terminal = terminalGetSize();

    return (terminal.width - text_width) / 2;
}

int terminalCenterY(int text_height){
    TerminalDims terminal = terminalGetSize();

    return (terminal.height - text_height) / 2;
}

void terminalSetColor(int r, int g, int b){ bufferPrint("\033[38;2;%d;%d;%dm", r, g, b); }