# ATLHECTB
**A Geant4 simulation of the ATLAS hadronic end-cap calorimeter beam tests.** 
[![Website](https://img.shields.io/website?down_message=cannot%20read%20the%20docs&label=ATLHECTB%20docs&logo=GitHub&up_message=always%20read%20the%20docs&url=https%3A%2F%2Flopezzot.github.io%2FATLHECTB%2F)](https://lopezzot.github.io/ATLHECTB/)

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
        <li><a href="#selected-presentations">Selected presentations</a></li>
      </ul>
    </li>
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
      </ul>
    </li>
    </li><li><a href="#my-quick-geant4-installation">My quick Geant4 installation</a></li>
    <li><a href="#selected-atlas-hec-references">Selected ATLAS HEC references</a>
  </ol>                                           
</details>

<!--Project desription-->
## Project description
The project targets a standalone Geant4 simulation of the ATLAS hadronic end-cap calorimeter beam tests to perform Geant4 regression testing, physics lists comparison and validation against test-beam data. 
- Start date: 11 May 2021 
- Status: from v1.0 on ATLHECTB is available for data production, from v2.0 on ATLHECTB is available on Geant Val

<!--Authors and contacts-->
## Authors and contacts
- Lorenzo Pezzotti (CERN EP-SFT) - lorenzo.pezzotti@cern.ch 
- Supervisor: Alberto Ribon (CERN EP-SFT)

<!--Documentation and results-->
## Documentation and results

### Selected presentations
- CERN EP-SFT Simulation Meeting 13/7/2021, **Results from the Geant4 ATLAS HEC test-beam simulation** [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://indico.cern.ch/event/1058649/contributions/4449544/attachments/2280962/3875541/G4Sim_lopezzot_13_7_2021.pdf)
- ATLAS Simulation Group Meeting 6/7/2021 (ATLAS restricted), **A Geant4 simulation of
the ATLAS HEC beam tests** [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://indico.cern.ch/event/995938/contributions/4421574/attachments/2277065/3868501/ATLASSim_lopezzot_6_7_2021.pdf) <em>(errata corrige: slide 11, EstBoard 0.8 mm Kapton -> 0.61 mm Kapton)</em>
- CERN EP-SFT Simulation Meeting 15/6/2021, **First results from the Geant4 ATLAS HEC test beam simulation** [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://indico.cern.ch/event/1049152/contributions/4407943/attachments/2264354/3844295/G4SW_lopezzot_15_6_2021.pdf) <em>(errata corrige: slide 9, EstBoard 0.8 mm Kapton -> 0.61 mm Kapton)</em>

**[⬆ back to top](#atlhectb)**

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
The following are results deployed on Geant Val so far.

| ATLHECTB         | Reproduce data | Reproduce analysis | Comments     |
| -------------    | ----------     | -----------        | -----------  |
| v2.0 <br /> Dataset #1 <br /> tag 2.0_1 (Geant4.10.07.p01, ATLHECTB v2.0, FTFP_BERT) <br /> Added on 11/10/2021 <br /> | python mc-config-generator.py submit -t ATLHECTB -d OUTPUT -v 10.7.p01 -q "testmatch" -r | python mc-config-generator.py parse -t ATLHECTB -d OUTPUT | First results on Geant Val, using Geant4.10.07.p01, ATLHECTB v2.0, FTFP_BERT. Analysis coded in parser.py. JSON files for test-beam data are created with parser.py (end of file). |

**[⬆ back to top](#atlhectb)**

<!--Available datasets and analyses-->
## Available datasets and analyses
We provide datasets and ROOT analyses, as well as instructions for their reproducibility.
Ask authors for access to datasets.

| ATLHECTB         | Reproduce data | Reproduce analysis | Comments     |
| -------------    | ----------     | -----------        | -----------  |
| v1.7 <br /> Dataset #1 <br /> tag 1.7_1 | Geant4.10.07.p01 <br /> ATLHECTB v1.7 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_beam3_run.mac -pl FTFP_BERT | root -l <br /> .x v1.7/ATLHECTBanalysis1_v1p7.C  | Analysis reproducable with v1.7 <br /> results for e- and pi-, assuming Data1/ is alongside root macro, new beam alignment and new em resolution study w.r.t. v1.6 |
| v1.6 <br /> Dataset #1 <br /> tag 1.6_1 | Geant4.10.07.p01 <br /> ATLHECTB v1.6 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_beam2_run.mac -pl FTFP_BERT | root -l <br /> .x v1.6/ATLHECTBanalysis1_v1p6.C  | Analysis reproducable with v1.6 <br /> results for e- and pi-, assuming Data1/ is alongside root macro, G4 added new timing scheme for layers' signals (see v1.6 description) w.r.t v1.5 |
| v1.5 <br /> Dataset #1 <br /> tag 1.5_1 | Geant4.10.07.p01 <br /> ATLHECTB v1.5 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_beam2_run.mac -pl FTFP_BERT | root -l <br /> .x v1.5/ATLHECTBanalysis1_v1p5.C  | Analysis reproducable with v1.5 <br /> results for e- and pi-, assuming Data1/ is alongside root macro, G4 HEC+Cryostat+World materials defined as ATLAS code w.r.t v1.3 |
| v1.4 <br /> Dataset #1 <br /> tag 1.4_1 | Geant4.10.07.p01 <br /> ATLHECTB v1.4 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_beam2_run.mac -pl FTFP_BERT | root -l <br /> .x v1.4/ATLHECTBanalysis1_v1p4.C  | Analysis reproducable with v1.5 <br /> results for e- and pi-, assuming Data1/ is alongside root macro, G4 HEC materials defined as ATLAS code w.r.t v1.3 |
| v1.3 <br /> Dataset #1 <br /> tag 1.3_1 | Geant4.10.07.p01 <br /> ATLHECTB v1.3 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_beam2_run.mac -pl FTFP_BERT | root -l <br /> .x v1.3/ATLHECTBanalysis1_v1p3.C  | Analysis reproducable with v1.5 <br /> results for e- and pi-, assuming Data1/ is alongside root macro, see v1.3 details for G4 changes w.r.t. v1.2 |
| v1.2 <br /> Dataset #1 <br /> tag 1.2_1 | Geant4.10.07.p01 <br /> ATLHECTB v1.2 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_beam1_run.mac -pl FTFP_BERT | root -l <br /> .x ATLHECTBanalysis1_v1p2.C  | Analysis reproducable with v1.2 <br /> results for e- and pi-, assuming Data1/ is alongside root macro, Dataset is identical to tag 1.0_1 but with different beam position |
| v1.0 <br /> Dataset #1 <br /> tag 1.0_1 | Geant4.10.07.p01 <br /> ATLHECTB v1.0 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_run.mac -pl FTFP_BERT | root -l <br /> .x ATLHECTBanalysis1_v1p1.C  | Analysis reproducable with v1.1 <br /> results for e- and pi-, assuming Data1/ is alongside root macro, Dataset is identical to v1.0 |
| v1.0 <br /> Dataset #1 <br /> tag 1.0_1 | Geant4.10.07.p01 <br /> ATLHECTB v1.0 <br /> ./ATLHECTB -m runcards/ATLHECTBescan_run.mac -pl FTFP_BERT | root -l <br /> .x ATLHECTBanalysis1_v1p0.C  | reproducable with v1.0 <br /> results for e-, assuming Data1/ is alongside root macro |

**[⬆ back to top](#atlhectb)**

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

**[⬆ back to top](#atlhectb)**

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
- <em>GEANT4 physics evaluation with testbeam data of the ATLAS hadronic end-cap calorimeter</em>, NIM A560 (2006): [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://www.sciencedirect.com/science/article/pii/S0168900205026835)
- <em>Performance of the ATLAS hadronic end-cap calorimeter in beam tests</em>, NIM A482 (2002): [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://www.sciencedirect.com/science/article/pii/S0168900201013389)

**[⬆ back to top](#atlhectb)**
