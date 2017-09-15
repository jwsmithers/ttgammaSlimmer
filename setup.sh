export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
alias setupATLAS='source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh'

setupATLAS -q
lsetup "root 6.10.04-x86_64-slc6-gcc62-opt"
lsetup "cmake 3.7.0"
