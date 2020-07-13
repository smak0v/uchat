#include "uchat.h"

int mx_record(PaStream *stream, t_audio *data, t_a_snippet *sample_block,
              t_client_info *info) {
    int err = 0;
    int j = 0;
    int i;

    printf("Wire on. Will run %d seconds.\n", NUM_SECONDS);
    fflush(stdout);
    for (i = 0; i < (NUM_SECONDS * SAMPLE_RATE) / FRAMES_PER_BUFFER; ++i) {
        if (info->can_record == 1)
            break;
        err = mx_process_stream(stream, data, sample_block, &j);
    }
    mx_save_audio(data);
    fflush(stdout);
    err = Pa_StopStream(stream);
    if (err != paNoError)
        mx_exit_stream(data, err);
    return err;
}

char *mx_record_audio(t_client_info *info) {
    t_audio *data = init_audio_data();
    t_a_snippet *sample_block = malloc(sizeof(t_a_snippet));
    PaError err = paNoError;

    sample_block->snippet = NULL;
    sample_block->size = 0;
    PaStream *stream = NULL;
    printf(" start record\n");
    err = mx_init_stream(&stream, data, sample_block);
    if (err) {
        return NULL;
    }
    err = mx_record(stream, data, sample_block, info);
    if (err != 0)
        printf("err =%d\n", err);
    printf("exit record\n");
    printf("record to file->%s\n", data->file_name);
    info->can_record = 1;
    return data->file_name;
}


t_audio * init_audio_data(void) {
    t_audio *data = malloc(sizeof(t_audio));
    data->format_type = paFloat32;
    data->n_channels = 0;
    data->sample_rate = SAMPLE_RATE;
    data->size = 0;
    data->rec_samples = NULL;
    return data;
}

int mx_exit_stream(t_audio *data, PaError err) {
    Pa_Terminate();
    if (data->rec_samples)
        free (data->rec_samples);
    if (err != paNoError) {
        fprintf(stderr, "Error occured while using the portaudio stream\n");
        fprintf(stderr, "Error number: %d\n", err);
        fprintf(stderr, "Error message: %s\n", Pa_GetErrorText( err ));
        err = 1;
    }
    return err;
}

long mx_save_audio(t_audio *data) {
    uint8_t err = SF_ERR_NO_ERROR;
    SF_INFO sfinfo ={
            .channels = data->n_channels,
            .samplerate = data->sample_rate,
            .format = SF_FORMAT_AIFF | SF_FORMAT_FLOAT
    };
    char file_name[100];

    snprintf(file_name, 100, "./Uchat_downloads/rec_%d.aif", rand());
    SNDFILE *outfile = sf_open(file_name, SFM_WRITE, &sfinfo);
    if (!outfile) {
        printf("error outfile =%d\n", sf_error(outfile));
        return -1;
    }
    long wr = sf_writef_float(outfile, data->rec_samples, data->size / 8);
    err = data->size - wr;
    sf_write_sync(outfile);
    sf_close(outfile);
    data->file_name = strdup(file_name);
    return err;
}

