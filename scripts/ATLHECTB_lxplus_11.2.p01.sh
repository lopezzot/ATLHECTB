#!/bin/sh

source /cvmfs/sft.cern.ch/lcg/contrib/gcc/12/x86_64-el9-gcc12-opt/setup.sh 
source /cvmfs/geant4.cern.ch/geant4/11.2.p01/x86_64-el9-gcc12-optdeb-MT/CMake-setup.sh 
export CXX=`which g++`
export CC=`which gcc`
cmake3 -DGeant4_DIR=/cvmfs/geant4.cern.ch/geant4/11.2.p01/x86_64-el9-gcc12-optdeb-MT/lib64/Geant4-11.2.1/ ../ATLHECTB/
make -j$(nproc)
