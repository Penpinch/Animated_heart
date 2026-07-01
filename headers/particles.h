# ifndef PARTICLES
# define PARTICLES

# define MAX_PARTICLES 80

typedef struct{
    float x, y;
    float vx, vy;
    float prev_x, prev_y;
    char symbol;
} Particle;

void particlesInit(int width, int height);
void particlesUpdate(int width, int height);
void drawParticles();

# endif