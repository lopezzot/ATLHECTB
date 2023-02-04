#!/bin/bash

VERSION="11.0.p03"
PLATFORM="x86_64-centos7-gcc8-optdeb"


# Geant4 libraries
source /cvmfs/geant4.cern.ch/geant4/$VERSION/${PLATFORM}/bin/geant4.sh
[ -e /cvmfs/geant4.cern.ch/geant4/$VERSION/setup_g4datasets.sh ] && source /cvmfs/geant4.cern.ch/geant4/$VERSION/setup_g4datasets.sh

# Test path
export PATH="/cvmfs/geant4.cern.ch/opt-geant-val/$VERSION/$PLATFORM/bin:/cvmfs/geant4.cern.ch/opt/$VERSION/$PLATFORM/bin:$PATH:/afs/cern.ch/work/l/lopezzot/Fellow/ATLHECTB/build1103/"

# Compiler
source /cvmfs/sft.cern.ch/lcg/contrib/gcc/8/x86_64-centos7/setup.sh

# Externals

#export LD_LIBRARY_PATH="/cvmfs/sft.cern.ch/lcg/releases/LCG_88/ROOT/6.08.06/x86_64-centos7-gcc62-opt/lib:/cvmfs/sft.cern.ch/lcg/releases/LCG_88/qt5/5.6.0/x86_64-centos7-gcc62-opt/lib/:/cvmfs/sft.cern.ch/lcg/releases/LCG_88/hdf5/1.8.18/x86_64-centos7-gcc62-opt/lib/:$LD_LIBRARY_PATH"
