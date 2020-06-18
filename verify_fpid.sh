#!/bin/bash

echo "Conducting verification"
echo "======================="


echo "Generate FPID in C"
echo "------------------"
rm -r ./c/fpid/*
cd ./c && make && bin/host
cd ..


# Build compare program
gcc compare_bin.c -o cmp
chmod +x cmp



# Compare 2 FPIDs
echo "Generate FPID in OpenCL-ndrange"
echo "-----------------------"
rm -r ./opencl-ndrange/fpid/*
cd ./opencl-ndrange && make && bin/host
cd ..

cmp_rs=$(./cmp opencl-ndrange/fpid/S0000.wav.raw c/fpid/S0000.wav.raw)

echo "-----------------------"
echo " "

if [ $cmp_rs = 'Equal' ]
    then
        echo "OKAY"
    else
        echo "dell OKAY"
fi



# Compare 2 FPIDs
echo "Generate FPID in OpenCL-single"
echo "-----------------------"
rm -r ./opencl-single/fpid/*
cd ./opencl-single && make && bin/host
cd ..

echo "-----------------------"
echo " "

cmp_rs=$(./cmp opencl-single/fpid/S0000.wav.raw c/fpid/S0000.wav.raw)

echo "-----------------------"
echo " "

if [ $cmp_rs = 'Equal' ]
    then
        echo "OKAY"
    else
        echo "dell OKAY"
fi