#include "uchat.h"

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

static long mx_save_audio(t_audio *data) {
    uint8_t err = SF_ERR_NO_ERROR;
    char file_name[100];
    SNDFILE *outfile;
    long wr;
    SF_INFO sfinfo = {
        .channels = data->number_of_channels,
        .samplerate = data->sample_rate,
        .format = SF_FORMAT_AIFF | SF_FORMAT_FLOAT
    };

    snprintf(file_name, 100, "rec_%d.aif", rand());
    if (!(outfile = sf_open(file_name, SFM_WRITE, &sfinfo)))
        return -1;
    wr = sf_writef_float(outfile, data->recorded_samples, data->size / 8);
    err = data->size - wr;
    sf_write_sync(outfile);
    sf_close(outfile);
    data->file_name = strdup(file_name);
    return err;
}

void mx_record_audio() {
    t_audio *data = mx_init_audio_data();
    t_sample_block *sample_block;
    PaError err = paNoError;
    PaStream *stream = NULL;

    if ((err = mx_init_input_stream(&stream, data)) != paNoError) {
        mx_free_audio_data(&data, NULL);
        return;
    }
    sample_block = mx_init_sample_block(data);
    for (int i = 0; i < (5 * SAMPLE_RATE) / FRAMES_PER_BUFFER; ++i) {
        // ToDo: handle flag from client
        err = mx_record_sample(stream, data, sample_block, i);
        if (err != paNoError)
            break;
    }
    if (data->recorded_samples)
        mx_save_audio(data);
    mx_exit_stream(stream);
    mx_free_audio_data(&data, &sample_block);
}
