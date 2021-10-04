##**************************************************
## \file  parser.py
## \brief: Implementation of ATLHECTB parser
##         for geant-val usage
## \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
##          @lopezzot
## \start date: 01 October 2021
##**************************************************

#!/usr/bin/env python

# class for sc test parsing
import os
import sys
import json
import shutil
import time
import numpy as np


from ROOT import gROOT, TFile, TTree, TCanvas, gStyle
from ROOT import TH1F, TF1
from gts.utils import getJSON
from gts.BaseParser import BaseParser, mergeROOT, mktempdir

gROOT.SetBatch(True)
gROOT.ProcessLine("gErrorIgnoreLevel = 5000;")

class Test(BaseParser):
    TEST = "ATLHECTB"
    IGNOREKEYS = ["PARTICLE","ENERGY"] #jobs will contain files for particles (e-,pi-) and energies (GeV)
    
    def parse(self, jobs):
    	eenergies = [20.,40.,50.,80.,100.,119.1,147.8]
	penergies = [20.,30.,40.,50.,60.,80.,100.,120.,150.,180.,200.]
	
	common_data = []
	common_json = []

	#Prepare jobs for analysis
	#
	particles = set([x["PARTICLE"] for x in jobs])
	ectrjobs = [x for x in jobs if x["PARTICLE"]=="e-"]
	ectrenergies = [float(x["ENERGY"]) for x in ectrjobs]
	ectrfiles = [os.path.join(x["path"],"ATLHECTBout_Run0.root") for x in ectrjobs]
	pijobs = [x for x in jobs if x["PARTICLE"]=="pi-"]
	pienergies = [float(x["ENERGY"]) for x in pijobs]
	pifiles = [os.path.join(x["path"],"ATLHECTBout_Run0.root") for x in pijobs]
	print "Found "+str(len(jobs))+" runs in jobs with:"
	print "--->"+ str(len(ectrjobs)) + " jobs with e-, energies (GeV): ", ectrenergies
	print "------> files: ", ectrfiles
	print "--->"+ str(len(pijobs)) + " jobs with pi-, energies (GeV): ", pienergies
	print "------> files: ", pifiles

	#e- analysis
	#
	print "Running e- analysis"
	sampfractions = []
	ersampfractions = []
	resolutions = []
	erresolutions = []
	for energy in eenergies:
	    #Find e- job with corresponding energy
	    job = [x for x in ectrjobs if float(x["ENERGY"])==energy][0]
	    infile = TFile.Open(os.path.join(job["path"],"ATLHECTBout_Run0.root"))
	    tree = infile.Get("ATLHECTBout")
	    H1sampfraction = TH1F("e-","e-",1000,0.,100.)
	    for evt in tree:
		H1sampfraction.Fill(evt.elAr/(energy*1000)*100) #percent value
	    sampfractions.append(H1sampfraction.GetMean())
	    ersampfractions.append(H1sampfraction.GetRMS())
	print "--->e- avg sampling fraction: " + str(np.mean(sampfractions)) + "%"
	for energy in eenergies:
	    #Find e- job with corresponding energy
	    job = [x for x in ectrjobs if float(x["ENERGY"])==energy][0]
	    infile = TFile.Open(os.path.join(job["path"],"ATLHECTBout_Run0.root"))
	    tree = infile.Get("ATLHECTBout")
	    recenergy = TH1F("e-", "e-", 2000, 0., 200.)
	    for evt in tree:
		addchannel = 0
		addchannel += evt.M2L1BirkeLayer[2]
		addchannel += evt.M2L1BirkeLayer[3]
		addchannel += evt.M2L1BirkeLayer[5]
		addchannel += evt.M2L2BirkeLayer[3]
		addchannel += evt.M2L2BirkeLayer[5]
		addchannel += evt.M3L1BirkeLayer[3]
		addchannel += evt.M3L2BirkeLayer[5]
		recenergy.Fill(addchannel/(10.*np.mean(sampfractions)))
	    xfitmin = recenergy.GetXaxis().GetBinCenter(recenergy.GetMaximumBin())-1.2*recenergy.GetStdDev();
	    xfitmax = recenergy.GetXaxis().GetBinCenter(recenergy.GetMaximumBin())+1.2*recenergy.GetStdDev();
	    F1recenergy = TF1("gaus","gaus(0)",xfitmin,xfitmax);
	    recenergy.Fit( F1recenergy ,"QR");
	    res =(recenergy.GetFunction("gaus").GetParameter(2)/energy)*(energy**(0.5))*100. #percent
	    resolutions.append(res)
	    erres = (recenergy.GetFunction("gaus").GetParError(2)/
		     recenergy.GetFunction("gaus").GetParameter(2))*res
	    erresolutions.append(erres)
	print "--->e- avg sampling term in resolution: " + str(np.mean(resolutions)) + " %GeV^{1/2}" 

        #Create JSON output files for e- energy resolution (graph)
        #
	yield getJSON(jobs[0], "chart",
                              mctool_name="GEANT4",
                              mctool_model=jobs[0]["PHYSLIST"],
                              observableName="e-energyresolution",
                              beamParticle=job["PARTICLE"],
                              beamEnergies=eenergies,
                              title="energyresolution(e-)",
                              xAxisName="<E_{Beam}> [GeV]",
                              yAxisName="#sigma_{0}/E_{0} #sqrt{E_{Beam}} [% #sqrt{GeV}]",
                              xValues=eenergies,
                              yValues=resolutions,
	                      yStatErrorsMinus=erresolutions,
			      yStatErrorPlus=erresolutions
                              )

        #Create JSON output files for e- sampling fraction (graph)
        #
	yield getJSON(jobs[0], "chart",
                              mctool_name="GEANT4",
                              mctool_model=jobs[0]["PHYSLIST"],
                              observableName="e-samplingfraction",
                              beamParticle=job["PARTICLE"],
                              beamEnergies=eenergies,
                              title="samplingfraction(e-)",
                              xAxisName="<E_{Beam}> [GeV]",
                              yAxisName="%",
                              xValues=eenergies,
                              yValues=sampfractions,
	                      yStatErrorsMinus=ersampfractions,
			      yStatErrorPlus=ersampfractions
                              )

	#pi- analysis
	#
	print "Running pi- analysis"
	responses = []
	erresponses = []
	resolutions = []
	erresolutions = []
	F1 = []
	F2 = []
	F3 = []
	F4 = []
	L0 = []
	sigmaL0 = []
	for energy in pienergies:
	    #Find pi- job with corresponding energy
	    job = [x for x in pijobs if float(x["ENERGY"])==energy][0]
	    infile = TFile.Open(os.path.join(job["path"],"ATLHECTBout_Run0.root"))
	    tree = infile.Get("ATLHECTBout")
	    recenergy = TH1F("pi-", "e-", 2000, 0., 200.)
	    response = TH1F("pi-", "pi-", 2*120, 0., 1.)
	    for evt in tree:
		addchannel = 0
		addchannelF1 = 0
		addchannelF2 = 0
                addchannelF3 = 0
		addchannelF4 = 0

		addchannel += evt.M2L1BirkeLayer[0] #M2L1
		addchannel += evt.M2L1BirkeLayer[1]
		addchannel += evt.M2L1BirkeLayer[2]
		addchannel += evt.M2L1BirkeLayer[3]
		addchannel += evt.M2L1BirkeLayer[4]
		addchannel += evt.M2L1BirkeLayer[5]
		addchannelF1 += evt.M2L1BirkeLayer[0] #M2L1
		addchannelF1 += evt.M2L1BirkeLayer[1]
		addchannelF1 += evt.M2L1BirkeLayer[2]
		addchannelF1 += evt.M2L1BirkeLayer[3]
		addchannelF1 += evt.M2L1BirkeLayer[4]
		addchannelF1 += evt.M2L1BirkeLayer[5]

		addchannel += evt.M3L1BirkeLayer[1] #M3L1
		addchannel += evt.M3L1BirkeLayer[3]
		addchannel += evt.M3L1BirkeLayer[5]
		addchannelF1 += evt.M3L1BirkeLayer[1] #M3L1
		addchannelF1 += evt.M3L1BirkeLayer[3]
		addchannelF1 += evt.M3L1BirkeLayer[5]

		addchannel += evt.M2L2BirkeLayer[2] #M2L2
		addchannel += evt.M2L2BirkeLayer[3] 
		addchannel += evt.M2L2BirkeLayer[4]
		addchannel += evt.M2L2BirkeLayer[5]
		addchannel += evt.M2L2BirkeLayer[6]
		addchannel += evt.M2L2BirkeLayer[7]
		addchannel += evt.M2L2BirkeLayer[8]
		addchannel += evt.M2L2BirkeLayer[9]
		addchannelF2 += evt.M2L2BirkeLayer[2] #M2L2
		addchannelF2 += evt.M2L2BirkeLayer[3] 
		addchannelF2 += evt.M2L2BirkeLayer[4]
		addchannelF2 += evt.M2L2BirkeLayer[5]
		addchannelF2 += evt.M2L2BirkeLayer[6]
		addchannelF2 += evt.M2L2BirkeLayer[7]
		addchannelF2 += evt.M2L2BirkeLayer[8]
		addchannelF2 += evt.M2L2BirkeLayer[9]

		addchannel += evt.M1L2BirkeLayer[2] #M1L2
		addchannel += evt.M1L2BirkeLayer[4]
		addchannel += evt.M1L2BirkeLayer[6]
		addchannel += evt.M1L2BirkeLayer[8]
		addchannelF2 += evt.M1L2BirkeLayer[2] #M1L2
		addchannelF2 += evt.M1L2BirkeLayer[4]
		addchannelF2 += evt.M1L2BirkeLayer[6]
		addchannelF2 += evt.M1L2BirkeLayer[8]

		addchannel += evt.M3L2BirkeLayer[3] #M3L2
		addchannel += evt.M3L2BirkeLayer[5]
		addchannel += evt.M3L2BirkeLayer[7]
		addchannel += evt.M3L2BirkeLayer[9]
		addchannelF2 += evt.M3L2BirkeLayer[3] #M3L2
		addchannelF2 += evt.M3L2BirkeLayer[5]
		addchannelF2 += evt.M3L2BirkeLayer[7]
		addchannelF2 += evt.M3L2BirkeLayer[9]

		addchannel += 2.*evt.M2L3BirkeLayer[4] #M2L3 
		addchannel += 2.*evt.M2L3BirkeLayer[5] 
		addchannel += 2.*evt.M2L3BirkeLayer[6] 
		addchannel += 2.*evt.M2L3BirkeLayer[7] 
		addchannel += 2.*evt.M2L3BirkeLayer[8] 
		addchannel += 2.*evt.M2L3BirkeLayer[9] 
		addchannel += 2.*evt.M2L3BirkeLayer[10] 
		addchannel += 2.*evt.M2L3BirkeLayer[11] 
		addchannelF3 += 2.*evt.M2L3BirkeLayer[4] #M2L3 
		addchannelF3 += 2.*evt.M2L3BirkeLayer[5] 
		addchannelF3 += 2.*evt.M2L3BirkeLayer[6] 
		addchannelF3 += 2.*evt.M2L3BirkeLayer[7] 
		addchannelF3 += 2.*evt.M2L3BirkeLayer[8] 
		addchannelF3 += 2.*evt.M2L3BirkeLayer[9] 
		addchannelF3 += 2.*evt.M2L3BirkeLayer[10] 
		addchannelF3 += 2.*evt.M2L3BirkeLayer[11] 

		addchannel += 2.*evt.M1L3BirkeLayer[4] #M1L3 
		addchannel += 2.*evt.M1L3BirkeLayer[6] 
		addchannel += 2.*evt.M1L3BirkeLayer[8] 
		addchannel += 2.*evt.M1L3BirkeLayer[10] 
		addchannelF3 += 2.*evt.M1L3BirkeLayer[4] #M1L3 
		addchannelF3 += 2.*evt.M1L3BirkeLayer[6] 
		addchannelF3 += 2.*evt.M1L3BirkeLayer[8] 
		addchannelF3 += 2.*evt.M1L3BirkeLayer[10] 

		addchannel += 2.*evt.M3L3BirkeLayer[5] #M3L3
		addchannel += 2.*evt.M3L3BirkeLayer[7] 
		addchannel += 2.*evt.M3L3BirkeLayer[9] 
		addchannel += 2.*evt.M3L3BirkeLayer[11] 
		addchannelF3 += 2.*evt.M3L3BirkeLayer[5] #M3L3
		addchannelF3 += 2.*evt.M3L3BirkeLayer[7] 
		addchannelF3 += 2.*evt.M3L3BirkeLayer[9] 
		addchannelF3 += 2.*evt.M3L3BirkeLayer[11] 

		addchannel += 2.*evt.M2L4BirkeLayer[6] #M2L4 
		addchannel += 2.*evt.M2L4BirkeLayer[7] 
		addchannel += 2.*evt.M2L4BirkeLayer[8] 
		addchannel += 2.*evt.M2L4BirkeLayer[9] 
		addchannel += 2.*evt.M2L4BirkeLayer[10] 
		addchannel += 2.*evt.M2L4BirkeLayer[11] 
		addchannelF4 += 2.*evt.M2L4BirkeLayer[6] #M2L4 
		addchannelF4 += 2.*evt.M2L4BirkeLayer[7] 
		addchannelF4 += 2.*evt.M2L4BirkeLayer[8] 
		addchannelF4 += 2.*evt.M2L4BirkeLayer[9] 
		addchannelF4 += 2.*evt.M2L4BirkeLayer[10] 
		addchannelF4 += 2.*evt.M2L4BirkeLayer[11] 

		addchannel += 2.*evt.M3L4BirkeLayer[7] #M3L4 
		addchannel += 2.*evt.M3L4BirkeLayer[9] 
		addchannel += 2.*evt.M3L4BirkeLayer[11] 
		addchannelF4 += 2.*evt.M3L4BirkeLayer[7] #M3L4 
		addchannelF4 += 2.*evt.M3L4BirkeLayer[9] 
		addchannelF4 += 2.*evt.M3L4BirkeLayer[11] 

		response.Fill((addchannel/energy)/(10.*np.mean(sampfractions)))
		recenergy.Fill(addchannel/(10.*np.mean(sampfractions)))
	
	    responses.append(response.GetMean())
	    erresponses.append(response.GetRMS())
	    recenergy.Fit("gaus","Q")
	    res = 100.*recenergy.GetFunction("gaus").GetParameter(2)/recenergy.GetFunction("gaus").GetParameter(1)
	    resolutions.append(res)
            erres = (recenergy.GetFunction("gaus").GetParError(2)/recenergy.GetFunction("gaus").GetParameter(2)+
	            recenergy.GetFunction("gaus").GetParError(1)/recenergy.GetFunction("gaus").GetParameter(1))*res
	    erresolutions.append(erres)
	    F1.append(addchannelF1/addchannel)
	    F2.append(addchannelF2/addchannel)
	    F3.append(addchannelF3/addchannel)
	    F4.append(addchannelF4/addchannel)
	    F1.append(addchannelF1/addchannel)
	    L0.append((28.05/2.)*addchannelF1/addchannel+
		      (28.05+53.6/2.)*addchannelF2/addchannel+
	              (28.05+53.6+53.35/2.)*addchannelF3/addchannel+
		      (28.05+53.6+53.35+46.8/2)*addchannelF4/addchannel)

	    depths = [28.05/2.,28.05+53.6/2.,28.05+53.6+53.35/2.,28.05+53.6+53.35+46.8/2]
	    residual = (depths[0]*addchannelF1/addchannel)**2+(depths[1]*addchannelF2/addchannel)**2+(depths[2]*addchannelF3/addchannel)**2+(depths[3]*addchannelF4/addchannel)**2
	    sigmaL0.append(2.*(residual/4.)**0.5) 
        
	#Create JSON output files for pi- energy resolution (graph)
        #
	yield getJSON(jobs[0], "chart",
                              mctool_name="GEANT4",
                              mctool_model=jobs[0]["PHYSLIST"],
                              observableName="pi-energyresolution",
                              beamParticle=job["PARTICLE"],
                              beamEnergies=pienergies,
                              title="energyresolution(pi-)",
                              xAxisName="<E_{Beam}> [GeV]",
                              yAxisName="#sigma_{0}/E_{0} [%]",
                              xValues=pienergies,
                              yValues=resolutions,
	                      yStatErrorsMinus=erresolutions,
			      yStatErrorsPlus=erresolutions
                              )

        #Create JSON output files for pi/e response ratio (graph)
        #
	yield getJSON(jobs[0], "chart",
                              mctool_name="GEANT4",
                              mctool_model=jobs[0]["PHYSLIST"],
                              observableName="#pi/e",
                              beamParticle=job["PARTICLE"],
                              beamEnergies=pienergies,
                              title="#pi/e",
                              xAxisName="<E_{Beam}> [GeV]",
                              yAxisName="#pi/E",
                              xValues=pienergies,
                              yValues=responses,
	                      yStatErrorsMinus=erresponses,
			      yStatErrorsPlus=erresponses
                              )

        #Create JSON output files for L0 (graph)
        #
	yield getJSON(jobs[0], "chart",
                              mctool_name="GEANT4",
                              mctool_model=jobs[0]["PHYSLIST"],
                              observableName="L_{0}",
                              beamParticle=job["PARTICLE"],
                              beamEnergies=pienergies,
                              title="L_{0}",
                              xAxisName="<E_{Beam}> [GeV]",
                              yAxisName="L_{0} [cm]",
                              xValues=pienergies,
                              yValues=L0
	                      #yStatErrorsMinus=,
			      #yStatErrorsPlus=
                              )

        #Create JSON output files for sigmaL0 (graph)
        #
	yield getJSON(jobs[0], "chart",
                              mctool_name="GEANT4",
                              mctool_model=jobs[0]["PHYSLIST"],
                              observableName="#sigma_{L}",
                              beamParticle=job["PARTICLE"],
                              beamEnergies=pienergies,
                              title="#sigma_{L}",
                              xAxisName="<E_{Beam}> [GeV]",
                              yAxisName="#sigma_{L} [cm]",
                              xValues=pienergies,
                              yValues=sigmaL0
	                      #yStatErrorsMinus=,
			      #yStatErrorsPlus=
                              )

##**************************************************
