# include <stdio.h>
# include <stdint.h>
# include <math.h>
# include <stdlib.h>
# include "music.h"

void renderNote(int16_t *buffer, int num_samples, double frequency){
    double fade = 2.8;

    for(int i = 0; i < num_samples; i++){
        double tempo = (double)i / SAMPLE_RATE;
        double sample = REST;

        if(frequency > REST){
            double env1 = exp(-fade * tempo);
            double env2 = exp(-fade * tempo * 1.8);
            double env3 = exp(-fade * tempo * 2.6);

            sample = 1 * env1 * sin(2 * PI * frequency * tempo) 
                + 0.30 * env2 * sin(2 * PI * frequency * tempo * 2) 
                + 0.1 * env3 * sin(2 * PI * frequency * tempo * 3);

            double attack = 1 - exp(-tempo * 400);
            sample *= attack;
        }

        double scaled = sample * 10000;
        if(scaled > 32000){ scaled = 32000; }
        if(scaled < -32000){ scaled = -32000; }

        buffer[i] = (int16_t)scaled;
    }
}

int16_t *buildAudio(size_t *out_total_samples){
    size_t total_samples = 0;
    for(size_t i = 0; i < NUM_NOTES; i++){ total_samples += (size_t)((melody[i].duration_ms / 1000) * SAMPLE_RATE); }

    int16_t *buffer = (int16_t)calloc(total_samples, sizeof(int16_t));
    if(!buffer){ fprintf(stderr, "Error: couldn't use memory for audio.\n"); exit(1); }

    size_t offset = 0;
    for(size_t i = 0; i < NUM_NOTES; i++){
        int n = (int)((melody[i].duration_ms / 1000) * SAMPLE_RATE);
        renderNote(buffer + offset, n, melody[i].frequency_hz);
        offset += n;
    }

    *out_total_samples = total_samples;
    return buffer;
}