# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include "particles.h"
# include "terminal.h"

static Particle particles[MAX_PARTICLES];
static const char symbols[] = { '.', '*', '+', 'o' };

static float randomFloat(float min, float max){ return min + ((float)rand() / RAND_MAX) * (max - min); }

void particlesInit(int width, int height){
    srand(time(NULL));

    for(int i = 0; i < MAX_PARTICLES; i++){
        particles[i].x = randomFloat(1, width);
        particles[i].y = randomFloat(1, height);

        particles[i].vx = randomFloat(-0.15f, 0.15f);
        particles[i].vy = randomFloat(-0.08f, 0.08f);

        particles[i].prev_x = (int)particles[i].x;
        particles[i].prev_y = (int)particles[i].y;

        particles[i].symbol = symbols[rand() % (sizeof(symbols) / sizeof(symbols[0]))];
    }
}

void particlesUpdate(int width, int height){
    for(int i = 0; i < MAX_PARTICLES; i++){
        particles[i].prev_x = (int)particles[i].x;
        particles[i].prev_y = (int)particles[i].y;

        particles[i].x += particles[i].vx;
        particles[i].y += particles[i].vy;

        if(particles[i].x < 1){ particles[i].x = 1; particles[i].vx *= -1; }

        if(particles[i].x > width){ particles[i].x = width; particles[i].vx *= -1; }

        if(particles[i].y < 1){ particles[i].y = 1; particles[i].vy *= -1; }

        if(particles[i].y > height){ particles[i].y = height; particles[i].vy *= -1; }

        particles[i].vx += randomFloat(-0.01f, 0.01f);
        particles[i].vy += randomFloat(-0.005f, 0.005f);

        if(particles[i].vx > 0.20f){ particles[i].vx = 0.20f; }
        if(particles[i].vx < -0.20f){ particles[i].vx = -0.20f; }

        if(particles[i].vy > 0.10f){ particles[i].vy = 0.10f; }
        if(particles[i].vy < -0.10f){ particles[i].vy = -0.10f; }
    }
}

void drawParticles(){
    for(int i = 0; i < MAX_PARTICLES; i++){
        bufferPrint("\033[%d;%dH ", (int)particles[i].y, (int)particles[i].x);

        switch(particles[i].symbol){
            case '.': bufferPrint("\033[38;2;160;160;160m."); break;
            case '*': bufferPrint("\033[38;2;255;220;120m*"); break;
            case '+': bufferPrint("\033[38;2;255;120;170m+"); break;
            case 'o': bufferPrint("\033[38;2;255;255;255mo"); break;
        }
        bufferPrint("\033[0m");
    }
}