#define NUMWAVE   131072 /* Number of samples of PCM data */
#define NUMDWTECO 4096   /* Number of bits per FPIDs */
#define NUMFRAME  128    /* Number of frames of generated FPID data */
#define N_DWT 8          /* Number of wave samples when conducting dwt */


// Function prototype
short int dwt(short int * wave16);

short int dwt(short int * wave16) {
    for (int i=N_DWT; i>1; i/=2) {
        for (int j=0; j<i/2; j++) {
            wave16[j] = (wave16[j*2] + wave16[j*2 + 1]) / 2;
        }
    }

    return wave16[0];
}

__kernel void generate_fpid(
    __global const short int * restrict wave16,
    __global unsigned int * restrict fpid
) {
    short int low_wave[NUMDWTECO+1];

    for (int i=0; i<NUMDWTECO; i++) {
        short int wave[8];
        for (int j=0; j<8; j++) {
            wave[j] = wave16[i*32 + j];
        }

        low_wave[i] = dwt(wave);
    }

    for (int i=0; i<NUMFRAME; i++) {
        fpid[i] = 0;
        
        for (int j=0; j<32; j++) {
            fpid[i] <<= 1;

            if (low_wave[i*32 + j] > low_wave[i*32 + j + 1]){
                fpid[i] |= 1;
            }
        }
    }
}




