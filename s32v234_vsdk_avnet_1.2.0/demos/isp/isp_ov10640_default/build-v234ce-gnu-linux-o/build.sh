#!/bin/bash

BUILD_DIR=$PWD
BUILD_VER="build-v234ce-gnu-linux-o"

#*******************************************************************************          
# build the demo

make allsub

#*******************************************************************************          
# go to ISP graph dir, patch the graph and rebuild it

cd ../../../../isp/graphs/ov10640_default/$BUILD_VER
cp ../src/ov10640_default.c_edit ../src/ov10640_default.c
touch ../src/ov10640_default.c
make

#*******************************************************************************          
# go back to demo build dir and rebuild it

cd $BUILD_DIR
make allsub