#!/bin/bash

aoc device/hifp_4096.cl -o bin_4096/hifp.aocx -board=pac_a10
echo "4096 done"

aoc device/hifp_2048.cl -o bin_2048/hifp.aocx -board=pac_a10
echo "2048 done"

aoc device/hifp_1024.cl -o bin_1024/hifp.aocx -board=pac_a10
echo "1024 done"

aoc device/hifp_512.cl -o bin_512/hifp.aocx -board=pac_a10
echo "512 done"

aoc device/hifp_256.cl -o bin_256/hifp.aocx -board=pac_a10
echo "256 done"

aoc device/hifp_128.cl -o bin_128/hifp.aocx -board=pac_a10
echo "128 done"

aoc device/hifp_64.cl -o bin_64/hifp.aocx -board=pac_a10
echo "64 done"

aoc device/hifp_32.cl -o bin_32/hifp.aocx -board=pac_a10
echo "32 done"

aoc device/hifp_16.cl -o bin_16/hifp.aocx -board=pac_a10
echo "16 done"

aoc device/hifp_8.cl -o bin_8/hifp.aocx -board=pac_a10
echo "8 done"

echo "----------------------------"
echo "all done"