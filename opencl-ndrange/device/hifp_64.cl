#define GROUP_SIZE 64

__attribute__ ((reqd_work_group_size(GROUP_SIZE, 1, 1)))
__kernel void generate_fpid(
    __global const short int * restrict wave,
    __global short int * restrict fpid
)
{
    int lid = get_local_id(0);
    int num_chunks = 4096 / GROUP_SIZE;
    __local short int dwtwave[4097];
    int fpid_offset = lid * num_chunks;
    
    // dwt
    for (int j=0; j<num_chunks; j++) {
        int wave_offset = (fpid_offset + j) * 32;

        short int dwtwave_tmp[8];
        
        #pragma unroll
        for (int k=0; k<8; k++) {
            dwtwave_tmp[k] = wave[wave_offset + k];
        }

        /* 3-stages HAAR wavelet transform */
        #pragma unroll
        for (int k=8; k>1; k/=2) {
            for (int l=0; l<k/2; l++) {
                dwtwave_tmp[l] = (dwtwave_tmp[l*2] + dwtwave_tmp[l*2 + 1]) / 2;
            }
        }

        dwtwave[fpid_offset+j] = dwtwave_tmp[0];
    }

    barrier(CLK_LOCAL_MEM_FENCE);


    // feature extraction
    for (int j=0; j<num_chunks; j++) {
        if (dwtwave[fpid_offset+j] > dwtwave[fpid_offset+j+1]) {
            fpid[fpid_offset+j] = 1;
        } else {
            fpid[fpid_offset+j] = 0;
        }
    }

}



