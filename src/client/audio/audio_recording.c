#include "client.h"

static int mx_record_sample(PaStream *stream, t_audio *data,
                            t_sample_block *sample_block, int i) {
    PaError err;
    size_t next_ndex;
    char *destination;

    ++i;
    err = Pa_ReadStream(stream, sample_block->snippet, FRAMES_PER_BUFFER);
    if (err != paNoError)
        return err;

    data->size = i * sample_block->size;
    data->recorded_samples = realloc(data->recorded_samples, data->size);

    next_ndex = (i - 1) * sample_block->size;
    destination = (char *)data->recorded_samples + next_ndex;

    memcpy(destination, sample_block->snippet, sample_block->size);

    return err;
}

static long mx_save_audio(t_audio *data, char *filename) {
    uint8_t err = SF_ERR_NO_ERROR;
    SNDFILE *outfile = NULL;
    long wr = 0;
    SF_INFO sfinfo = {
        .channels = data->number_of_channels,
        .samplerate = data->sample_rate,
        .format = SF_FORMAT_AIFF | SF_FORMAT_FLOAT};

    if (!(outfile = sf_open(filename, SFM_WRITE, &sfinfo)))
        return -1;

    wr = sf_writef_float(outfile, data->recorded_samples, data->size / 8);
    err = data->size - wr;
    sf_write_sync(outfile);
    sf_close(outfile);
    return err;
}

static void handle_recorded_audio(t_audio *data, t_glade *g) {
    char buf[PATH_MAX];
    char filename[100];

    snprintf(filename, 100, "rec_%d.aif", rand());
    if (data->recorded_samples) {
        mx_save_audio(data, filename);

        realpath(filename, buf);
        g->filename = strdup(buf);
        mx_send_msg(g->b_audio, g);
    }
}

void *mx_thread_record_audio(void *thread_data) {
    t_audio *data = mx_init_audio_data();
    t_sample_block *sample_block = NULL;
    PaStream *stream = NULL;

    if (mx_init_input_stream(&stream, data) != paNoError) {
        mx_free_audio_data(&data, NULL);
        return thread_data;
    }
    sample_block = mx_init_sample_block(data);
    for (int i = 0; i < (60 * SAMPLE_RATE) / FRAMES_PER_BUFFER; ++i) {
        if (!((t_thread_data *)thread_data)->glade->record_audio_pressed)
            break;
        if (mx_record_sample(stream, data, sample_block, i) != paNoError)
            break;
    }
    handle_recorded_audio(data, ((t_thread_data *)thread_data)->glade);
    mx_exit_stream(stream);
    mx_free_audio_data(&data, &sample_block);
    pthread_join(((t_thread_data *)thread_data)->glade->recorder, NULL);
    return thread_data;
}

gboolean mx_record_audio(GtkWidget *w, GdkEventKey *e, t_glade *g) {
    pthread_t recorder_id;
    t_thread_data *data = malloc(sizeof(t_thread_data));

    (void)w;
    (void)e;

    pthread_mutex_lock(&g->recorder_mutex);
    g->record_audio_pressed = true;
    pthread_mutex_unlock(&g->recorder_mutex);

    data->ssl = g->ssl;
    data->glade = g;

    pthread_create(&recorder_id, NULL, mx_thread_record_audio, (void *)data);

    pthread_mutex_lock(&g->recorder_mutex);
    g->recorder = recorder_id;
    pthread_mutex_unlock(&g->recorder_mutex);

    return FALSE;
}
