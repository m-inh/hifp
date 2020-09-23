#!/bin/bash

aoc device/4096_hifp.cl -o bin_4096/hifp.aocx -board=pac_a10
echo "4096 done"

aoc device/2048_hifp.cl -o bin_2048/hifp.aocx -board=pac_a10
echo "2048 done"

aoc device/1024_hifp.cl -o bin_1024/hifp.aocx -board=pac_a10
echo "1024 done"

aoc device/512_hifp.cl -o bin_512/hifp.aocx -board=pac_a10
echo "512 done"

aoc device/256_hifp.cl -o bin_256/hifp.aocx -board=pac_a10
echo "256 done"

aoc device/128_hifp.cl -o bin_128/hifp.aocx -board=pac_a10
echo "128 done"

aoc device/64_hifp.cl -o bin_64/hifp.aocx -board=pac_a10
echo "64 done"

aoc device/32_hifp.cl -o bin_32/hifp.aocx -board=pac_a10
echo "32 done"

aoc device/16_hifp.cl -o bin_16/hifp.aocx -board=pac_a10
echo "16 done"

aoc device/8_hifp.cl -o bin_8/hifp.aocx -board=pac_a10
echo "8 done"

echo "----------------------------"
echo "all done"