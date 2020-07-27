#include "client.h"

void mx_play_audio_thread(t_glade *g, char *path) {
    pthread_t play_audio_thread_id;

    pthread_create(&play_audio_thread_id, NULL, mx_play_audio_file,
        (void *)path);

    g->player = play_audio_thread_id;
}
