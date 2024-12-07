# ATLHECTB
**A Geant4 simulation of the ATLAS hadronic end-cap calorimeter beam tests.**

![example workflow](https://github.com/lopezzot/ATLHECTB/actions/workflows/geant4-test.yml/badge.svg)
![example workflow](https://github.com/lopezzot/ATLHECTB/actions/workflows/clang-format-test.yml/badge.svg)
![GitHub commits since latest release (by date) for a branch](https://img.shields.io/github/commits-since/lopezzot/ATLHECTB/latest/main?logo=github)

<figure>
<img src="./images/ATLHECTB_movie.gif" alt="Trulli" style="width:100%">
<figcaption align="center"><b>Fig. - 10 GeV muon passing through the ATLAS HEC.</b></figcaption>
</figure>

<br/><br/>

<video align="center" src="https://user-images.githubusercontent.com/34685792/136785193-1741c29f-3666-4859-bd6c-34166515a7f7.mp4" alt="Trulli" controls="controls" style="width:100%">
</video>
<figcaption align="center"><b>Vid. - Video example of how to use ATLHECTB in Geant Val.</b></figcaption>

<br/><br/>

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#project-description">Project description</a></li>
    <li><a href="#authors-and-contacts">Authors and contacts</a></li>
      <li>
      <a href="#documentation-and-results">Documentation and results</a>
      <ul>
        <li><a href="#selected-articles">Selected articles</a></li>
        <li><a href="#selected-presentations">Selected presentations</a></li>
      </ul>
    </li>
     </li><li><a href="#directory-structure">Directory structure</a></li>
    <li>
      <a href="#geant-val-integration">Geant Val integration</a>
      <ul>
        <li><a href="#list-of-results-on-geant-val">List of results on Geant Val</a></li>
      </ul>
    </li> 
    <li><a href="#available-datasets-and-analyses">Available datasets and analyses</a></li>
    <li>
      <a href="#how-to">How to</a>
      <ul>
        <li><a href="#build-compile-and-execute-on-maclinux">Build, compile and execute on Mac/Linux</a></li>
        <li><a href="#build-compile-and-execute-on-lxplus">Build, compile and execute on lxplus</a></li>
        <li><a href="#submit-a-job-with-htcondor-on-lxplus">Submit a job with HTCondor on lxplus</a></li>
        <li><a href="#get-atlas-hec-geo-parameters-from-mysql-database">Get ATLAS HEC geo parameters from mysql database</a></li>
        <li><a href="#dump-atlhectb-gdml-geometry-description-file">Dump ATLHECTB GDML geometry description file</a></li>
        <li><a href="#use-flukacern-hadron-inelastic-process">Use Fluka.Cern hadron inelastic process</a></li>
      </ul>
    </li>
    </li><li><a href="#geant4-and-github-actions">Geant4 and Github Actions</a></li>  
    </li><li><a href="#bugs-report">Bugs report</a></li>
    </li><li><a href="#my-quick-geant4-installation">My quick Geant4 installation</a></li>
    <li><a href="#selected-atlas-hec-references">Selected ATLAS HEC references</a>
  </ol>                                           
</details>

<!--Project desription-->
## Project description
The project targets a standalone Geant4 simulation of the ATLAS hadronic end-cap calorimeter beam tests to perform Geant4 regression testing, physics lists comparison and validation against test-beam data. It is also an example of how to use a Geant4 application with Geant Val and how to automatically test it using Github Actions. 
- ⏰ Start date: 11 May 2021 
- 📌 Status: from v1.0 on ATLHECTB is available for data production, from v2.0 on ATLHECTB is available on Geant Val
- Citation: if you want to use the code read the [LICENSE](https://github.com/lopezzot/ATLHECTB/blob/main/LICENSE) and cite [![DOI:10.3390/instruments6030041](https://zenodo.org/badge/DOI/10.3390/instruments6030041.svg)](https://doi.org/10.3390/instruments6030041)

<!--Authors and contacts-->
## Authors and contacts
- 👨‍🔬 Lorenzo Pezzotti (CERN EP-SFT) - lorenzo.pezzotti@cern.ch 
- 👨‍🔬 Supervisor: Alberto Ribon (CERN EP-SFT)

<!--Documentation and results-->
## Documentation and results
### Selected articles
- Pezzotti, L.; Kiryunin, A.; Konstantinov, D.; Ribon, A.; Strizenec, P.; on behalf of the Geant4 Collaboration. Including Calorimeter Test Beams in Geant-val—The Physics Validation Testing Suite of Geant4. Instruments 2022, 6, 41.
[![DOI:10.3390/instruments6030041](https://zenodo.org/badge/DOI/10.3390/instruments6030041.svg)](https://doi.org/10.3390/instruments6030041)

### Selected presentations
- 🗣️ CALOR2024 22/5/2024, [**Validating Geant4 against calorimeters beam tests**](https://indico.cern.ch/event/1339557/contributions/5898513/attachments/2860143/5004975/lopezzot_CALOR2024.pdf)
- 🗣️ CERN EP-SFT Group Meeting 22/4/2024, [**End of Fellowship Report**](https://indico.cern.ch/event/1395384/contributions/5865083/attachments/2841640/4967534/lopezzot_epsft_22_4_2024.pdf)
- 🗣️ Geant4 Collaboration Meeting 2023 25-29/9/2023, [**Validation of Geant4 physics via calorimeter test-beams in geant-val**](https://indico.cern.ch/event/1307331/contributions/5581219/attachments/2721132/4728358/lopezzot_ValidationCalorimeters_G4CM23.pdf)
- 🗣️ ATLAS Simulation Group Meeting 5/9/2023 (ATLAS restricted), [**Testing ATLAS calorimeters with Geant4 and FLUKA.CERN**](https://indico.cern.ch/event/1321227/contributions/5565265/attachments/2709186/4704111/lopezzot_ATLASsim_5_9_2023.pdf)
- 🗣️ CERN EP-SFT Simulation Meeting 5/9/2023, [**Testing ATLAS calorimeters with Geant4 and FLUKA.CERN**](https://indico.cern.ch/event/1322382/contributions/5565251/attachments/2708985/4703734/lopezzot_5_9_2023.pdf)
- 🗣️ 11th BTTB Workshop 19/4/2023, [**Geant4 Validation on Test-Beam Calorimetry Data**](https://indico.cern.ch/event/1232761/contributions/5320333/attachments/2631283/4551077/lopezzot_11thBTTBWS_19_4_2023.pdf)
- 🗣️ CERN EP-SFT Simulation Meeting 7/2/2023, [**Testing alternative FTF tunes on calorimeters**](https://indico.cern.ch/event/1251688/contributions/5261214/attachments/2589213/4467664/lopezzot_g4sim_7_2_2023.pdf) <em>(results obtained with v2.4)</em>
- 🗣️ Geant4 Collaboration Meeting 28/9/2022, [**Physics validation of Geant4 via calorimeter test-beams**](https://indico.cern.ch/event/1156193/contributions/5051067/attachments/2517516/4328428/lopezzot_G4Collab2022.pdf)
- 🗣️ CALOR2022 17/5/2022, [**Including calorimeter test-beams into geant-val**](https://indico.cern.ch/event/847884/contributions/4833199/attachments/2445590/4190486/lopezzot_CALOR2022.pdf)
- 🗣️ CERN EP-SFT Group Meeting 2/5/2022, [**Highlights from recent Geant4 validation on test-beam data**](https://indico.cern.ch/event/1155403/contributions/4851266/attachments/2435130/4170273/EPSFT_2_5_2022_lopezzot.pdf)
- 🗣️ ATLAS Simulation Group Meeting 9/11/2021 (ATLAS restricted), [**Updated results from the Geant ATLAS HEC simulation**](https://indico.cern.ch/event/1079946/contributions/4597365/attachments/2342511/3993995/ATLASSim_lopezzot_9_11_2021.pdf)
- 🗣️ CERN EP-SFT Simulation Meeting 2/11/2021, [**Regression testing and physics list comparison with ATLHECTB**](https://indico.cern.ch/event/1093045/contributions/4597043/attachments/2337968/3985256/G4_lopezzot_2_11_2021.pdf)
- 🗣️ CERN EP-SFT Simulation Meeting 19/10/2021, [**Geant Val integration of the ATLAS HEC test beam simulation**](https://indico.cern.ch/event/1088251/contributions/4575977/attachments/2330415/3971619/G4_lopezzot_19_10_2021.pdf)
- 🗣️ CERN EP-SFT Simulation Meeting 13/7/2021, [**Results from the Geant4 ATLAS HEC test-beam simulation**](https://indico.cern.ch/event/1058649/contributions/4449544/attachments/2280962/3875541/G4Sim_lopezzot_13_7_2021.pdf)
- 🗣️ ATLAS Simulation Group Meeting 6/7/2021 (ATLAS restricted), [**A Geant4 simulation of
the ATLAS HEC beam tests**](https://indico.cern.ch/event/995938/contributions/4421574/attachments/2277065/3868501/ATLASSim_lopezzot_6_7_2021.pdf) <em>(errata corrige: slide 11, EstBoard 0.8 mm Kapton -> 0.61 mm Kapton)</em>
- 🗣️ CERN EP-SFT Simulation Meeting 15/6/2021, [**First results from the Geant4 ATLAS HEC test beam simulation**](https://indico.cern.ch/event/1049152/contributions/4407943/attachments/2264354/3844295/G4SW_lopezzot_15_6_2021.pdf) <em>(errata corrige: slide 9, EstBoard 0.8 mm Kapton -> 0.61 mm Kapton)</em>

**[⬆ back to top](#atlhectb)**

<!--Directory structure-->
## Directory structure
```
.
├── ATLHECTB.cc               # main()
├── ATLHECTB_gui.mac          # GUI settings
├── ATLHECTB_init_vis.mac     # Initialization and visualization settings
├── ATLHECTB_run.mac          # macro card example
├── CMakeLists.txt            # CMake instructions     
├── GNUmakefile               # GNUmake instructions
├── LICENSE                   # The LICENSE
├── README.md                 # Markdown documentation
├── _config.yml               # github pages settings
├── analysis                  # ROOT macros for data analysis
├── geantval_scripts          # Scripts for Geant Val usage
├── images                    # Images for documentation
├── include                   # Includers (.hh)
├── runcards                  # Macro cards for production
├── scripts                   # Scripts for lxplus and lxplus with HTCondor usage
├── src                       # Sources (.cc)
```

<!--Geant Val integration-->
## Geant Val integration
Geant Val [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://geant-val.cern.ch/) is the Geant4 testing and validation suite. It is a project hosted on gitlab.cern.ch [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://gitlab.cern.ch/GeantValidation) used to facilitate the maintenance and validation of Geant4 applications, referred to as <em> tests</em>.\
The following are instructions to use ATLHECTB within Geant Val, from batch submission to website deployment.
1. On lxplus git clone ATLHECTB and the Geant Val/geant-config-generator
   ```sh
   git clone git@github.com:lopezzot/ATLHECTB.git
   git clone ssh://git@gitlab.cern.ch:7999/GeantValidation/geant-config-generator.git
   ```
2. Copy the ATLHECTB geant val scripts into tests/geant4/; cd geant-config-generator
   ```sh
   cp -r ATLHECTB/geantval_scripts/ATLHECTB/ geant-config-generator/tests/geant4/
   cd geant-config-generator
   ```
3. We will execute ATLHECTB via Geant Val using Geant4.10.7.p01, therefore we must make sure file ```10.7.p01.sh``` exists into ```configs/geant/```. In file ```10.7.p01.sh``` we also export the path to the ATLHECTB 10.7.p01 executable file (for instruction on how to compile ATLHECTB see [How to: Build, compile and execute on lxplus](#build-compile-and-execute-on-lxplus)).\
Hence ```10.7.p01.sh``` looks like this:
   ```sh
   #!/bin/bash
   VERSION="10.7.p01"
   PLATFORM="x86_64-centos7-gcc8-optdeb"
   # Geant4 libraries
   source /cvmfs/geant4.cern.ch/geant4/$VERSION/${PLATFORM}/bin/geant4.sh
   [ -e /cvmfs/geant4.cern.ch/geant4/$VERSION/setup_g4datasets.sh ] && source /cvmfs/geant4.cern.ch/geant4/$VERSION/setup_g4datasets.sh
   # Test path
   export PATH="/cvmfs/geant4.cern.ch/opt-geant-val/$VERSION/$PLATFORM/bin:/cvmfs/geant4.cern.ch/opt/$VERSION/$PLATFORM/bin:$PATH:/afs/cern.ch/work/l/lopezzot/path-to-executable/"
   # Compiler
   source /cvmfs/sft.cern.ch/lcg/contrib/gcc/8/x86_64-centos7/setup.sh
   ```
4. Create macros and metadata for Geant Val execution
   ```sh
   python mc-config-generator.py submit -t ATLHECTB -d OUTPUT -v 10.7.p01 -q "testmatch" -r
   ```
   this command creates the Geant Val files for batch submission using HTCondor under the ```OUTPUT``` folder, using ATLHECTB, Geant4.10.7.p01 and the ```testmatch``` job flavour.
5. To monitor the jobs use
   ```sh
   python mc-config-generator.py status -t ATLHECTB -d OUTPUT
   ```
   When the job execution ends, the root output files are stored in the corresponding job folder. Each job folder will look like this:
   ```sh
   ATLHECTB-env.log  ATLHECTB.json  ATLHECTB.mac  ATLHECTBout_Run0.root  ATLHECTB.sh  bsub.sh  config.sh  test_stderr.txt  test_stdout.txt
   ```
6. Execute the analysis on the root files in the ```OUTPUT``` folder to create Geant Val JSON output files
    ```sh
    python mc-config-generator.py parse -t ATLHECTB -d OUTPUT 
    ```
    the analysis is coded in ```tests/geant4/ATLHECTB/parser.py```. The ```OUTPUTJSON``` folder is created with the corresponding JSON files.
7. The last part is to deploy the results on Geant Val. The ATLHECTB layout on the Geant Val website is defined in the ```ATLHECTB.xml``` file on ```gitlab.com/thegriglat/geant-val-layouts``` (additional info are in the ```tags.json``` file).\
   Deploy JSON files on the Geant Val database
   ```sh
    find . -name '*.json' | while read i; do curl -H "Content-Type: application/json" -H "token: askauthor" --data @$i https://geant-val.cern.ch/upload; echo; done
   ```

### List of results on Geant Val
The following are results deployed on Geant Val so far. A copy of the used config files is stored in ```geantval_scripts/configs/```.
<details><summary>Geant-Val-Table</summary>
<p>

| ATLHECTB         | Reproduce data | Reproduce analysis | Comments     |
| -------------    | ----------     | -----------        | -----------  |
| v2.6.2 <br /> Dataset #2 tag 2.6.2_2 <br /> G4.11.2.p01 PL: FTFP_INCLXX <br /> Added on 19/4/2024 | python3 mc-config-generator.py submit -t ATLHECTB -d OUTPUTxxx -v xxx -q "testmatch" -r | python3 mc-config-generator.py parse -t ATLHECTB -d OUTPUTxxx | Results as in 2.6.2_1 but with FTFP_INCLXX PL only. Jobs launched again with no crash w.r.t. 2.6.2_1 (seg fault). |
| v2.6.2 <br /> Dataset #2 tag 2.6.2_1 <br /> G4.11.2.p01 PL: FTFP_BERT(+tune1,2,3), QGSP_BERT, FTFP_BERT_ATL <br /> (Note: no FTFP_INCLXX due to a crash to be investigated) <br /> Added on 15/4/2024 | python3 mc-config-generator.py submit -t ATLHECTB -d OUTPUTxxx -v xxx -q "testmatch" -r | python3 mc-config-generator.py parse -t ATLHECTB -d OUTPUTxxx | First results produced on el9 lxplus machines. |
| v2.5 <br /> Dataset #2 tag 2.5_2  <br /> (G4.10.4.p03, G4.10.5.p01, G4.10.6.p03, G4.10.7.p01, G4.10.7.p03, G4.11.0.p04, G4.11.1.p01) <br />(PL: FTFP_BERT, QGSP_BERT, FTFP_BERT_ATL, FTFP_INCLXX) <br /> (Note: no FTFP_INCLXX for 10.4.p03) (Note: for G4.11.1.p01 FTFP_BERT(+tunes1,2,3) have been included) <br /> Added on 2/5/2023 | python mc-config-generator.py submit -t ATLHECTB -d OUTPUTxxx -v xxx -q "testmatch" -r | python mc-config-generator.py parse -t ATLHECTB -d OUTPUTxxx | New results by v2.5, previous results on geant-val to be removed. Exp. data json files not recreated, kept the 2.4_2 ones. Results with 10.4.p03 obtained in single thread mode, see [Bugs report](#bugs-report). |
| v2.4 <br /> Dataset #2 <br /> tag 2.4_2 (Geant4.10.7.p03, Geant4.11.0.p03 ATLHECTB v2.4, FTFP_BERT, FTFP_BERT_ATL, QGSP_BERT, FTFP_BERT_INCLXX) (Geant4.11.1, ATLHECTB v2.4, FTFP_BERT(+tune1-2-3), FTFP_BERT_ATL, QGSP_BERT, FTFP_BERT_INCLXX) <br /> ~Added on 4/2/2023~ Removed on 2/5/2023 (exp. data not removed) <br /> | python mc-config-generator.py submit -t ATLHECTB -d OUTPUT -v 10.7.p03 (11.0.p03, 11.1) -q "testmatch" -r | python mc-config-generator.py parse -t ATLHECTB -d OUTPUT |  Results for three Geant4 versions using v2.4 i.e. simulation with scintillators + new pi/e and longitudinal shape analysis. New exp. data uploaded (better error bars) reproducable with parser.py v2.4.|
| v2.1 <br /> Dataset #2 <br /> tag 2.1_2 (Geant4.10.05.p01, ATLHECTB v2.1, FTFP_BERT, FTFP_BERT_ATL, QGSP_BERT, FTFP_BERT_INCLXX) (Geant4.10.4, Geant4.10.4.p01, ATLHECTB v2.1, FTFP_BERT, FTFP_BERT_ATL, QGSP_BERT) <br /> ~Added on 28/10/2021~ Removed on 2/5/2023 <br /> | python mc-config-generator.py submit -t ATLHECTB -d OUTPUT -v 10.5.p01 (10.4, 10.4.p01) -q "testmatch" -r | python mc-config-generator.py parse -t ATLHECTB -d OUTPUT |  Results for three Geant4 versions, can be compared to tag 2.1_1, 2.0_1, and 2.0_2. Results for 10.4 and 10.4.p01 obtained in single thread mode, see [Bugs report](#bugs-report).|
| v2.1 <br /> Dataset #1 <br /> tag 2.1_1 (Geant4.10.06.p01 and Geant4.11.0.beta, ATLHECTB v2.1, FTFP_BERT, FTFP_BERT_ATL, QGSP_BERT, FTFP_BERT_INCLXX) <br /> ~Added on 21/10/2021~ Removed on 2/5/2023 <br /> | python mc-config-generator.py submit -t ATLHECTB -d OUTPUT -v 10.6.p01 (11.0.beta) -q "testmatch" -r | python mc-config-generator.py parse -t ATLHECTB -d OUTPUT |  Results for two Geant4 versions, can be compared to tag 2.0_1 and 2.0_2.|
| v2.0 <br /> Dataset #2 <br /> tag 2.0_2 (Geant4.10.07.p01, ATLHECTB v2.0, FTFP_BERT_ATL, QGSP_BERT, FTFP_BERT_INCLXX) <br /> ~Added on 18/10/2021~ Removed on 2/5/2023 <br /> | python mc-config-generator.py submit -t ATLHECTB -d OUTPUT -v 10.7.p01 -q "testmatch" -r | python mc-config-generator.py parse -t ATLHECTB -d OUTPUT | Everything identical to 2.0_1 but changed physics lists in params.conf (change it yourself to reproduce data).|
| v2.0 <br /> Dataset #1 <br /> tag 2.0_1 (Geant4.10.07.p01, ATLHECTB v2.0, FTFP_BERT) <br /> ~Added on 11/10/2021~ Removed on 2/5/2023 <br /> | python mc-config-generator.py submit -t ATLHECTB -d OUTPUT -v 10.7.p01 -q "testmatch" -r | python mc-config-generator.py parse -t ATLHECTB -d OUTPUT | First results on Geant Val, using Geant4.10.07.p01, ATLHECTB v2.0, FTFP_BERT. Analysis coded in parser.py. JSON files for test-beam data are created with parser.py (end of file). |

</p>
</details>

<!--Available datasets and analyses-->
## Available datasets and analyses
We provide datasets and ROOT analyses, as well as instructions for their reproducibility.
Ask authors for access to datasets. From v2.0 on results are deployed on Geant Val, refer to the [List of results on Geant Val](#list-of-results-on-geant-val).
<details><summary>Results-Table</summary>
<p>

| ATLHECTB         | Reproduce data | Reproduce analysis | Comments     |
| -------------    | ----------     | -----------        | -----------  |
| v2.6.1 <br /> Dataset #2 <br /> tag 2.6.1_2 <br /> Created on 3/9/2023 | Geant4.10.6.3 (modified) <br /> ATLHECTB v2.6.1 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_beam4_run.mac -pl FTFP_BERT | Analysis reproducable with v2.6.1 generic-analysis `root 'ATLHECTBanalysis.C("Data1/","2.6.1","10.6.3","FTFP_BERT modified")'` | Results to test the new FTF parameters to fix the fluctuations disprepancy with respect to ATLAS data. Excitation energy per wounded nucleon set to 50 MeV. |
| v2.6.1 <br /> Dataset #1 <br /> tag 2.6.1_1 <br /> Created on 17/8/2023 | Geant4.10.6.3 (modified) <br /> ATLHECTB v2.6.1 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_beam4_run.mac -pl FTFP_BERT | Analysis reproducable with v2.6.1 generic-analysis `root 'ATLHECTBanalysis.C("Data1/","2.6.1","10.6.3","FTFP_BERT modified")'` | Results to test the new FTF parameters to fix the fluctuations disprepancy with respect to ATLAS data. |
| v2.6 <br /> Dataset #1 <br /> tag 2.6_1 | Geant4.11.1.ref05 <br /> ATLHECTB v2.6 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_beam4_run.mac -pl FTFP_BERT or use it with the FLUKA.CERN interface | Analysis reproducable with analysis/v2.6 root 'ATLHECTBanalysis1_v2p6.C(true)' | Results to compare G4-FTFP_BERT with the FTFP_BERT PL using the FLUKA.CERN interface as on Geant4-11.1.ref05. See [how to use it](#use-flukacern-hadron-inelastic-process). |
| v2.5 <br /> Dataset #1 <br /> tag 2.5_1 | Geant4.10.7.p01 <br /> ATLHECTB v2.5 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_beam4_run.mac -pl FTFP_BERT | root -l <br /> .x v2.5/ATLHECTBanalysis1_v2p5.C | Analysis reproducable with v2.5 <br /> results for e- and pi-, assuming Data1/ is alongside root macro, including all modification from v2.5. These results compared with success with ATLAS (Andrey's) ones. |
| v2.4 <br /> Dataset #1 <br /> tag 2.4_1 | Geant4.10.7.p03 <br /> ATLHECTB v2.4 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_beam3_run.mac -pl FTFP_BERT | root -l <br /> .x v2.4/ATLHECTBanalysis1_v2p4.C | Analysis reproducable with v2.4 <br /> results for e- and pi-, assuming Data1/ is alongside root macro, added scintillators in sim + new pi/e and shower length analysis |
| v1.7 <br /> Dataset #1 <br /> tag 1.7_1 | Geant4.10.07.p01 <br /> ATLHECTB v1.7 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_beam3_run.mac -pl FTFP_BERT | root -l <br /> .x v1.7/ATLHECTBanalysis1_v1p7.C  | Analysis reproducable with v1.7 <br /> results for e- and pi-, assuming Data1/ is alongside root macro, new beam alignment and new em resolution study w.r.t. v1.6 |
| v1.6 <br /> Dataset #1 <br /> tag 1.6_1 | Geant4.10.07.p01 <br /> ATLHECTB v1.6 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_beam2_run.mac -pl FTFP_BERT | root -l <br /> .x v1.6/ATLHECTBanalysis1_v1p6.C  | Analysis reproducable with v1.6 <br /> results for e- and pi-, assuming Data1/ is alongside root macro, G4 added new timing scheme for layers' signals (see v1.6 description) w.r.t v1.5 |
| v1.5 <br /> Dataset #1 <br /> tag 1.5_1 | Geant4.10.07.p01 <br /> ATLHECTB v1.5 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_beam2_run.mac -pl FTFP_BERT | root -l <br /> .x v1.5/ATLHECTBanalysis1_v1p5.C  | Analysis reproducable with v1.5 <br /> results for e- and pi-, assuming Data1/ is alongside root macro, G4 HEC+Cryostat+World materials defined as ATLAS code w.r.t v1.3 |
| v1.4 <br /> Dataset #1 <br /> tag 1.4_1 | Geant4.10.07.p01 <br /> ATLHECTB v1.4 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_beam2_run.mac -pl FTFP_BERT | root -l <br /> .x v1.4/ATLHECTBanalysis1_v1p4.C  | Analysis reproducable with v1.5 <br /> results for e- and pi-, assuming Data1/ is alongside root macro, G4 HEC materials defined as ATLAS code w.r.t v1.3 |
| v1.3 <br /> Dataset #1 <br /> tag 1.3_1 | Geant4.10.07.p01 <br /> ATLHECTB v1.3 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_beam2_run.mac -pl FTFP_BERT | root -l <br /> .x v1.3/ATLHECTBanalysis1_v1p3.C  | Analysis reproducable with v1.5 <br /> results for e- and pi-, assuming Data1/ is alongside root macro, see v1.3 details for G4 changes w.r.t. v1.2 |
| v1.2 <br /> Dataset #1 <br /> tag 1.2_1 | Geant4.10.07.p01 <br /> ATLHECTB v1.2 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_beam1_run.mac -pl FTFP_BERT | root -l <br /> .x ATLHECTBanalysis1_v1p2.C  | Analysis reproducable with v1.2 <br /> results for e- and pi-, assuming Data1/ is alongside root macro, Dataset is identical to tag 1.0_1 but with different beam position |
| v1.0 <br /> Dataset #1 <br /> tag 1.0_1 | Geant4.10.07.p01 <br /> ATLHECTB v1.0 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_run.mac -pl FTFP_BERT | root -l <br /> .x ATLHECTBanalysis1_v1p1.C  | Analysis reproducable with v1.1 <br /> results for e- and pi-, assuming Data1/ is alongside root macro, Dataset is identical to v1.0 |
| v1.0 <br /> Dataset #1 <br /> tag 1.0_1 | Geant4.10.07.p01 <br /> ATLHECTB v1.0 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_run.mac -pl FTFP_BERT | root -l <br /> .x ATLHECTBanalysis1_v1p0.C  | reproducable with v1.0 <br /> results for e-, assuming Data1/ is alongside root macro |

</p>
</details>

<!--How to:-->
## How to

### Build, compile and execute on Mac/Linux
1. git clone the repo
   ```sh
   git clone https://github.com/lopezzot/ATLHECTB.git
   ```
2. source Geant4 env
   ```sh
   source /relative_path_to/geant4.10.07_p01-install/bin/geant4.sh
   ```
3. cmake build directory and make (using geant4.10.07_p01)
   ```sh
   mkdir ATLHECTB-build; cd ATLHECTB-build/
   cmake -DGeant4_DIR=/absolute_path_to/geant4.10.07_p01-install/lib/Geant4-10.7.1/ relative_path_to/ATLHECTB/
   make
   ```
4. execute (example with ATLHECTB_run.mac macro card, 2 threads and FTFP_BERT physics list)
   ```sh
   ./ATLHECTB -m ATLHECTB_run.mac -t 2 -pl FTFP_BERT
   ```
Parser options
   * -m macro.mac: pass a Geant4 macro card (example -m ATLHECTB_run.mac available in source directory and automatically copied in build directory) 
   * -t integer: pass number of threads for multi-thread execution (example -t 3, default t=2)
   * -pl Physics_List: select Geant4 physics list (example -pl FTFP_BERT)
   * It is possible to select alternative FTF tunings with PL_tuneID (example -pl FTFP_BERT_tune0) [only for Geant4-11.1.0 or higher]

Custom CMake options
   * `WITH_LEAKAGEANALYSIS`: if set to `ON` include the usage of the `SpectrumAnalysis` singleton to study the particle leakage (default `OFF`)
5. (optional) It is possible to install the executable in `bin` under `CMAKE_INSTALL_PREFIX`
   ```sh
   cmake -DCMAKE_INSTALL_PREFIX=/absolute-path-to-installdir/ -DGeant4_DIR=/absolute_path_to/geant4.10.07_p01-install/lib/Geant4-10.7.1/ relative_path_to/ATLHECTB/
   make
   make install
   ```
   Likely, to execute from the install directory you have to first update the library env
   ```sh
   DYLD_LIBRARY_PATH="/absolute_path_to/geant4.10.07_p01-install/lib/:$DYLD_LIBRARY_PATH"
   cd install_dir
   ./ATLHECTB
   ```
### Build, compile and execute on lxplus
1. git clone the repo
   ```sh
   git clone https://github.com/lopezzot/ATLHECTB.git
   ```
2. cmake build directory and make (using geant4.10.07_p01, check for gcc and cmake dependencies for other versions)
   ```sh
   mkdir ATLHECTB-build; cd ATLHECTB-build/
   source /cvmfs/sft.cern.ch/lcg/contrib/gcc/8.3.0/x86_64-centos7/setup.sh 
   source /cvmfs/geant4.cern.ch/geant4/10.7.p01/x86_64-centos7-gcc8-optdeb-MT/CMake-setup.sh 
   export CXX=`which g++`
   export CC=`which gcc`
   cmake3 -DGeant4_DIR= /cvmfs/geant4.cern.ch/geant4/10.7.p01/x86_64-centos7-gcc8-optdeb-MT/lib64/Geant4-10.7.1 ../ATLHECTB/
   make
   ```
   Hint: cp and source the scripts/ATLHECTB_lxplus_10.7.p01.sh file in the build directory.
3. execute (example with ATLHECTB_run.mac macro card, 2 threads and FTFP_BERT physics list)
   ```sh
   ./ATLHECTB -m ATLHECTB_run.mac -t 2 -pl FTFP_BERT
   ```
   
### Submit a job with HTCondor on lxplus
1. git clone the repo
   ```sh
   git clone https://github.com/lopezzot/ATLHECTB.git
   ```
2. prepare execution files (example with Geant4.10.07_p01, ATLHECTB_run.mac, 2 threads, FTFP_BERT physics list)
    ```sh
    mkdir ATLHECTB-build; cd ATLHECTB-build
    mkdir error log output
    cp ../ATLHECTB/scripts/ATLHECTB_lxplus_10.7.p01.sh .
    source ATLHECTB_lxplus_10.7.p01.sh
    ```
3. prepare for HTCondor submission (example with Geant4.10.07_p01, ATLHECTB_run.mac, 2 threads, FTFP_BERT physics list)
    ```sh
    cp ../ATLHECTB/scripts/ATLHECTB_HTCondor_10.7.p01.sh .
    export MYHOME=`pwd`
    echo cd $MYHOME >> ATLHECTB_HTCondor_10.7.p01.sh
    echo $MYHOME/ATLHECTB -m $MYHOME/ATLHECTB_run.mac -t 2 >> ATLHECTB_HTCondor_10.7.p01.sh
    cp ../ATLHECTB/scripts/ATLHECTB_HTCondor.sub .
    sed -i '1 i executable = ATLHECTB_HTCondor_10.7.p01.sh' ATLHECTB_HTCondor.sub
    ```
4. submit a job
   ```sh
   condor_submit ATLHECTB_HTCondor.sub 
   ```
5. monitor the job
   ```sh
   condor_q
   ```
   or (for persistency)
   ```sh
   condor_wait -status log/*.log
   ```
6. additional info from HTCondor (optional) \
   rm all your jobs
    ```sh
   condor_rm username
   ```
   inspect your accounting group
   ```sh
   condor_q owner $LOGNAME -long | grep '^AccountingGroup' | sort | uniq -c
   ```
   display all accounting groups
   ```sh
   haggis group list
   ```
   display your accounting groups
    ```sh
   haggis rights
   ```
   check what accounting group a job has
   ```sh
   condor_q jobid.0 -af AccountingGroup
   ```
   specify the accounting group for yout job, to be added in the .sub file
   ```sh
   +AccountingGroup = "group_u_*"
   ```
   check job CPU usage
   ```sh
   condor_q -l jobid.0 | grep CPUsUsage
   ```
   ssh to machine where job is running
   ```sh
   condor_ssh_to_job jobid.0
   ```
   
### Get ATLAS HEC geo parameters from mysql database
Caveat: ATLHECTB geo parameters are hardcoded in the DetectorConstruction, instructions to get geo parameters from ATLAS mysql database are intended for comparisons only.

1. access mysql database
   ```sh 
   mysql --host=dbod-tbnova.cern.ch --user=reader --password=askauthor --port=5501 atlas
   ```
2. get ATLAS HEC main geo parameters
   ```sh
   SELECT version.name, version.type, version.value, version.comment FROM structure as directory LEFT JOIN structure ON \
   directory.ID=structure.parent LEFT JOIN relation ON structure.ID=relation.strID LEFT JOIN parameter as version ON \
   version.ID=relation.parID WHERE directory.name="HENDGEO" AND structure.name="HENG";
   ```
   
   - Also available at the ATLAS geo web database: [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://atlas-geometry-db.web.cern.ch/atlas-geometry-db/show_tagged_node.php?node_id=NjA0NA==&tag_id=MTA2MTI1&user=QVRMQVNERF9SRUFERVI=)

3. get ATLAS HEC logitudinal block geo parameters
   ```sh
   SELECT version.name, version.type, version.value, version.comment FROM structure as directory LEFT JOIN structure ON \
   directory.ID=structure.parent LEFT JOIN relation ON structure.ID=relation.strID LEFT JOIN parameter as version ON \
   version.ID=relation.parID WHERE directory.name="HENDGEO" AND structure.name="HBLO";
   ```
   
   - Also available at the ATLAS geo web database: [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://atlas-geometry-db.web.cern.ch/atlas-geometry-db/show_tagged_node.php?node_id=NjA0NQ==&tag_id=MTA2MTI2&user=QVRMQVNERF9SRUFERVI=)
   
4. get all parameters (use only to visualize each vector entry)
   ```sh
   SELECT * FROM parameter;
   ```
   
### Dump ATLHECTB GDML geometry description file
We support GDML geometry description. By default it is not active, to activate it, modify the ATLHECTBDetectorConstruction fDumpGDMLgeo data member, as follows
   ```c++
   ATLHECTBDetectorConstruction::ATLHECTBDetectorConstruction()
    : G4VUserDetectorConstruction(),
      fCheckOverlaps(false),
      fDumpGDMLgeo(true){}
   ```
At the first execution, it will create the ATLHECTBgeo.gdml file with the up to date GDML geometry description.

### Use Fluka.Cern hadron inelastic process
`Geant4-11.1-ref05` introduces a Fluka.Cern interface to use the Fluka.Cern hadron inelastic process in any geant4 application as explained in `examples/extended/hadronic/FlukaCern`. The following are my instructions to use this repo with a customized FTFP_BERT physics list using it. It assumes that cvmfs is mounted.
1. Install Fluka.Cern from source code (example with fluka4-3.3)
   ```sh
   source /cvmfs/sft.cern.ch/lcg/contrib/gcc/10.1.0/x86_64-centos7/setup.sh
   cd fluka4-3.3 && make -j 4
   cd src/ && make cpp_headers
   mkdir /path-to/fluka4-3.3-install && make install DESTDIR=/path-to/fluka4-3.3-install/
   PATH="/users/lopezzot/fluka4-3.3-install/bin/":$PATH
   ```
2. Setup `geant4-11.1.ref05` and compile the fluka interface as in the example
   ```sh
   source /cvmfs/geant4.cern.ch/geant4/11.1.ref05/x86_64-centos7-gcc10-optdeb-MT/CMake-setup.sh 
   source /cvmfs/geant4.cern.ch/geant4/11.1.ref05/x86_64-centos7-gcc10-optdeb-MT/bin/geant4.sh 
   cd FlukaCern/FlukaInterface/
   make interface
   make env
   source env_FLUKA_G4_interface.sh 
   ```
3. Build and execute ATLHECTB
   ```sh
   git clone https://github.com/lopezzot/ATLHECTB.git
   mkdir ATLHECTB-build && cd ATLHECTB-build
   /cvmfs/sft.cern.ch/lcg/contrib/CMake/3.23.2/Linux-x86_64/bin/cmake -DG4_USE_FLUKA=1 ../ATLHECTB/
   make
   ```
   NOTE: the Fluka.Cern interface can only be used in single-threaded mode.

**[⬆ back to top](#atlhectb)**

<!--Geant4 and Github Actions-->
## Geant4 and Github Actions
⚠️ ATLHECTB is automatically tested on every ``push`` against Geant4.11.0 and the latest Geant4 release via GitHub Actions on ``ubuntu-latest``.

Geant4 binaries are available [here](https://github.com/hahnjo/geant4-actions-binaries). If you want your Geant4 application to be tested automatically, feel free to use this [workflow](https://github.com/lopezzot/ATLHECTB/blob/main/.github/workflows/geant4-test.yml).

<!--Bugs report-->
## Bugs report
List of bugs found.
1. 🐛 Using ATLHECTB v2.1 and Geant4.10.4(.p01, .p03) executing in multi-threaded mode on lxplus via Geant Val, the M2L2BirkeLayer branch in root output file is out-of-sync with rest of file. Using single-thread mode the output file is correct. Reported by Lorenzo Pezzotti on 25/10/2021.

<!--My quick Geant4 installation-->
## My quick Geant4 installation
Here is my standard Geant4 installation (example with Geant4.10.7.p01) starting from the unpacked geant4.10.07.tar.gz file under the example path "path/to".

1. create build directory alongside source files
      ```sh
   cd /path/to
   mkdir geant4.10.07-build
   cd geant4.10.07-build
   ```
2. link libraries with CMAKE (example with my favourite libraries)
   ```sh
   cmake -DCMAKE_INSTALL_PREFIX=/Users/lorenzo/myG4/geant4.10.07_p01-install \
   -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_QT=ON -DGEANT4_BUILD_MULTITHREADED=ON \
   -DGEANT4_USE_GDML=ON ../geant4.10.07.p01
   ```
3. make it
   ```sh
   make -jN
   make install
   ```
   
<!--Selected ATLAS HEC references-->
## Selected ATLAS HEC references
- 📄 <em>Performance of the ATLAS Hadronic Endcap Calorimeter in beam tests: Selected results</em>, ATL-LARG-PUB-2022-001: [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://cds.cern.ch/record/2811731) - Experimental data extracted from this reference
- 📄 <em>GEANT4 physics evaluation with testbeam data of the ATLAS hadronic end-cap calorimeter</em>, NIM A560 (2006): [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://www.sciencedirect.com/science/article/pii/S0168900205026835)
- 📄 <em>Performance of the ATLAS hadronic end-cap calorimeter in beam tests</em>, NIM A482 (2002): [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://www.sciencedirect.com/science/article/pii/S0168900201013389)

**[⬆ back to top](#atlhectb)**
