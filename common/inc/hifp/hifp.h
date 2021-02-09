#ifndef HIFP_H
#define HIFP_H

#define PRINT_FPID 1;
#define PRINT_PLAIN_FPID 0;
#define PRINT_DWT 0;

#define NUM_WAVE 131072; /* Number of samples of PCM data */
#define NUM_DWT_ECO 4096; /* Number of bits per FPIDs */
#define NUM_FRAME 128;   /* Number of frames of generated FPID data */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

namespace hifp
{

typedef struct
{
    /* RIFF Header */
    unsigned int riff;      /* RIFF */
    unsigned int size_file; /* 8 minus the size of the entire file */
    unsigned int wave;      /* WAVE */

    /* fmt Chunk */
    unsigned int fmt;             /* fmt */
    unsigned int size_fmt;        /* Number of bytes of fmt channel after format ID */
    unsigned short FormatId;      /* Format ID */
    unsigned short NumChannel;    /* Number of channels */
    unsigned int SampleRate;      /* Sampling frequency */
    unsigned int ByteRate;        /* Number of bytes sampled per second (byte/sec) */
    unsigned short BlockAlign;    /* Number of bytes per block */
    unsigned short BitsPerSample; /* Number of quantization bits */

    /* Data Chunk */
    unsigned int data;      /* DATA */
    unsigned int size_wave; /* Number of bytes of waveform data */
} WAVEHEADER;

#define ERRPRINT(c)                                                    \
    do                                                                 \
    {                                                                  \
        if (errno == 0)                                                \
        {                                                              \
            printf("%s,%d\n", __FILE__, __LINE__);                     \
        }                                                              \
        else                                                           \
        {                                                              \
            printf("%s,%d,%s\n", __FILE__, __LINE__, strerror(errno)); \
        }                                                              \
    } while (0)

#define ASSERT(c)       \
    do                  \
    {                   \
        if (!(c))       \
        {               \
            ERRPRINT(); \
            goto err;   \
        }               \
    } while (0)


/* function prototype */
int readwav8(FILE *fp, short int *wave16, unsigned short int numch);
int dwt1(short int *wave16);
WAVEHEADER read_wave_header(FILE *ifp);
int read_wav_data(FILE *ifp, short int *wav_data, WAVEHEADER wave_header);
int gen_fpid(short int *wave16, unsigned int *fpid, short int *dwt_eco);
short int gen_fpid_2(short int *wave16, short int *fpid, short int *dwt_eco);
short int gen_fpid_3(short int *wave16, short int *fpid, short int *dwt_eco);
int save_fp_to_disk(FILE *ofp, unsigned int *fpid);
void verify_fpid(unsigned int *fpid, unsigned int *plain_fpid, unsigned int *dwt);
// int run_all(FILE *ifp, FILE *ofp);

void init_ref_dwt(short int *ref_dwt);
void init_ref_fpid(unsigned int *ref_fpid);

} // namespace hifp

#endif