uint hifp_rtl(
    __global uint * wave,
    __global uint * fpid,
    uint global_id, 
    uint local_id, 
    uint global_size_0,
    uint workgroup_size
) {
    short int dwt_wave[4097];

    for (int i=0; i<4096; i++) {
        short int wave_tmp[8];
        
        for (int j=0; j<8; j++) {
            wave_tmp[j] = wave[i*32 + j];
        }

        for (int k=8; k>1; k/=2) {
            for (int l=0; l<k/2; l++) {
                wave_tmp[l] = (wave_tmp[l*2] + wave_tmp[l*2 + 1]) / 2;
            }
        }

        dwt_wave[i] = wave_tmp[0];
    }

    for (int i=0; i<4096; i++) {
        if (dwt_wave[i] > dwt_wave[i+1]) {
            fpid[i] = 1;
        }   
    }

    return 0;
}