// Copyright (C) 2013-2018 Altera Corporation, San Jose, California, USA. All rights reserved.
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to
// whom the Software is furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 
// This agreement shall be governed in all respects by the laws of the State of California and
// by the laws of the United States of America.

// Before compiling this kernel, create opencl_lib.aoclib with:
//	  cd device/lib1 && aocl library hdl-comp-pkg opencl_lib.xml -o opencl_lib.aoco
//	  cd device/lib1 && aocl library create -name opencl_lib opencl_lib.aoco
//	  cd device/lib1 && cp opencl_lib.aoclib ..
// Then compile this kernel with:
//    aoc -l opencl_lib.aoclib -L lib1 -I lib1 hifp.cl

short int my_hifp(__global const short int * wave, __global short int * fpid);

__kernel void generate_fpid(
    __global const short int * restrict wave,
    __global short int * restrict fpid
) {
    my_hifp(wave, fpid);
}

__kernel void generate_fpid_lib(
    __global const short int * restrict wave,
    __global short int * restrict fpid
) {
    // fpid = my_hifp(wave);
}