# include <stdio.h>
# include <stdint.h>
# include <math.h>
# include <stdlib.h>
# include <string.h>
# include "music.h"

# include "windows.h"

void renderNote(int16_t *buffer, int num_samples, double frequency){
    double fade = 2.8;

    for(int i = 0; i < num_samples; i++){
        double tempo = (double)i / SAMPLE_RATE;
        double sample = REST;

        if(frequency > REST){
            double env1 = exp(-fade * tempo);
            double env2 = exp(-fade * tempo * 1.8);
            double env3 = exp(-fade * tempo * 2.6);

            sample = 1.00 * env1 * sin(2 * PI * frequency * tempo) 
                + 0.30 * env2 * sin(2 * PI * frequency * tempo * 2.0) 
                + 0.1 * env3 * sin(2 * PI * frequency * tempo * 3.0);

            double attack = 1 - exp(-tempo * 400);
            sample *= attack;
        }

        double scaled = sample * 10000.0;
        if(scaled > 32000.0){ scaled = 32000.0; }
        if(scaled < -32000.0){ scaled = -32000.0; }

        buffer[i] = (int16_t)scaled;
    }
}

int16_t *buildAudio(size_t *out_total_samples){
    size_t total_samples = 0;
    for(size_t i = 0; i < NUM_NOTES; i++){ total_samples += (size_t)((melody[i].duration_ms / 1000.0) * SAMPLE_RATE); }

    int16_t *buffer = (int16_t*)calloc(total_samples, sizeof(int16_t));
    if(!buffer){ fprintf(stderr, "Error: couldn't use memory for audio.\n"); exit(1); }

    size_t offset = 0;
    for(size_t i = 0; i < NUM_NOTES; i++){
        int n = (int)((melody[i].duration_ms / 1000.0) * SAMPLE_RATE);
        renderNote(buffer + offset, n, melody[i].frequency_hz);
        offset += n;
    }

    *out_total_samples = total_samples;
    return buffer;
}

void playAudio(int16_t *buffer, size_t total_samples){
    WAVEFORMATEX wa_for_x;
    memset(&wa_for_x, 0, sizeof(wa_for_x));
    wa_for_x.wFormatTag = WAVE_FORMAT_PCM;
    wa_for_x.nChannels = 1;
    wa_for_x.nSamplesPerSec = SAMPLE_RATE;
    wa_for_x.wBitsPerSample = 16;
    wa_for_x.nBlockAlign = (wa_for_x.nChannels * wa_for_x.wBitsPerSample) / 8;
    wa_for_x.nAvgBytesPerSec = wa_for_x.nSamplesPerSec * wa_for_x.nBlockAlign;

    HWAVEOUT h_wave_out;
    HANDLE h_event = CreateEvent(NULL, FALSE, FALSE, NULL);

    if(waveOutOpen(&h_wave_out, WAVE_MAPPER, &wa_for_x, (DWORD_PTR)h_event, 0, CALLBACK_EVENT) != MMSYSERR_NOERROR){
        fprintf(stderr, "Error: couldn't open the audio device.\n");
        exit(1);
    }

    WAVEHDR header;
    memset(&header, 0, sizeof(header));
    header.lpData = (LPSTR)buffer;
    header.dwBufferLength = (DWORD)(total_samples * sizeof(int16_t));

    waveOutPrepareHeader(h_wave_out, &header, sizeof(header));
    waveOutWrite(h_wave_out, &header, sizeof(header));

    while(!(header.dwFlags & WHDR_DONE)){
        WaitForSingleObject(h_event, INFINITE);
    }

    waveOutUnprepareHeader(h_wave_out, &header, sizeof(header));
    waveOutClose(h_wave_out);
    CloseHandle(h_event);
}

int main(){
    size_t total_samples;
    int16_t *audio = buildAudio(&total_samples);

    playAudio(audio, total_samples);

    free(audio);
    printf("Finished.\n");

    return 0;
}