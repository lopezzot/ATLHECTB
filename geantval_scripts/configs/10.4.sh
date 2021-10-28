
#!/bin/bash

VERSION="10.4"
PLATFORM="x86_64-slc6-gcc63-opt"


# Geant4 libraries
source /cvmfs/geant4.cern.ch/geant4/$VERSION/$PLATFORM/bin/geant4.sh
[ -e /cvmfs/geant4.cern.ch/geant4/$VERSION/setup_g4datasets.sh ] && source /cvmfs/geant4.cern.ch/geant4/$VERSION/setup_g4datasets.sh

# Test path
export PATH="/cvmfs/geant4.cern.ch/opt-geant-val/$VERSION/$PLATFORM/bin:/cvmfs/geant4.cern.ch/opt/$VERSION/$PLATFORM/bin:$PATH:/afs/cern.ch/work/l/lopezzot/Fellow/geant-val/prova/build10.4/"

# Compiler
source /cvmfs/sft.cern.ch/lcg/external/gcc/6.2/x86_64-slc6/setup.sh
#source /cmnfs/sft.cern.ch/lcg/contrib/gcc/6.3/x86_64-slc6/setup.sh
# Externals

export LD_LIBRARY_PATH="/cvmfs/sft.cern.ch/lcg/releases/LCG_88/ROOT/6.08.06/x86_64-slc6-gcc62-opt/lib:/cvmfs/sft.cern.ch/lcg/releases/LCG_88/qt5/5.6.0/x86_64-slc6-gcc62-opt/lib/:/cvmfs/sft.cern.ch/lcg/releases/LCG_88/hdf5/1.8.18/x86_64-slc6-gcc62-opt/lib/:$LD_LIBRARY_PATH"
