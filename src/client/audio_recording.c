#include "uchat.h"

t_audata *mx_init_audio_data(void) {
    t_audata *data = malloc(sizeof(t_audata));
    data->format_type = paFloat32;
    data->number_of_channels = 0;
    data->sample_rate = SAMPLE_RATE;
    data->size = 0;
    data->recorded_samples = NULL;
    return data;
}

static void set_input_parameters(PaStreamParameters *input_parameters, t_audata *data) {
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
    output_parameters->channelCount = 2;
    output_parameters->sampleFormat = paFloat32;
    output_parameters->suggestedLatency = out_info->defaultLowOutputLatency;
    output_parameters->hostApiSpecificStreamInfo = NULL;
}

void mx_record_audio() {
    t_audata *data = mx_init_audio_data();
    PaStreamParameters input_parameters;
    PaStreamParameters output_parameters;
    PaError err = paNoError;
    PaStream *stream;

    if ((err = Pa_Initialize()) != paNoError)
        mx_terminate("Pa_Initialize");

    set_input_parameters(&input_parameters, data);

    err = Pa_OpenStream(&stream, &input_parameters, NULL, SAMPLE_RATE,
                        FRAMES_PER_BUFFER, paClipOff, NULL, NULL);
    if (err != paNoError)
        mx_terminate("Pa_OpenStream");

    if ((err = Pa_StartStream(stream)) != paNoError)
        mx_terminate("Pa_StartStream");

    float *snippet = malloc(FRAMES_PER_BUFFER * sizeof(float) * data->number_of_channels);
    memset(snippet, SAMPLE_SILENCE, FRAMES_PER_BUFFER * sizeof(float) * data->number_of_channels);
    fflush(stdout);
    for (int i = 0; i < (5 * SAMPLE_RATE) / FRAMES_PER_BUFFER; ++i) {
        err = Pa_ReadStream(stream, snippet, FRAMES_PER_BUFFER);
    }
    fflush(stdout);
    err = Pa_StopStream(stream);

    err = Pa_CloseStream(stream);

    set_output_parameters(&output_parameters);

    err = Pa_OpenStream(&stream, NULL, &output_parameters, SAMPLE_RATE,
                        FRAMES_PER_BUFFER, paClipOff, NULL, NULL);
    if ((err = Pa_StartStream(stream)) != paNoError)
        mx_terminate("Pa_StartStream");
    fflush(stdout);
    for (int i = 0; i < (5 * SAMPLE_RATE) / FRAMES_PER_BUFFER; ++i) {
        err = Pa_WriteStream(stream, snippet, FRAMES_PER_BUFFER);
    }
    fflush(stdout);

    err = Pa_StopStream(stream);

    err = Pa_CloseStream(stream);

    Pa_Terminate();
}
