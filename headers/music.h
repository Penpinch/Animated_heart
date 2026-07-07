# ifndef MUSIC
# define MUSIC

# include <stdint.h> // To avoid incompatibility between different architectures.

# define REST 0.0
# define NOTE_C4 261.63
# define NOTE_D4 293.66
# define NOTE_E4 329.63
# define NOTE_F4 349.23
# define NOTE_G4 392.00
# define NOTE_A4 440.00
# define NOTE_B4 493.88
# define NOTE_C5 523.25

# define PI 3.14159265358979323846
# define SAMPLE_RATE 44100

typedef struct{
    double frequency_hz;
    int duration_ms;
} Note;

static const Note melody[] = {
    {NOTE_C4,500}, {NOTE_E4,500}, {NOTE_E4,500}, {NOTE_F4,500}, {NOTE_E4,500}, {NOTE_D4,1000},
    {NOTE_E4,500}, {NOTE_G4,500}, {NOTE_G4,500}, {NOTE_F4,500}, {NOTE_E4,500}, {NOTE_D4,1000},
    {NOTE_E4,500}, {NOTE_C5,500}, {NOTE_C5,500}, {NOTE_A4,500}, {NOTE_F4,500}, {NOTE_E4,500},
    {NOTE_D4,1000}, {REST,300}, {NOTE_C4,1500}
};

# define NUM_NOTES (sizeof(melody) / sizeof(melody[0]))

void renderNote(int16_t *buffer, int num_samples, double frequency);
// int16_t *buildAudio(size_t *out_total_samples);
void playAudio();
void playAudioAsync();
void audioStop();
void waitForAudio();

# endif