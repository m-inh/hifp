__attribute__ ((reqd_work_group_size(16, 1, 1)))
__kernel void generate_fpid(
    __global const short int * restrict wave,
    __global short int * restrict fpid
)
{
    int lid = get_local_id(0);
    
    __local short int dwtwave[4097];    
    
    int fpid_offset = lid * 256;
    
    // dwt    
    for (int i=0; i<256; i++) {
        int wave_offset = (fpid_offset + i) * 32;

        short int wave_tmp[8];
        
        for (int j=0; j<8; j++) {
            wave_tmp[j] = wave[wave_offset + j];
        }

        /* 3-stages HAAR wavelet transform */
        for (int k=8; k>1; k/=2) {
            for (int l=0; l<k/2; l++) {
                wave_tmp[l] = (wave_tmp[l*2] + wave_tmp[l*2 + 1]) / 2;
            }
        }

        dwtwave[fpid_offset+i] = wave_tmp[0];
    }

    barrier(CLK_LOCAL_MEM_FENCE);


    // feature extraction
    for (int i=0; i<256; i++) {
        if (dwtwave[fpid_offset+i] > dwtwave[fpid_offset+i+1]) {
            fpid[fpid_offset+i] = 1;
        } else {
            fpid[fpid_offset+i] = 0;
        }
    }

}



