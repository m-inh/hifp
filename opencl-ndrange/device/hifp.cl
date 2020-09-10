#define NUMWAVE   131072 /* Number of samples of PCM data */
#define NUMDWTECO 4096   /* Number of bits per FPIDs */
#define NUMFRAME  128    /* Number of frames of generated FPID data */
#define N_DWT 8          /* Number of wave samples when conducting dwt */



__kernel void dwt(
    __global const short int * restrict wave16,
    __global short int * restrict dwteco16
)
{
    int global_id  = get_global_id(0);
    int wave_offset = global_id * 32;

    short int dwteco_tmp[8];

    // Copy wave data from Global to Private
    #pragma unroll
    for (int i=0; i<N_DWT; i++) {
        dwteco_tmp[i] = wave16[wave_offset + i];
    }

    /* 3-stages HAAR wavelet transform */
    #pragma unroll
    for (int i=N_DWT; i>1; i/=2) {
        for (int j=0; j<i/2; j++) {
            dwteco_tmp[j] = (dwteco_tmp[j*2] + dwteco_tmp[j*2 + 1]) / 2;
        }
    }

    dwteco16[global_id] = dwteco_tmp[0];
}



__kernel void generate_fpid(
    __global const short int * restrict dwteco16,
    __global unsigned int * restrict fpid
)
{
    int global_id = get_global_id(0);

    if (global_id < 4095) {
        if (dwteco16[global_id] > dwteco16[global_id + 1]) {
            fpid[global_id] = 1;
        } else {
            fpid[global_id] = 0;
        }
    } else {
        /* The last feature always be 0 */
        fpid[global_id] = 0;
    }
}
