# ****************************************************
# \file  parser.py
# \brief: Implementation of ATLHECTB parser
#         for geant-val usage
# \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
#          @lopezzot
# \start date: 01 October 2021
# ****************************************************

#!/usr/bin/env python

# class for test parsing
import os
import sys
import json
import numpy as np

from ROOT import gROOT, TFile, TTree, TCanvas, gStyle
from ROOT import TH1F, TF1
from gts.utils import getJSON
from gts.BaseParser import BaseParser, mergeROOT, mktempdir


class Test(BaseParser):
    TEST = "ATLHECTB"
    # jobs will contain files for particles (e-,pi-) and energies (GeV)
    IGNOREKEYS = ["PARTICLE", "ENERGY"]

    def parse(self, jobs):
        eenergies = [20., 40., 50., 80., 100., 119.1, 147.8]
        penergies = [20., 30., 40., 50., 60.,
                     80., 100., 120., 150., 180., 200.]

        common_data = []
        common_json = []

        # Prepare jobs for analysis
        #
        particles = set([x["PARTICLE"] for x in jobs])
        ectrjobs = [x for x in jobs if x["PARTICLE"] == "e-"]
        ectrenergies = [float(x["ENERGY"]) for x in ectrjobs]
        ectrfiles = [os.path.join(x["path"], "ATLHECTBout_Run0.root")
                     for x in ectrjobs]
        pijobs = [x for x in jobs if x["PARTICLE"] == "pi-"]
        pienergies = [float(x["ENERGY"]) for x in pijobs]
        pifiles = [os.path.join(x["path"], "ATLHECTBout_Run0.root")
                   for x in pijobs]
        print("Found "+str(len(jobs))+" runs in jobs:")
        print("--->" + str(len(ectrjobs)) + " jobs with e-, energies (GeV): " + str(ectrenergies) + " ,physlist: " + str(set([x["PHYSLIST"] for x in ectrjobs])))
        # print "------> files: ", ectrfiles
        print("--->" + str(len(pijobs)) + " jobs with pi-, energies (GeV): " + str(pienergies) + " ,physlist: " + str(set([x["PHYSLIST"] for x in pijobs])))
        # print "------> files: ", pifiles

        # e- analysis
        #
        print("Running e- analysis")
        sampfractions = []
        ersampfractions = []
        resolutions = []
        erresolutions = []
        erecenergies = []
        for energy in eenergies:
            # Find e- job with corresponding energy
            job = [x for x in ectrjobs if float(x["ENERGY"]) == energy][0]
            infile = TFile.Open(os.path.join(
                job["path"], "ATLHECTBout_Run0.root"))
            tree = infile.Get("ATLHECTBout")
            H1sampfraction = TH1F("e-", "e-", 1000, 0., 100.)
            for evt in tree:
                H1sampfraction.Fill(evt.elAr/(energy*1000)
                                    * 100)  # percent value
            sampfractions.append(H1sampfraction.GetMean())
            ersampfractions.append(H1sampfraction.GetMeanError())
        print("--->e- sampling fraction: " + str(sampfractions) + " ,physlist: " + str(set([x["PHYSLIST"] for x in ectrjobs])))
        print("--->e- avg sampling fraction: " + str(np.mean(sampfractions)) + "%" + " ,physlist: " + str(set([x["PHYSLIST"] for x in ectrjobs])))
        outfile = TFile.Open("OUTe-"+str(ectrjobs[0]["PHYSLIST"])+".root", "RECREATE")
        for energy in eenergies:
            # Find e- job with corresponding energy
            job = [x for x in ectrjobs if float(x["ENERGY"]) == energy][0]
            infile = TFile.Open(os.path.join(
                job["path"], "ATLHECTBout_Run0.root"))
            tree = infile.Get("ATLHECTBout")
            recenergy = TH1F("e-"+str(energy), "e-" +
                             str(energy), 2000, 0., 200.)
            for evt in tree:
                addchannel = 0
                addchannel += evt.M2L1BirkeLayer[1] #M2L1
                addchannel += evt.M2L1BirkeLayer[2]
                addchannel += evt.M2L1BirkeLayer[3]
                addchannel += evt.M2L1BirkeLayer[5]
                addchannel += evt.M2L2BirkeLayer[3] #M2L2
                addchannel += evt.M2L2BirkeLayer[5]
                addchannel += evt.M3L1BirkeLayer[2] #M3L1
                recenergy.Fill(addchannel/(10.*np.mean(sampfractions)))
            xfitmin = recenergy.GetXaxis().GetBinCenter(
                recenergy.GetMaximumBin())-1.2*recenergy.GetStdDev()
            xfitmax = recenergy.GetXaxis().GetBinCenter(
                recenergy.GetMaximumBin())+1.2*recenergy.GetStdDev()
            F1recenergy = TF1("gaus", "gaus(0)", xfitmin, xfitmax)
            recenergy.Fit(F1recenergy, "QR")
            res = (recenergy.GetFunction("gaus").GetParameter(
                2)/energy)*(energy**(0.5))*100.  # percent
            resolutions.append(res)
            erres = (recenergy.GetFunction("gaus").GetParError(2) /
                     recenergy.GetFunction("gaus").GetParameter(2))*res
            erresolutions.append(erres)
            erecenergies.append(recenergy.GetFunction("gaus").GetParameter(1))
            outfile.cd()
            recenergy.Write()
        print("--->e- sampling terms in resolution: " + str(resolutions) + " %GeV^{1/2}" + " ,physlist: " + str(set([x["PHYSLIST"] for x in ectrjobs])))
        print("--->e- avg sampling term in resolution: " + str(np.mean(resolutions)) + " %GeV^{1/2}" + " ,physlist: " + str(set([x["PHYSLIST"] for x in ectrjobs])))
        print("-->e- reconstructed energies: " + str(erecenergies) + ",physlist: " + str(set([x["PHYSLIST"] for x in ectrjobs])))

        # This contains e- reconstructed energies or 99% of beam energy
        # if no data are available
        #
        fullerecenergies = [erecenergies[0], 0.99*30., erecenergies[1],
                            erecenergies[2], 0.99*60., erecenergies[3],
                            erecenergies[4], erecenergies[5], erecenergies[6],
                            0.99*180., 0.99*200]

        # Create JSON output files for e- energy resolution (graph)
        #
        yield getJSON(jobs[0], "chart",
                      mctool_name="GEANT4",
                      mctool_model=jobs[0]["PHYSLIST"],
                      observableName="energy resolution",
                      # secondaryParticle="e-",
                      beamParticle=job["PARTICLE"],
                      targetName="ATLAS-HEC",
                      beamEnergies=eenergies,
                      title="energyresolution(e-)",
                      xAxisName="<E_{Beam}> [GeV]",
                      yAxisName="#sigma_{0}/E_{0} #sqrt{E_{Beam}} [% #sqrt{GeV}]",
                      xValues=eenergies,
                      yValues=resolutions,
                      yStatErrorsMinus=erresolutions,
                      yStatErrorsPlus=erresolutions
                      )

        # Create JSON output files for e- sampling fraction (graph)
        #
        yield getJSON(jobs[0], "chart",
                      mctool_name="GEANT4",
                      mctool_model=jobs[0]["PHYSLIST"],
                      observableName="sampling fraction",
                      # secondaryParticle="e-",
                      beamParticle=job["PARTICLE"],
                      targetName="ATLAS-HEC",
                      beamEnergies=eenergies,
                      title="samplingfraction(e-)",
                      xAxisName="<E_{Beam}> [GeV]",
                      yAxisName="[%]",
                      xValues=eenergies,
                      yValues=sampfractions,
                      yStatErrorsMinus=ersampfractions,
                      yStatErrorsPlus=ersampfractions
                      )

        # pi- analysis
        #
        print("Running pi- analysis")
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
        pioutfile = TFile.Open("OUTpi-"+str(pijobs[0]["PHYSLIST"])+".root", "RECREATE")
        for index, energy in enumerate(penergies):
            # Find pi- job with corresponding energy
            job = [x for x in pijobs if float(x["ENERGY"]) == energy][0]
            infile = TFile.Open(os.path.join(
                job["path"], "ATLHECTBout_Run0.root"))
            tree = infile.Get("ATLHECTBout")
            recenergy = TH1F("pi-recenergy", "pi-", 2000, 0., 200.)
            response = TH1F("pi-response"+str(energy), "pi-"+str(energy), 2*150, 0., 1.5)
            H1F1 = TH1F("pi-F1", "pi-F1", 100*80, 0., 8000)
            H1F2 = TH1F("pi-F2", "pi-F2", 100*80, 0., 8000)
            H1F3 = TH1F("pi-F3", "pi-F3", 100*80, 0., 8000)
            H1F4 = TH1F("pi-F4", "pi-F4", 100*80, 0., 8000)
            for evt in tree:
                addchannel = 0
                addchannelF1 = 0
                addchannelF2 = 0
                addchannelF3 = 0
                addchannelF4 = 0

                addchannel += evt.M2L1BirkeLayer[0]  # M2L1
                addchannel += evt.M2L1BirkeLayer[1]
                addchannel += evt.M2L1BirkeLayer[2]
                addchannel += evt.M2L1BirkeLayer[3]
                addchannel += evt.M2L1BirkeLayer[4]
                addchannel += evt.M2L1BirkeLayer[5]
                addchannelF1 += evt.M2L1BirkeLayer[0]  # M2L1
                addchannelF1 += evt.M2L1BirkeLayer[1]
                addchannelF1 += evt.M2L1BirkeLayer[2]
                addchannelF1 += evt.M2L1BirkeLayer[3]
                addchannelF1 += evt.M2L1BirkeLayer[4]
                addchannelF1 += evt.M2L1BirkeLayer[5]

                addchannel += evt.M3L1BirkeLayer[0]  # M3L1
                addchannel += evt.M3L1BirkeLayer[2]
                addchannel += evt.M3L1BirkeLayer[4]
                addchannelF1 += evt.M3L1BirkeLayer[0]  # M3L1
                addchannelF1 += evt.M3L1BirkeLayer[2]
                addchannelF1 += evt.M3L1BirkeLayer[4]

                addchannel += evt.M2L2BirkeLayer[2]  # M2L2
                addchannel += evt.M2L2BirkeLayer[3]
                addchannel += evt.M2L2BirkeLayer[4]
                addchannel += evt.M2L2BirkeLayer[5]
                addchannel += evt.M2L2BirkeLayer[6]
                addchannel += evt.M2L2BirkeLayer[7]
                addchannel += evt.M2L2BirkeLayer[8]
                addchannel += evt.M2L2BirkeLayer[9]
                addchannelF2 += evt.M2L2BirkeLayer[2]  # M2L2
                addchannelF2 += evt.M2L2BirkeLayer[3]
                addchannelF2 += evt.M2L2BirkeLayer[4]
                addchannelF2 += evt.M2L2BirkeLayer[5]
                addchannelF2 += evt.M2L2BirkeLayer[6]
                addchannelF2 += evt.M2L2BirkeLayer[7]
                addchannelF2 += evt.M2L2BirkeLayer[8]
                addchannelF2 += evt.M2L2BirkeLayer[9]

                addchannel += evt.M1L2BirkeLayer[3]  # M1L2
                addchannel += evt.M1L2BirkeLayer[5]
                addchannel += evt.M1L2BirkeLayer[7]
                addchannelF2 += evt.M1L2BirkeLayer[3]  # M1L2
                addchannelF2 += evt.M1L2BirkeLayer[5]
                addchannelF2 += evt.M1L2BirkeLayer[7]

                addchannel += evt.M3L2BirkeLayer[2]  # M3L2
                addchannel += evt.M3L2BirkeLayer[3] 
                addchannel += evt.M3L2BirkeLayer[4] 
                addchannel += evt.M3L2BirkeLayer[5]
                addchannel += evt.M3L2BirkeLayer[6]
                addchannel += evt.M3L2BirkeLayer[7]
                addchannel += evt.M3L2BirkeLayer[8]
                addchannelF2 += evt.M3L2BirkeLayer[2]  # M3L2
                addchannelF2 += evt.M3L2BirkeLayer[3]
                addchannelF2 += evt.M3L2BirkeLayer[4]
                addchannelF2 += evt.M3L2BirkeLayer[5]
                addchannelF2 += evt.M3L2BirkeLayer[6]
                addchannelF2 += evt.M3L2BirkeLayer[7]
                addchannelF2 += evt.M3L2BirkeLayer[8]

                addchannel += 2.*evt.M2L3BirkeLayer[2]  # M2L3
                addchannel += 2.*evt.M2L3BirkeLayer[3]
                addchannel += 2.*evt.M2L3BirkeLayer[4]
                addchannel += 2.*evt.M2L3BirkeLayer[5]
                addchannel += 2.*evt.M2L3BirkeLayer[6]
                addchannel += 2.*evt.M2L3BirkeLayer[7]
                addchannel += 2.*evt.M2L3BirkeLayer[8]
                addchannel += 2.*evt.M2L3BirkeLayer[9]
                addchannelF3 += 2.*evt.M2L3BirkeLayer[2]  # M2L3
                addchannelF3 += 2.*evt.M2L3BirkeLayer[3]
                addchannelF3 += 2.*evt.M2L3BirkeLayer[4]
                addchannelF3 += 2.*evt.M2L3BirkeLayer[5]
                addchannelF3 += 2.*evt.M2L3BirkeLayer[6]
                addchannelF3 += 2.*evt.M2L3BirkeLayer[7]
                addchannelF3 += 2.*evt.M2L3BirkeLayer[8]
                addchannelF3 += 2.*evt.M2L3BirkeLayer[9]

                addchannel += 2.*evt.M1L3BirkeLayer[3]  # M1L3
                addchannel += 2.*evt.M1L3BirkeLayer[5]
                addchannel += 2.*evt.M1L3BirkeLayer[7]
                addchannelF3 += 2.*evt.M1L3BirkeLayer[3]  # M1L3
                addchannelF3 += 2.*evt.M1L3BirkeLayer[5]
                addchannelF3 += 2.*evt.M1L3BirkeLayer[7]

                addchannel += 2.*evt.M3L3BirkeLayer[2]  # M3L3
                addchannel += 2.*evt.M3L3BirkeLayer[3]
                addchannel += 2.*evt.M3L3BirkeLayer[4]
                addchannel += 2.*evt.M3L3BirkeLayer[5]
                addchannel += 2.*evt.M3L3BirkeLayer[6]
                addchannel += 2.*evt.M3L3BirkeLayer[7]
                addchannel += 2.*evt.M3L3BirkeLayer[8]
                addchannelF3 += 2.*evt.M3L3BirkeLayer[2]  # M3L3
                addchannelF3 += 2.*evt.M3L3BirkeLayer[3]
                addchannelF3 += 2.*evt.M3L3BirkeLayer[4]
                addchannelF3 += 2.*evt.M3L3BirkeLayer[5]
                addchannelF3 += 2.*evt.M3L3BirkeLayer[6]
                addchannelF3 += 2.*evt.M3L3BirkeLayer[7]
                addchannelF3 += 2.*evt.M3L3BirkeLayer[8]

                addchannel += 2.*evt.M2L4BirkeLayer[2]  # M2L4
                addchannel += 2.*evt.M2L4BirkeLayer[3]
                addchannel += 2.*evt.M2L4BirkeLayer[4]
                addchannel += 2.*evt.M2L4BirkeLayer[5]
                addchannel += 2.*evt.M2L4BirkeLayer[6]
                addchannel += 2.*evt.M2L4BirkeLayer[7]
                addchannelF4 += 2.*evt.M2L4BirkeLayer[2]  # M2L4
                addchannelF4 += 2.*evt.M2L4BirkeLayer[3]
                addchannelF4 += 2.*evt.M2L4BirkeLayer[4]
                addchannelF4 += 2.*evt.M2L4BirkeLayer[5]
                addchannelF4 += 2.*evt.M2L4BirkeLayer[6]
                addchannelF4 += 2.*evt.M2L4BirkeLayer[7]

                addchannel += 2.*evt.M3L4BirkeLayer[2]  # M3L4
                addchannel += 2.*evt.M3L4BirkeLayer[4]
                addchannel += 2.*evt.M3L4BirkeLayer[6]
                addchannelF4 += 2.*evt.M3L4BirkeLayer[2]  # M3L4
                addchannelF4 += 2.*evt.M3L4BirkeLayer[4]
                addchannelF4 += 2.*evt.M3L4BirkeLayer[6]

                if addchannel > 0.:
                    response.Fill((addchannel/fullerecenergies[index]) /
                                  (10.*np.mean(sampfractions)))
                    recenergy.Fill(addchannel/(10.*np.mean(sampfractions)))
                    H1F1.Fill(addchannelF1)
                    H1F2.Fill(addchannelF2)
                    H1F3.Fill(addchannelF3)
                    H1F4.Fill(addchannelF4)

            # Response
            #
            respxfitmin = response.GetMean()-1.5*response.GetStdDev()
            respxfitmax = response.GetMean()+1.5*response.GetStdDev()
            F1Response = TF1("rgaus", "gaus(0)", respxfitmin, respxfitmax)
            response.Fit(F1Response, "QR")
            responses.append(response.GetFunction("rgaus").GetParameter(1))
            erresponses.append(3.*response.GetFunction("rgaus").GetParError(1))
            pioutfile.cd()
            response.Write()

            # Resolution
            #
            recenergy.Fit("gaus", "Q")
            res = 100.*recenergy.GetFunction("gaus").GetParameter(
                2)/recenergy.GetFunction("gaus").GetParameter(1)
            resolutions.append(res)
            erres = (recenergy.GetFunction("gaus").GetParError(2)/recenergy.GetFunction("gaus").GetParameter(2) +
                     recenergy.GetFunction("gaus").GetParError(1)/recenergy.GetFunction("gaus").GetParameter(1))*res
            erresolutions.append(erres)

            Fmean = H1F1.GetMean()+H1F2.GetMean()+H1F3.GetMean()+H1F4.GetMean()
            F1.append(H1F1.GetMean()/Fmean)
            F2.append(H1F2.GetMean()/Fmean)
            F3.append(H1F3.GetMean()/Fmean)
            F4.append(H1F4.GetMean()/Fmean)
            depths = [28.05/2., 28.05+53.6/2., 28.05 +
                      53.6+53.35/2., 28.05+53.6+53.35+46.8/2]

            L0.append(depths[0]*H1F1.GetMean()/Fmean +
                      depths[1]*H1F2.GetMean()/Fmean +
                      depths[2]*H1F3.GetMean()/Fmean +
                      depths[3]*H1F4.GetMean()/Fmean)

            sigmaL0.append( (F1[index]*(depths[0]-L0[index])**2+
                             F2[index]*(depths[1]-L0[index])**2+
                             F3[index]*(depths[2]-L0[index])**2+ 
                             F4[index]*(depths[3]-L0[index])**2)**0.5 )

            #The statistical error on L0 and sigmaL0 associated with different
            #random seeds was estimated to be of the order of 0.5%
            #therefore I add +-0.5% of the L0 (sigmaL0) value as statistical error
            #
            erL0 = [0.005*x for x in L0]
            ersigmaL0 = [0.005*x for x in sigmaL0]

        print("--->pi- pi/e: " + str(responses) + " ,physlist: " + str(set([x["PHYSLIST"] for x in ectrjobs])))
        print("--->pi- avg pi/e: " + str(np.mean(responses)))
        print("--->pi- resolutions: " + str(resolutions) + " %")
        print("--->pi- avg resolution: " + str(np.mean(resolutions)) + " %")
        print("--->pi- F1: " + str(F1))
        print("--->pi- F2: " + str(F2))
        print("--->pi- F3: " + str(F3))
        print("--->pi- F4: " + str(F4))
        print("--->pi- L0 [cm]: " + str(L0))
        print("--->pi- sigmaL0 [cm]: " + str(sigmaL0))

        # Create JSON output files for pi- energy resolution (graph)
        #
        yield getJSON(jobs[0], "chart",
                      mctool_name="GEANT4",
                      mctool_model=jobs[0]["PHYSLIST"],
                      observableName="energy resolution",
                      # secondaryParticle="pi-",
                      beamParticle=job["PARTICLE"],
                      targetName="ATLAS-HEC",
                      beamEnergies=penergies,
                      title="energyresolution(pi-)",
                      xAxisName="<E_{Beam}> [GeV]",
                      yAxisName="#sigma_{0}/E_{0} [%]",
                      xValues=penergies,
                      yValues=resolutions,
                      yStatErrorsPlus=erresolutions,
                      yStatErrorsMinus=erresolutions
                      )

        # Create JSON output files for pi/e response ratio (graph)
        #
        yield getJSON(jobs[0], "chart",
                      mctool_name="GEANT4",
                      mctool_model=jobs[0]["PHYSLIST"],
                      observableName="energy response",
                      beamParticle=job["PARTICLE"],
                      targetName="ATLAS-HEC",
                      beamEnergies=penergies,
                      title="#pi/e",
                      xAxisName="<E_{Beam}> [GeV]",
                      yAxisName="#pi/e",
                      xValues=penergies,
                      yValues=responses,
                      yStatErrorsPlus=erresponses,
                      yStatErrorsMinus=erresponses
                      )

        # Create JSON output files for L0 (graph)
        #
        yield getJSON(jobs[0], "chart",
                      mctool_name="GEANT4",
                      mctool_model=jobs[0]["PHYSLIST"],
                      observableName="longitudinal shower barycenter",
                      beamParticle=job["PARTICLE"],
                      targetName="ATLAS-HEC",
                      beamEnergies=penergies,
                      title="L_{0}",
                      xAxisName="<E_{Beam}> [GeV]",
                      yAxisName="L_{0} [cm]",
                      xValues=penergies,
                      yValues=L0,
                      yStatErrorsPlus=erL0,
                      yStatErrorsMinus=erL0
                      )

        # Create JSON output files for sigmaL0 (graph)
        #
        yield getJSON(jobs[0], "chart",
                      mctool_name="GEANT4",
                      mctool_model=jobs[0]["PHYSLIST"],
                      observableName="shower length",
                      targetName="ATLAS-HEC",
                      beamParticle=job["PARTICLE"],
                      beamEnergies=penergies,
                      title="#sigma_{L}",
                      xAxisName="<E_{Beam}> [GeV]",
                      yAxisName="#sigma_{L} [cm]",
                      xValues=penergies,
                      yValues=sigmaL0,
                      yStatErrorsPlus=ersigmaL0,
                      yStatErrorsMinus=ersigmaL0
                      )

        # ------------------------------------------------------------
        # Create JSON files for experimental data
        # uncomment this part only if you want to recreate these files
        # ------------------------------------------------------------
        '''
        # Create JSON output files for experimental e- energy resolution (graph)
        #
        yield getJSON(jobs[0], "chart",
                      mctool_name="experiment",
                      mctool_version="experiment",
                      mctool_model="experiment",
                      testName="experiment",
                      observableName="energy resolution",
                      # secondaryParticle="e-",
                      beamParticle="e-",
                      targetName="ATLAS-HEC",
                      beamEnergies=eenergies,
                      title="energyresolution(e-)",
                      xAxisName="<E_{Beam}> [GeV]",
                      yAxisName="#sigma_{0}/E_{0} #sqrt{E_{Beam}} [% #sqrt{GeV}]",
                      xValues=eenergies,
                      yValues=[22.2, 21.6, 21.5, 22.30, 22.0, 21.2, 21.3],
                      yStatErrorsMinus=[22.57-22.2, 21.89-21.6, 21.78-21.5,
                                        22.6-22.30, 22.32-22.0, 21.46-21.2, 21.57-21.3],
                      yStatErrorsPlus=[22.57-22.2, 21.89-21.6, 21.78-21.5,
                                       22.6-22.30, 22.32-22.0, 21.46-21.2, 21.57-21.3]
                      )

        # Create JSON output files for experimental pi- energy resolution (graph)
        #
        yield getJSON(jobs[0], "chart",
                      mctool_name="experiment",
                      mctool_model="experiment",
                      mctool_version="experiment",
                      testName="experiment",
                      observableName="energy resolution",
                      # secondaryParticle="pi-",
                      beamParticle="pi-",
                      targetName="ATLAS-HEC",
                      beamEnergies=penergies,
                      title="energyresolution(pi-)",
                      xAxisName="<E_{Beam}> [GeV]",
                      yAxisName="#sigma_{0}/E_{0} [%]",
                      xValues=penergies,
                      yValues=[13.465587044534415, 13.465587044534415, 11.546558704453442, 11.279352226720649, 11.060728744939272,
                               10.137651821862349, 9.433198380566802, 8.777327935222674, 8.000000000000002, 7.465587044534413, 7.684210526315789],
                      yStatErrorsPlus=[14.47-13.47, 13.89-13.47, 11.85-11.55, 11.52 - \
                                       11.28, 11.27-11.06, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001],
                      yStatErrorsMinus=[14.47-13.47, 13.89-13.47, 11.85-11.55, 11.52 - \
                                        11.28, 11.27-11.06, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001]
                      )

        # Create JSON output files for experimental pi/e response ratio (graph)
        #
        yield getJSON(jobs[0], "chart",
                      mctool_name="experiment",
                      mctool_model="experiment",
                      mctool_version="experiment",
                      testName="experiment",
                      observableName="energy response",
                      beamParticle="pi-",
                      targetName="ATLAS-HEC",
                      beamEnergies=[20., 40., 50.,
                                    80., 100., 120., 150.],
                      title="#pi/e",
                      xAxisName="<E_{Beam}> [GeV]",
                      yAxisName="#pi/e",
                      xValues=[20., 40., 50., 80., 100., 120., 150.],
                      yValues=[0.777, 0.796, 0.8026,
                               0.819, 0.825, 0.829, 0.840],
                      yStatErrorsPlus=[0.79-0.777, 0.8095-0.796, 0.816-0.8026,
                                       0.833-0.819, 0.84-0.825, 0.844-0.829, 0.854-0.840],
                      yStatErrorsMinus=[0.79-0.777, 0.8095-0.796, 0.816-0.8026,
                                        0.833-0.819, 0.84-0.825, 0.844-0.829, 0.854-0.840]
                      )

        # Create JSON output files for experimental L0 (graph)
        #
        yield getJSON(jobs[0], "chart",
                      mctool_name="experiment",
                      mctool_model="experiment",
                      mctool_version="experiment",
                      testName="experiment",
                      observableName="longitudinal shower barycenter",
                      beamParticle="pi-",
                      targetName="ATLAS-HEC",
                      beamEnergies=penergies,
                      title="L_{0}",
                      xAxisName="<E_{Beam}> [GeV]",
                      yAxisName="L_{0} [cm]",
                      xValues=penergies,
                      yValues=[47.20989874689059, 50.415242855641324, 51.444637789482286, 52.951825942751704, 54.405759865463,
                               55.72198021652049, 57.223749518259424, 58.30097983112803, 58.86865124318264, 60.04659745173835, 60.27474978686629]
                      # yStatErrorsMinus=,
                      # yStatErrorsPlus=
                      )

        # Create JSON output files for experimental sigmaL0 (graph)
        #
        yield getJSON(jobs[0], "chart",
                      mctool_name="experiment",
                      mctool_model="experiment",
                      mctool_version="experiment",
                      testName="experiment",
                      observableName="shower length",
                      targetName="ATLAS-HEC",
                      beamParticle="pi-",
                      beamEnergies=penergies,
                      title="#sigma_{L}",
                      xAxisName="<E_{Beam}> [GeV]",
                      yAxisName="#sigma_{L} [cm]",
                      xValues=penergies,
                      yValues=[31.561680356622446, 32.623646745172536, 33.04349844031647, 33.923354237050305, 34.50614698802983,
                               34.94316059883212, 35.591168629313415, 35.798147808347814, 36.09916133280085, 36.380940559327335, 36.45373298649713]
                      # yStatErrorsMinus=,
                      # yStatErrorsPlus=
                      )
        '''
# ****************************************************
