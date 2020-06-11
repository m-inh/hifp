#define NUMWAVE   131072 /* Number of samples of PCM data */
#define NUMDWTECO 4096   /* Number of bits per FPIDs */
#define NUMFRAME  128    /* Number of frames of generated FPID data */
#define N_DWT 8 /* Number of wave samples when conducting dwt */

__kernel void dwt(
    __global const short int * restrict wave16,
    __global unsigned int * restrict    dwteco
)
{
    int global_id  = get_global_id(0);
    int wave_offset = global_id * 32;

    short int dwteco_tmp[4];

    /* 3-stages HAAR wavelet transform */
    #pragma unroll
    for (int i=N_DWT; i>1; i/=2) {
        for (int j=0; j<i/2; j++) {
            dwteco_tmp[j] = (dwteco_tmp[j*2] + dwteco_tmp[j*2 + 1]) / 2;
        }
    }

    dwteco[global_id] = dwteco_tmp[0];
}


__kernel void generate_fpid(
    __global const unsigned int * restrict dwteco,
    __global unsigned int * restrict       fpid
)
{
    int global_id     = get_global_id(0);
    int dwteco_offset = global_id * 32;
    int dwteco_index  = 0;
    int i = 0;

    /* Generate FPID */
    if (global_id < NUMFRAME - 1) {
        #pragma unroll
        for (i=0; i<32; i++) {
            dwteco_index = dwteco_offset + i;
            
            fpid[global_id] <<= 1;
            
            if (dwteco[dwteco_index] > dwteco[dwteco_index + 1]) {
                fpid[global_id] |= 1;
            }
        }
    } else {
        #pragma unroll
        for (i=0; i<31; i++) {
            dwteco_index = dwteco_offset + i;
            
            fpid[global_id] <<= 1;
            
            if (dwteco[dwteco_index] > dwteco[dwteco_index + 1]) {
                fpid[global_id] |= 1;
            }
        }

        fpid[global_id] <<= 1;
    }
}
