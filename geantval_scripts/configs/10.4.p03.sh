
#!/bin/bash

VERSION="10.4.p03"
PLATFORM="x86_64-centos7-gcc63-opt-MT"


# Geant4 libraries
source /cvmfs/geant4.cern.ch/geant4/$VERSION/$PLATFORM/bin/geant4.sh
[ -e /cvmfs/geant4.cern.ch/geant4/$VERSION/setup_g4datasets.sh ] && source /cvmfs/geant4.cern.ch/geant4/$VERSION/setup_g4datasets.sh

# Test path
export PATH="/afs/cern.ch/user/d/dkonst/work/public/GATE/install/bin/:/cvmfs/geant4.cern.ch/opt-geant-val/$VERSION/$PLATFORM/bin:/cvmfs/geant4.cern.ch/opt/$VERSION/$PLATFORM/bin:$PATH:/afs/cern.ch/work/l/lopezzot/Fellow/ATLHECTB/build1043/"

# Compiler
source /cvmfs/sft.cern.ch/lcg/external/gcc/6.2/x86_64-centos7/setup.sh

# Externals

export LD_LIBRARY_PATH="/cvmfs/sft.cern.ch/lcg/releases/LCG_88/ROOT/6.08.06/x86_64-centos7-gcc62-opt/lib:/cvmfs/sft.cern.ch/lcg/releases/LCG_88/qt5/5.6.0/x86_64-centos7-gcc62-opt/lib/:/cvmfs/sft.cern.ch/lcg/releases/LCG_88/hdf5/1.8.18/x86_64-centos7-gcc62-opt/lib/:$LD_LIBRARY_PATH"
