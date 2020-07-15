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

int mx_record_sample(PaStream *stream, t_audio *data,
                     t_sample_block *sample_block, int i) {
    PaError err;
    size_t next_ndex;
    char *destination;

    ++i;
    err = Pa_ReadStream(stream, sample_block->snippet, FRAMES_PER_BUFFER);

    data->size = i * sample_block->size;
    data->recorded_samples = realloc(data->recorded_samples, data->size);

    next_ndex = ((i)- 1) * sample_block->size;
    destination = (char*)data->recorded_samples + next_ndex;
    memcpy(destination, sample_block->snippet, sample_block->size);

    return err;
}

long mx_save_audio(t_audio *data) {
    uint8_t err = SF_ERR_NO_ERROR;
    char file_name[100];
    SF_INFO sfinfo = {
            .channels = data->number_of_channels,
            .samplerate = data->sample_rate,
            .format = SF_FORMAT_AIFF | SF_FORMAT_FLOAT
    };
    SNDFILE *outfile;
    long wr;

    snprintf(file_name, 100, "rec_%d.aif", rand());
    if (!(outfile = sf_open(file_name, SFM_WRITE, &sfinfo))) {
        printf("error outfile =%d\n", sf_error(outfile));
        return -1;
    }
    wr = sf_writef_float(outfile, data->recorded_samples, data->size / 8);
    err = data->size - wr;
    sf_write_sync(outfile);
    sf_close(outfile);
    return err;
}

int mx_init_audio_stream(PaStream **stream, t_audio *data) {
    PaError err = paNoError;
    PaStreamParameters input_parameters;
    PaStreamParameters output_parameters;

    set_input_output_parameters(&input_parameters, &output_parameters, data);

    err = Pa_OpenStream(stream, &input_parameters, &output_parameters,
                        SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, NULL, NULL);
    if (err != paNoError)
        return err;

    return Pa_StartStream(*stream);
}

void mx_record_audio() {
    t_audio *data = mx_init_audio_data();
    t_sample_block *sample_block;
    PaError err = paNoError;
    PaStream *stream;

    if ((err = Pa_Initialize()) != paNoError)
        mx_terminate("Can't initialize PortAudio library");

    if ((err = mx_init_audio_stream(&stream, data)) != paNoError)
        mx_terminate("Can't start audio stream");

    sample_block = mx_init_sample_block(data);

    for (int i = 0; i < (5 * SAMPLE_RATE) / FRAMES_PER_BUFFER; ++i) {
        // ToDo: handle flag from client
        err = mx_record_sample(stream, data, sample_block, i);
    }
    err = Pa_StopStream(stream);
    err = Pa_CloseStream(stream);

    if (data && data->recorded_samples)
        mx_save_audio(data);

    // ToDo: send audio file to server (?)

    Pa_Terminate();
}
