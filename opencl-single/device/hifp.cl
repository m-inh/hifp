__kernel void generate_fpid(
    __global const short int * restrict wave,
    __global short int * restrict fpid
) {
    short int dwt_wave[4097];

    #pragma unroll
    for (int i=0; i<4096; i++) {
        short int wave_tmp[8];
        
        #pragma unroll
        for (int j=0; j<8; j++) {
            wave_tmp[j] = wave[i*32 + j];
        }

        #pragma unroll
        for (int k=8; k>1; k/=2) {
            #pragma unroll
            for (int l=0; l<k/2; l++) {
                wave_tmp[l] = (wave_tmp[l*2] + wave_tmp[l*2 + 1]) / 2;
            }
        }

        dwt_wave[i] = wave_tmp[0];
    }

    #pragma unroll
    for (int i=0; i<4096; i++) {
        if (dwt_wave[i] > dwt_wave[i+1]) {
            fpid[i] = 1;
        }   
    }
}
