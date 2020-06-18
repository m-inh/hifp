#!/bin/bash

echo "Conducting verification"
echo "======================="

echo "Clean fpid directory"
echo "--------------------"
rm -r ./c/fpid/*
rm -r ./opencl/fpid/*

echo "Generate FPID in C"
echo "------------------"
cd ./c && make && bin/host
cd ..

echo "Generate FPID in OpenCL"
echo "-----------------------"
cd ./opencl && make && bin/host
cd ..

echo "-----------------------"
echo " "

gcc compare_bin.c -o cmp
chmod +x cmp
cmp_rs=$(./cmp opencl/fpid/S0000.wav.raw c/fpid/S0000.wav.raw)

if [ $cmp_rs = 'Equal' ]
    then
        echo "OKAY"
    else
        echo "dell OKAY"
fi