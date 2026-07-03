# Animated heart.

An animation of a heart for a gift made in the **C** programming language.

## Features
- Runs in Microsoft Windows, macOS and Linux.
- Implemented in C.
- Runs in terminal.
- Different colors.

>Tested on Microsoft Windows 11 and Fedora 44.

# Project Structure

```
Animated_heart/
├── headers/
│   ├── shapes.h
│   ├── terminal.h
│   ├── text.h
│   ├── animation.h
│   └── particles.h
├── src/
│   ├── shapes.c      # Heart definition.
│   ├── terminal.c    # Terminal use and double buffer.
│   ├── text.c        # Text shown.
│   ├── animation.c   # Animation of the heart.
│   ├── particles.c   # Background particles movement.
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
```bash
gcc src\heart.c src\shapes.c src\terminal.c src\text.c src\animation.c src\particles.c -I.\headers\ -o animated_heart.exe
```

Run the executable:
```bash
.\animated_heart.exe
```

## Technologies Used
- C programming language.
- Termianl.
- Git and GitHub.