#include "client.h"

static void mx_play_audio(SF_INFO *sfinfo, SNDFILE *file, PaStream *stream) {
    float buf[FRAMES_PER_BUFFER * sfinfo->channels];
    PaError err = paNoError;
    size_t read_count = 0;

    memset(buf, 0, sizeof(buf));
    while ((read_count = sf_read_float(file, buf, FRAMES_PER_BUFFER))) {
        err = Pa_WriteStream(
            stream, buf, FRAMES_PER_BUFFER / sfinfo->channels);
        if (err != paNoError)
            return;
        memset(buf, 0, sizeof(buf));
    }
}

void mx_play_audio_file(char *path) {
    t_audio *data = mx_init_audio_data();
    PaError err = paNoError;
    PaStream *stream = NULL;
    SNDFILE *file;
    SF_INFO sfinfo = {
        .samplerate = data->sample_rate,
        .format = SF_FORMAT_AIFF | SF_FORMAT_FLOAT
    };

    if ((file = sf_open(path, SFM_READ, &sfinfo))) {
        if ((err = mx_init_output_stream(&stream, data)) != paNoError) {
            mx_free_audio_data(&data, NULL);
            return;
        }
        sfinfo.channels = data->number_of_channels;
        mx_play_audio(&sfinfo, file, stream);
        sf_close(file);
    }
    mx_free_audio_data(&data, NULL);
    mx_exit_stream(stream);
}
