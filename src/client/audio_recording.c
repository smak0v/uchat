#include "uchat.h"

t_audio *mx_init_audio_data() {
    t_audio *data = malloc(sizeof(t_audio));

    data->format_type = paFloat32;
    data->number_of_channels = 0;
    data->sample_rate = SAMPLE_RATE;
    data->size = 0;
    data->recorded_samples = NULL;
    return data;
}

t_sample_block *mx_init_sample_block(t_audio *data) {
    t_sample_block *sample_block = mx_memalloc(sizeof(t_sample_block));

    sample_block->size =
        FRAMES_PER_BUFFER * sizeof(float) * data->number_of_channels;

    if ((sample_block->snippet = malloc(sample_block->size)) == NULL)
        return NULL;
    memset(sample_block->snippet, SAMPLE_SILENCE, sample_block->size);

    return sample_block;
}

static void set_input_parameters(PaStreamParameters *input_parameters, t_audio *data) {
    const PaDeviceInfo *inp_info;

    if ((input_parameters->device = Pa_GetDefaultInputDevice()) == paNoDevice)
        mx_terminate("Error: No default input device");
    inp_info = Pa_GetDeviceInfo(input_parameters->device);
    data->number_of_channels = inp_info->maxInputChannels;
    input_parameters->channelCount = data->number_of_channels;
    input_parameters->sampleFormat = paFloat32;
    input_parameters->suggestedLatency = inp_info->defaultLowInputLatency;
    input_parameters->hostApiSpecificStreamInfo = NULL;
}

static void set_output_parameters(PaStreamParameters *output_parameters) {
    const PaDeviceInfo *out_info;

    if ((output_parameters->device = Pa_GetDefaultOutputDevice()) == paNoDevice)
        mx_terminate("Error: No default output device");
    out_info = Pa_GetDeviceInfo(output_parameters->device);
    output_parameters->channelCount = out_info->maxOutputChannels;
    output_parameters->sampleFormat = paFloat32;
    output_parameters->suggestedLatency = out_info->defaultLowOutputLatency;
    output_parameters->hostApiSpecificStreamInfo = NULL;
}

int mx_record_sample(PaStream *stream, t_audio *data, t_sample_block *sample_block, int i) {
    PaError err;
    
    ++i;
    err = Pa_ReadStream(stream, sample_block->snippet, FRAMES_PER_BUFFER);
    data->recorded_samples = realloc(data->recorded_samples, i * sample_block->size);
    data->size = i * sample_block->size;
    size_t next_ndex = ((i)- 1) * sample_block->size;
    char *destination = (char*)data->recorded_samples + next_ndex;
    memcpy(destination, sample_block->snippet, sample_block->size);
    // memcpy(data->recorded_samples + ((i -1) * sample_block->size), sample_block->snippet, sample_block->size);
    return err;
}

void mx_record_audio() {
    PaError err = paNoError;
    t_audio *data = mx_init_audio_data();
    t_sample_block *sample_block;
    PaStreamParameters input_parameters;
    PaStreamParameters output_parameters;
    PaStream *stream;

    if ((err = Pa_Initialize()) != paNoError)
        mx_terminate("Pa_Initialize");

    set_input_parameters(&input_parameters, data);
    set_output_parameters(&output_parameters);
    sample_block = mx_init_sample_block(data);

    err = Pa_OpenStream(&stream, &input_parameters, &output_parameters, SAMPLE_RATE,
                        FRAMES_PER_BUFFER, paClipOff, NULL, NULL);
    if (err != paNoError)
        mx_terminate("Pa_OpenStream");

    if ((err = Pa_StartStream(stream)) != paNoError)
        mx_terminate("Pa_StartStream");

    for (int i = 0; i < (5 * SAMPLE_RATE) / FRAMES_PER_BUFFER; ++i) {
        err = mx_record_sample(stream, data, sample_block, i);
    }

    for (int i = 0; i < (5 * SAMPLE_RATE) / FRAMES_PER_BUFFER; ++i) {
        err = Pa_WriteStream(stream, (char *)data->recorded_samples + i * sample_block->size, FRAMES_PER_BUFFER);
    }

    err = Pa_StopStream(stream);
    err = Pa_CloseStream(stream);

    Pa_Terminate();
}
