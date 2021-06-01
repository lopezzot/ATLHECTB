# ATLHECTB
**A Geant4 simulation of the ATLAS hadronic end-cap calorimeter beam tests.**

<figure>
<img src="https://github.com/lopezzot/ATLHECTB/blob/main/images/ATLHECTB_movie.gif" alt="Trulli" style="width:100%">
<figcaption align="center"><b>Fig. - 10 GeV muon passing through the ATLAS HEC.</b></figcaption>
</figure>

<br/><br/>

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#project-description">Project description</a></li>
    <li><a href="#authors-and-contacts">Authors and contacts</a></li>
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
- Status: under development

<!--Authors and contacts-->
## Authors and contacts
- Lorenzo Pezzotti (CERN EP-SFT) - lorenzo.pezzotti@cern.ch 
- Supervisor: Alberto Ribon (CERN EP-SFT)

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
    cp ../ATLHECTB/script/ATLHECTB_HTCondor.sub .
    cp ../ATLHECTB/script/ATLHECTB_lxplus_10.7.p01.sh .
    echo "./ATLHECTB -m ATLHECTB_run.mac -t 2 -pl FTFP_BERT" >> ATLHECTB_lxplus_10.7.p01.sh
    sed -i '1 i executable = ATLHECTB_lxplus_10.7.p01.sh' ATLHECTB_HTCondor.sub
    ```
3. submit a job
   ```sh
   condor_submit ATLHECTB_HTCondor.sub 
   ```
4. monitor the job
   ```sh
   condor_q
   ```
   or (for persistency)
   ```sh
   condor_wait -status log/*.log
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
- Geant4 evaluation with test-beam data (NIM A560 (2006)):  [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://www.sciencedirect.com/science/article/pii/S0168900205026835)
- Beam tests performance (NIM A482 (2002)): [![Website shields.io](https://img.shields.io/website-up-down-green-red/http/shields.io.svg)](https://www.sciencedirect.com/science/article/pii/S0168900201013389)
