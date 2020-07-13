#pragma once

// Includes
#include <gtk/gtk.h>
#include "portaudio.h"

// Constants
#define MX_UI_PATH "./src/client/ui/"

#define FRAMES_PER_BUFFER 1024
#define SAMPLE_RATE  (44100)
#define SAMPLE_SILENCE  (0.0f)

// Structures

typedef struct s_audata
{
    uint16_t format_type;
    uint8_t number_of_channels;
    uint32_t sample_rate;
    size_t size;
    float *recorded_samples;
} t_audata;

typedef struct s_ausnippet
{
    float *snippet;
    size_t size;
} t_ausnippet;


// Functions
int mx_init_client(int argc, char **argv);

t_audata *mx_init_audio_data(void);
void mx_record_audio(void);

// UI
char *mx_build_ui_path(char *filename);

// SSL/TLS
SSL_CTX *mx_init_client_ctx(void);
void mx_show_server_certs(SSL *ssl);
