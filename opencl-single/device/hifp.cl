#define NUMWAVE   131072 /* Number of samples of PCM data */
#define NUMDWTECO 4096   /* Number of bits per FPIDs */
#define NUMFRAME  128    /* Number of frames of generated FPID data */
#define N_DWT 8          /* Number of wave samples when conducting dwt */



__kernel void generate_fpid(
    __global const short int * restrict wave16,
    __global unsigned int * restrict fpid
)
{
    for (int i=0; i<NUMFRAME; i++) {
        fpid[i] = 11;
    }
}




