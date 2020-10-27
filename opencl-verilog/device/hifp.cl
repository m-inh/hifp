short int hifp_rtl(
    __global const short int * wave, 
    __global short int * fpid
);

__kernel void generate_fpid(
    __global const short int * restrict wave,
    __global short int * restrict fpid
) {
    hifp_rtl(wave, fpid);
}

// __kernel void generate_fpid_lib(
//     __global const short int * restrict wave,
//     __global short int * restrict fpid
// ) {
//     // fpid = my_hifp(wave);
// }