# include <stdio.h>
# include <stdint.h>
# include <math.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include "music.h"

# ifdef _WIN32
    # include "windows.h"
# else
    # include <alsa/asoundlib.h>
    # include <unistd.h>
    # include <pthread.h>
# endif

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

static volatile bool stop_flag = false;

void audioStop(){ stop_flag = true; }

# ifdef _WIN32
    static HANDLE audio_thread_handle = NULL;

    void playAudio(){
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
            CloseHandle(h_event);
            return;
        }

        size_t note_index = 0;
        while(!stop_flag){
            const Note *note = &melody[note_index % NUM_NOTES];
            int n = (int)((note->duration_ms / 1000.0) * SAMPLE_RATE);

            int16_t *buffer = (int16_t*)malloc((size_t)n * sizeof(int16_t));
            renderNote(buffer, n, note->frequency_hz);

            WAVEHDR header;
            memset(&header, 0, sizeof(header));
            header.lpData = (LPSTR)buffer;
            header.dwBufferLength = (DWORD)(n * sizeof(int16_t));
    
            waveOutPrepareHeader(h_wave_out, &header, sizeof(header));
            waveOutWrite(h_wave_out, &header, sizeof(header));
    
            while(!(header.dwFlags & WHDR_DONE)){ WaitForSingleObject(h_event, INFINITE); }
    
            waveOutUnprepareHeader(h_wave_out, &header, sizeof(header));
            free(buffer);
            note_index++;
        }
        waveOutClose(h_wave_out);
        CloseHandle(h_event);
    }

    DWORD WINAPI audioThread(LPVOID param){
        (void)param;
        playAudio();

        return 0;
    }

    void playAudioAsync(){ 
        stop_flag = false;
        audio_thread_handle = CreateThread(NULL, 0, audioThread, NULL, 0, NULL); 
    }

    void waitForAudio(){
        if(audio_thread_handle){
            WaitForSingleObject(audio_thread_handle, INFINITE);
            CloseHandle(audio_thread_handle);
            audio_thread_handle = NULL;
        }
    }

# else
    static pthread_t audio_thread;
    static int thread_started = 0;

    void playAudio(){
        snd_pcm_t *pcm_handle;
        snd_pcm_hw_params_t *params;
        int err;

        if((err = snd_pcm_open(&pcm_handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0){
            fprintf(stderr, "Error at opening ALSA device: %s\n", snd_strerror(err));
            return;
        }

        snd_pcm_hw_params_alloca(&params);
        snd_pcm_hw_params_any(pcm_handle, params);
        snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
        snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE);
        snd_pcm_hw_params_set_channels(pcm_handle, params, 1);

        unsigned int rate = SAMPLE_RATE;
        snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, 0);

        if((err = snd_pcm_hw_params(pcm_handle, params)) < 0){
            fprintf(stderr, "Error configuring ALSA parameters: %s\n", snd_strerror(err));
            snd_pcm_close(pcm_handle);
            return;
        }

        size_t note_index = 0;
        while(!stop_flag){
            const Note *note = &melody[note_index % NUM_NOTES];
            int n = (int)((note->duration_ms / 1000.0) * SAMPLE_RATE);

            int16_t *buffer = (int16_t*)malloc((size_t)n * sizeof(int16_t));
            renderNote(buffer, n, note->frequency_hz);

            snd_pcm_uframes_t frames_written = 0;
            while(frames_written < (snd_pcm_uframes_t)n && !stop_flag){
                snd_pcm_sframes_t written = snd_pcm_writei(pcm_handle, buffer + frames_written, (snd_pcm_uframes_t)n - frames_written);
                if(written < 0){
                    written = snd_pcm_recover(pcm_handle, (int)written, 0);
                    if(written < 0){ fprintf(stderr, "Error de escritura ALSA: %s\n", snd_strerror((int)written)); break; }
                } else {
                    frames_written += (snd_pcm_uframes_t)written;
                }
            }
            free(buffer);
            if(stop_flag){ break; }
            note_index++;
        }

        snd_pcm_drop(pcm_handle);
        snd_pcm_close(pcm_handle);
    }

    void *audioThread(void *param){
        (void)param;
        playAudio();

        return NULL;
    }

    void playAudioAsync(){
        stop_flag = false;
        thread_started = (pthread_create(&audio_thread, NULL, audioThread, NULL) == 0);
    }

    void waitForAudio(){
        if(thread_started){
            pthread_join(audio_thread, NULL);
            thread_started = 0;
        }
    }
# endif