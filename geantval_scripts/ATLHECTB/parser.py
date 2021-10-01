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

from math import sqrt, fabs
from array import array
from decimal import Decimal
from copy import deepcopy

from ROOT import gROOT, TFile, TTree, TCanvas, gStyle
from ROOT import TH1F, TF1
from gts.utils import getJSON
from gts.BaseParser import BaseParser, mergeROOT, mktempdir

gROOT.SetBatch(True)
gROOT.ProcessLine("gErrorIgnoreLevel = 5000;")

NAMES = {
    "obs": {
        'energy resolution': 'energy resolution'
    },
    "xaxis": {
        'energy resolution': 'E (GeV)'
    },
    "yaxis": {
        'energy resolution': '#sqrt{E_{0}/GeV}#sigma_{E}/E (%)'
    }
}

OBSERVABLE_MAP = {
    "e-energyres": "e-energyres"
}

XAXIS_MAP = {
    "e-energyres": 'E (GeV)'
}

YAXIS_MAP = {
    "e-energyres": 'sigmaE'
}

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
	resolutions = []
	for energy in eenergies:
	    sampfraction = 0
	    #Find e- job with corresponding energy
	    job = [x for x in ectrjobs if float(x["ENERGY"])==energy][0]
	    infile = TFile.Open(os.path.join(job["path"],"ATLHECTBout_Run0.root"))
	    tree = infile.Get("ATLHECTBout")
	    for evt in tree:
		sampfraction += evt.elAr/(energy*1000)*100 #percent value
	    sampfractions.append(sampfraction/tree.GetEntries())
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
	resolutions2 = [2.*x for x in resolutions]
	print "-->e- avg sampling term in resolution: " + str(np.mean(resolutions))
	print jobs[0]["PHYSLIST"]
	rjson = getJSON(jobs[0], "chart",
                              mctool_name="GEANT4",
                              mctool_model=jobs[0]["PHYSLIST"],
                              observableName="e-energyresolution",
                              beamParticle=jobs[0]["PARTICLE"],
                              beamEnergies=eenergies,
                              title="e-energyresolution",
                              xAxisName="energy",
                              yAxisName="sigma",
                              xValues=eenergies,
                              yValues=resolutions
                              )
	yield rjson
	rjson = getJSON(jobs[0], "chart",
                              mctool_name="GEANT4",
                              mctool_model=jobs[0]["PHYSLIST"],
                              observableName="e-energyresolution",
                              beamParticle=jobs[0]["PARTICLE"],
                              beamEnergies=eenergies,
                              title="e-energyresolution",
                              xAxisName="energy",
                              yAxisName="sigma",
                              xValues=eenergies,
                              yValues=resolutions2
                              )
	yield rjson
	'''
	for energy in energies:
	    print "MERGE ENERGY", energy
	    # loop by energies
	    energy_jobs = [x for x in jobs if x["ENERGY"] == energy]
	    if not energy_jobs:
		print """
		===========================
		Cannot find data for energy {0} and combination {1}. Skipping this energy.
		===========================
		""".format(energy, jobs)
		raise RuntimeError("ERROR")
	    else:
		meshroot = []
		ntupleroot = []
		for job in energy_jobs:
		    meshroot.append(os.path.join(job["path"], "mesh.root"))
		    ntupleroot.append(os.path.join(job["path"], "ntuple.root"))
		tmpdir = mktempdir()
		mesh = os.path.join(tmpdir, "mesh.root")
		ntuple = os.path.join(tmpdir, "ntuple.root")
		mergeROOT(meshroot, mesh)
		mergeROOT(ntupleroot, ntuple)
		observables = {}
		observables.update(
		    get_observables_from_mesh(
			os.path.join(mesh), float(energy)
		    )
		)
		observables.update(
		    get_observables_from_ntuples(
			os.path.join(ntuple), float(energy), energy_jobs[0]
		    )
		)
		shutil.rmtree(tmpdir)
		if float(energy) == 500:
		    print "Exporting histograms from ntuple.root"
		    for hhist in [x for x in get_histograms_from_ntuples(ntuple)]:
			rj = getJSON(energy_jobs[0], "histogram",
				       mctool_name="GEANT4",
				       mctool_model=energy_jobs[0]['PHYSLIST'],
				       observableName=hhist['title'],
				       targetName=energy_jobs[0]['DETECTOR'],
				       beamParticle=energy_jobs[0]['PARTICLE'],
				       beamEnergies=[float(energy)],
				       plotType="TH1"
				       )
			rj['histogram'] = hhist
			print "TITLE:", hhist['title']
			common_json.append(rj)
		copycomb = deepcopy(energy_jobs[0])
		copycomb["ENERGY"] = energy
		print "APPEND", [copycomb, observables]
		common_data.append((copycomb, observables))
	print "LEN DATA  =", len(common_data)
	for data in common_data:
	    comb = data[0]
	    if comb["ENERGY"] != sorted(energies)[0]:
		continue
	    for observable in XAXIS_MAP:
		print observable
		xvalues = sorted(map(float, energies))
		yvalues = []
		yerrors = []
		xdata = []
		cdata_multienergy = []
		for xx in common_data:
		    if xx[0]["DETECTOR"] == comb["DETECTOR"] and \
			    xx[0]["PHYSLIST"] == comb["PHYSLIST"] and \
			    xx[0]["VERSION"] == comb["VERSION"] and \
			    xx[0]["PARTICLE"] == comb["PARTICLE"]:
			cdata_multienergy.append(xx)

		for x in xvalues:
		    # find correspondinf Y data
		    ydata = [xx for xx in cdata_multienergy if float(xx[0]["ENERGY"]) == x]
		    if not ydata:
			print "no data found for x:{0}, observable:{1}, comb:{2}".format(x, observable, comb)
			continue
		    ydata = ydata[0]
		    yvalues.append(ydata[1][observable][0])
		    xdata.append(x)
		    yerrors.append(ydata[1][observable][1])
		rjson = getJSON(comb, "chart",
				  mctool_name="GEANT4",
				  mctool_model=comb["PHYSLIST"],
				  testName="simplified calorimeter",
				  plotType="SCATTER2D",
				  observableName=OBSERVABLE_MAP[
				      observable],
				  targetName=comb["DETECTOR"],
				  beamParticle=comb["PARTICLE"],
				  beamEnergies=map(float, xdata),
				  title=OBSERVABLE_MAP[observable],
				  xAxisName=XAXIS_MAP[observable],
				  yAxisName=YAXIS_MAP[observable],
				  xValues=map(float, xdata),
				  yValues=map(float, yvalues),
				  yStatErrorsPlus=map(float, yerrors),
				  yStatErrorsMinus=map(float, yerrors)
				  )
		common_json.append(rjson)
	return common_json
	'''

##**************************************************
