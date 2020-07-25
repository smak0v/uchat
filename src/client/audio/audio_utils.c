#include "client.h"

void mx_set_output_parameters(PaStreamParameters *output_parameters,
                              t_audio *data) {
    const PaDeviceInfo *out_info;

    output_parameters->device = Pa_GetDefaultOutputDevice();

    if ((output_parameters->device) == paNoDevice)
        return;

    out_info = Pa_GetDeviceInfo(output_parameters->device);
    data->number_of_channels = out_info->maxOutputChannels;
    output_parameters->channelCount = out_info->maxOutputChannels;
    output_parameters->sampleFormat = paFloat32;
    output_parameters->suggestedLatency = out_info->defaultLowOutputLatency;
    output_parameters->hostApiSpecificStreamInfo = NULL;
}

void mx_set_input_parameters(PaStreamParameters *input_parameters,
                                 t_audio *data) {
    const PaDeviceInfo *inp_info;

    if ((input_parameters->device = Pa_GetDefaultInputDevice()) == paNoDevice)
        return;

    inp_info = Pa_GetDeviceInfo(input_parameters->device);
    data->number_of_channels = inp_info->maxInputChannels;
    input_parameters->channelCount = data->number_of_channels;
    input_parameters->sampleFormat = paFloat32;
    input_parameters->suggestedLatency = inp_info->defaultLowInputLatency;
    input_parameters->hostApiSpecificStreamInfo = NULL;
}

void mx_exit_stream(PaStream *stream) {
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();
}

void mx_free_audio_data(t_audio **data, t_sample_block **sample_block) {
    if (data && *data) {
        free((*data)->file_name);
        free((*data)->recorded_samples);
        free(*data);
        data = NULL;
    }

    if (sample_block && *sample_block) {
        if ((*sample_block)->snippet)
            free((*sample_block)->snippet);

        free(*sample_block);
        sample_block = NULL;
    }
}
