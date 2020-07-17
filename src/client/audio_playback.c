#include "client.h"

static void set_input_output_parameters(PaStreamParameters *input_parameters,
                                        PaStreamParameters *output_parameters,
                                        t_audio *data) {
    const PaDeviceInfo *inp_info;
    const PaDeviceInfo *out_info;

    if ((input_parameters->device = Pa_GetDefaultInputDevice()) == paNoDevice)
        mx_terminate("Error: No default input device");
    inp_info = Pa_GetDeviceInfo(input_parameters->device);
    data->number_of_channels = inp_info->maxInputChannels;
    input_parameters->channelCount = data->number_of_channels;
    input_parameters->sampleFormat = paFloat32;
    input_parameters->suggestedLatency = inp_info->defaultLowInputLatency;
    input_parameters->hostApiSpecificStreamInfo = NULL;
    if ((output_parameters->device = Pa_GetDefaultOutputDevice()) == paNoDevice)
        mx_terminate("Error: No default output device");
    out_info = Pa_GetDeviceInfo(output_parameters->device);
    output_parameters->channelCount = out_info->maxOutputChannels;
    output_parameters->sampleFormat = paFloat32;
    output_parameters->suggestedLatency = out_info->defaultLowOutputLatency;
    output_parameters->hostApiSpecificStreamInfo = NULL;
}

void mx_play_audio_file(char *path) {
    t_audio *data = mx_init_audio_data();
    PaError err = paNoError;
    PaStream *stream;
    SNDFILE *file;
    SF_INFO sfinfo = {
        .channels = data->number_of_channels,
        .samplerate = data->sample_rate,
        .format = SF_FORMAT_AIFF | SF_FORMAT_FLOAT
    };

    if (!(file = sf_open(path, SFM_READ, SF_INFO *sfinfo))
        mx_terminate("open");

    if ((err = Pa_Initialize()) != paNoError)
        mx_terminate("Can't initialize PortAudio library");

    if ((err = mx_init_audio_stream(&stream, data)) != paNoError)
        mx_terminate("Can't start audio stream");

    sfinfo.channels = data->number_of_channels;
    err = Pa_StopStream(stream);
    err = Pa_CloseStream(stream);
    Pa_Terminate();
}
