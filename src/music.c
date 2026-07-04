# include <stdio.h>
# include <stdint.h>
# include <math.h>
# include "music.h"

void renderNote(int16_t *buffer, int num_samples, double frequency){
    double fade = 2.8;

    for(int i = 0; i < num_samples; i++){
        double tempo = (double)i / 44100;
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