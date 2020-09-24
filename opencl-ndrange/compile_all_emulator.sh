#!/bin/bash

# aoc -march=emulator device/hifp_4096.cl -o bin_4096/hifp.aocx
# echo "4096 done"
# echo "----------------------------"

# aoc -march=emulator device/hifp_2048.cl -o bin_2048/hifp.aocx
# echo "2048 done"
# echo "----------------------------"

# aoc -march=emulator device/hifp_1024.cl -o bin_1024/hifp.aocx
# echo "1024 done"
# echo "----------------------------"

aoc -march=emulator device/hifp_512.cl -o bin_512/hifp.aocx
echo "512 done"
echo "----------------------------"

aoc -march=emulator device/hifp_256.cl -o bin_256/hifp.aocx
echo "256 done"
echo "----------------------------"

aoc -march=emulator device/hifp_128.cl -o bin_128/hifp.aocx
echo "128 done"
echo "----------------------------"

aoc -march=emulator device/hifp_64.cl -o bin_64/hifp.aocx
echo "64 done"
echo "============================"

echo "all done"