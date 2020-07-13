#include "uchat.h"

int mx_process_stream(PaStream *stream, t_audio *data,
                          t_a_snippet *sample_block, int *i) {
    if (!stream || !data || !sample_block)
        return -1;
    (*i)++;
    Pa_ReadStream(stream, sample_block->snippet, FRAMES_PER_BUFFER);
    data->rec_samples = realloc(data->rec_samples, sample_block->size * (*i));
    data->size = sample_block->size * (*i);
    if (data->rec_samples) {
        size_t next_ndex = ((*i)- 1) * sample_block->size;
        char *destination = (char*)data->rec_samples + next_ndex;
        memcpy(destination, sample_block->snippet, sample_block->size);
    }
    else {
        free(data->rec_samples);
        data->rec_samples = NULL;
        data->size = 0;
    }
    return 0;
}

static int init_stream_start(t_audio *data) {
    PaError err;
    const PaDeviceInfo *info;

    err = Pa_Initialize();
    if (err != paNoError)
        return mx_exit_stream(data, err);
    err = Pa_GetDeviceCount();

    info = Pa_GetDeviceInfo(Pa_GetDefaultInputDevice());
    if (!info) {
        fprintf(stdout, "%s\n", "Unable to find info on def input device.");
        return 1;
    }
    return 0;
}

static int set_input_parameters(PaStreamParameters *input_parameters,
                                PaStreamParameters *output_parameters) {
    const PaDeviceInfo* inp_info;
    const PaDeviceInfo* out_info;
    int num_chan;

    input_parameters->device = Pa_GetDefaultInputDevice();
    inp_info = Pa_GetDeviceInfo(input_parameters->device);
    printf( "   Name: %s\n", inp_info->name );
    printf( "     LL: %g s\n", inp_info->defaultLowInputLatency);
    printf( "     HL: %g s\n", inp_info->defaultHighInputLatency);
    if (input_parameters->device == paNoDevice)
        mx_err_return3("Error: No default input device.\n", NULL, -1);
    output_parameters->device = Pa_GetDefaultOutputDevice();
    out_info = Pa_GetDeviceInfo( output_parameters->device);
    printf( "   Name: %s\n", out_info->name );
    printf( "     LL: %g s\n", out_info->defaultLowOutputLatency);
    printf( "     HL: %g s\n", out_info->defaultHighOutputLatency);
    num_chan = inp_info->maxInputChannels < out_info->maxOutputChannels
                   ? inp_info->maxInputChannels : out_info->maxOutputChannels;
    input_parameters->suggestedLatency = inp_info->defaultLowInputLatency;
    return num_chan;
}


static int init_sample_block(t_audio *data, t_a_snippet *sample_block) {
    PaError err = 0;

    sample_block->size = FRAMES_PER_BUFFER * sizeof(float) * data->n_channels;
    sample_block->snippet = malloc(sample_block->size);
    if(sample_block->snippet == NULL) {
        printf("Could not allocate record array.\n");
        return mx_exit_stream(data, err);
    }
    memset(sample_block->snippet, SAMPLE_SILENCE, sample_block->size);
    return 0;
}

int mx_init_stream(PaStream **stream, t_audio *data,
                   t_a_snippet *sample_block) {
    PaError err;
    PaStreamParameters input_parameters;
    PaStreamParameters output_parameters;

    if (!stream || !data || !sample_block)
        return 1;
    if (init_stream_start(data))
        return 1;
    data->n_channels = set_input_parameters(&input_parameters,
                                                 &output_parameters);
    input_parameters.channelCount = data->n_channels;
    input_parameters.sampleFormat = paFloat32;
    input_parameters.hostApiSpecificStreamInfo = NULL;
    err = Pa_OpenStream(stream, &input_parameters, NULL, data->sample_rate,
                        FRAMES_PER_BUFFER, paClipOff, NULL, NULL);
    if (err)
        return mx_exit_stream(data, err);
    if (init_sample_block(data, sample_block))
        return 1;
    return Pa_StartStream(*stream);
}
