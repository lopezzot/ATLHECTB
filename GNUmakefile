##**************************************************
## \file GNUmakefile
## \brief: GNUmakefile of G4ATLHECTB project
## \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
## \start date: 11 May 2021
##**************************************************

name := ATLHECTB
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
    G4INSTALL == ../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*

##**************************************************
