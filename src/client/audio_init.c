#include "client.h"

t_audio *mx_init_audio_data() {
    t_audio *data = malloc(sizeof(t_audio));

    data->format_type = paFloat32;
    data->number_of_channels = 0;
    data->sample_rate = SAMPLE_RATE;
    data->size = 0;
    data->recorded_samples = NULL;
    data->file_name = NULL;
    return data;
}

t_sample_block *mx_init_sample_block(t_audio *data) {
    t_sample_block *sample_block = mx_memalloc(sizeof(t_sample_block));

    if (sample_block) {
        sample_block->size =
            FRAMES_PER_BUFFER * sizeof(float) * data->number_of_channels;

        if ((sample_block->snippet = malloc(sample_block->size)) == NULL)
            return NULL;
        memset(sample_block->snippet, SAMPLE_SILENCE, sample_block->size);
    }

    return sample_block;
}

int mx_init_input_stream(PaStream **stream, t_audio *data) {
    PaError err = paNoError;
    PaStreamParameters input_parameters;

    if ((err = Pa_Initialize()) != paNoError)
        return err;

    mx_set_input_parameters(&input_parameters, data);

    err = Pa_OpenStream(stream, &input_parameters, NULL,
                        SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, NULL, NULL);
    if (err != paNoError)
        return err;

    return Pa_StartStream(*stream);
}

int mx_init_output_stream(PaStream **stream, t_audio *data) {
    PaError err = paNoError;
    PaStreamParameters output_parameters;

    if ((err = Pa_Initialize()) != paNoError)
        return err;
    mx_set_output_parameters(&output_parameters, data);

    err = Pa_OpenStream(stream, NULL, &output_parameters,
                        SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, NULL, NULL);
    if (err != paNoError)
        return err;

    return Pa_StartStream(*stream);
}
