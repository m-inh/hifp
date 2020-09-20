#define NUMWAVE   131072 /* Number of samples of PCM data */
#define NUMDWTECO 4096   /* Number of bits per FPIDs */
#define NUMFRAME  128    /* Number of frames of generated FPID data */
#define N_DWT 8          /* Number of wave samples when conducting dwt */


__kernel void generate_fpid(
    __global const short int * restrict wave,
    __global short int * restrict fpid,
    __local short int * dwtwave
)
{
    int gid  = get_global_id(0);
    int lid = get_local_id(0);
    int group_id = get_group_id(0);
    // int group_size = get_local_size(0);
    
    
    // dwt
    for (int i=0; i<32; i++) {
        int fpid_offset = lid * 32;
        
        for (int j=0; j<32; j++) {
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
    }

    barrier(CLK_LOCAL_MEM_FENCE);


    // feature extraction
    for (int i=0; i<32; i++) {
        int fpid_offset = lid * 32;

        for (int j=0; j<32; j++) {
            if (dwtwave[fpid_offset+j] > dwtwave[fpid_offset+j+1]) {
                fpid[fpid_offset+j] = 1;
            } else {
                fpid[fpid_offset+j] = 0;
            }
        }
    }


}



