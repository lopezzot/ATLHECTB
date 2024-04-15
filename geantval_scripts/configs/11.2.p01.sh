#!/bin/bash

export G4GCC_VERSION="12"
export G4GCC_PLATFORM="x86_64-el9-gcc12-opt"
export GEANT4_VERSION="11.2"
export GEANT4_PLATFORM="x86_64-el9-gcc12-optdeb-MT"
export GCC_VERSION="12"
export GCC_PLATFORM="x86_64-el9-gcc12-opt"

source /cvmfs/sft.cern.ch/lcg/contrib/gcc/${G4GCC_VERSION}/${G4GCC_PLATFORM}/setup.sh
source /cvmfs/geant4.cern.ch/geant4/${GEANT4_VERSION}/${GEANT4_PLATFORM}/bin/geant4.sh
source /cvmfs/sft.cern.ch/lcg/contrib/gcc/${GCC_VERSION}/${GCC_PLATFORM}/setup.sh

# Test path
GVALPATH="/cvmfs/geant4.cern.ch/opt-geant-val/$GEANT4_VERSION/$GEANT4_PLATFORM/bin:/cvmfs/geant4.cern.ch/opt/$GEANT4_VERSION/$GEANT4_PLATFORM/bin"
YOURPATH="/afs/cern.ch/work/l/lopezzot/Fellow/ATLHECTB/build/"
export PATH="$GVALPATH:$PATH:$YOURPATH"

# Geant Val exports
export VERSION=$GEANT4_VERSION
export PLATFORM=$GEANT4_PLATFORM
