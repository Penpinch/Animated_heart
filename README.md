# Animated heart.

An animation of a heart for a gift made in the **C** programming language.

## Features
- Runs in Microsoft Windows and Linux.
- Implemented in C.
- Runs in terminal.
- Different colors.
- Plays a melody during the animation.
- The duration of the music is adapted to the duration of the text.

>Tested on Microsoft Windows 11, Fedora 44 and Ubuntu 25.10.

# Project Structure

```
Animated_heart/
├── headers/
│   ├── shapes.h
│   ├── terminal.h
│   ├── text.h
│   ├── animation.h
│   ├── particles.h
│   └── music.h
├── src/
│   ├── shapes.c      # Heart definition.
│   ├── terminal.c    # Terminal use and double buffer.
│   ├── text.c        # Text shown.
│   ├── animation.c   # Animation of the heart.
│   ├── particles.c   # Background particles movement.
│   ├── music.c       # Play music.
│   └── heart.c       # Main.
└── README.md
```

## Clone, Build and Run
Clone the repository:
```bash
git clone https://github.com/Penpinch/Animated_heart.git

cd Animated_heart
```

Compile the project:

### Microsoft Windows:
```bash
gcc src\heart.c src\shapes.c src\animation.c src\terminal.c src\text.c src\particles.c src\music.c -I.\headers\ -o animated_heart.exe -lwinmm -lm
```

Run the executable:
```bash
.\animated_heart.exe
```

### Linux:
```bash
gcc src/heart.c src/shapes.c src/animation.c src/terminal.c src/text.c src/particles.c src/music.c -Iheaders/ -o animated_heart.exe -lasound -lm
```
>ALSA is necessary to compile on Linux.

Run the executable:
```bash
./animated_heart
```

## Technologies Used
- C programming language.
- Termianl.
- Git and GitHub.