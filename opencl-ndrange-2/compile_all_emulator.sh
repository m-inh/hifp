#!/bin/bash

aoc -march=emulator device/4096_hifp.cl -o bin_4096/hifp.aocx
echo "4096 done"

aoc -march=emulator device/2048_hifp.cl -o bin_2048/hifp.aocx
echo "2048 done"

aoc -march=emulator device/1024_hifp.cl -o bin_1024/hifp.aocx
echo "1024 done"

aoc -march=emulator device/512_hifp.cl -o bin_512/hifp.aocx
echo "512 done"

aoc -march=emulator device/256_hifp.cl -o bin_256/hifp.aocx
echo "256 done"

aoc -march=emulator device/128_hifp.cl -o bin_128/hifp.aocx
echo "128 done"

aoc -march=emulator device/64_hifp.cl -o bin_64/hifp.aocx
echo "64 done"

aoc -march=emulator device/32_hifp.cl -o bin_32/hifp.aocx
echo "32 done"

aoc -march=emulator device/16_hifp.cl -o bin_16/hifp.aocx
echo "16 done"

aoc -march=emulator device/8_hifp.cl -o bin_8/hifp.aocx
echo "8 done"

echo "----------------------------"
echo "all done"