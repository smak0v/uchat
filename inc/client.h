#pragma once

// Includes
#include "uchat.h"
#include <gtk/gtk.h>
#include "portaudio.h"
#include "sndfile.h"

// Constants
#define MX_UI_PATH "./src/client/ui/"

#define FRAMES_PER_BUFFER 1024
#define SAMPLE_RATE  (44100)
#define SAMPLE_SILENCE  (0.0f)

// Structures

typedef struct s_audio
{
    uint16_t format_type;
    uint8_t number_of_channels;
    uint32_t sample_rate;
    size_t size;
    float *recorded_samples;
    char *file_name;
} t_audio;

typedef struct s_sample_block
{
    float *snippet;
    size_t size;
} t_sample_block;


// Functions
int mx_init_client(int argc, char **argv);
int mx_init_input_stream(PaStream **stream, t_audio *data);
int mx_init_output_stream(PaStream **stream, t_audio *data);
t_sample_block *mx_init_sample_block(t_audio *data);
void mx_exit_stream(PaStream *stream);
void mx_free_audio_data(t_audio **data, t_sample_block **sample_block);
void mx_set_output_parameters(PaStreamParameters *output_parameters, 
                              t_audio *data);
void mx_set_input_parameters(PaStreamParameters *input_parameters,
                             t_audio *data);

t_audio *mx_init_audio_data();
void mx_record_audio(void);
void mx_play_audio_file(char *path);

// UI
char *mx_build_ui_path(char *filename);

// SSL/TLS
SSL_CTX *mx_init_client_ctx(void);
void mx_show_server_certs(SSL *ssl);
