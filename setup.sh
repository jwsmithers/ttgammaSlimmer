export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
alias setupATLAS='source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh'

setupATLAS -q
localSetupROOT -q
#localSetupSFT -q releases/LCG_84/eigen/3.2.7
#localSetupBoost -q boost-1.60.0-python2.7-x86_64-slc6-gcc49
#EIGEN_INC=$SFT_HOME_eigen/include/eigen3
#BOOST_INC=$ALRB_BOOST_ROOT/include
#export CPLUS_INCLUDE_PATH=$EIGEN_INC:$BOOST_INC
lsetup "cmake 3.6.0"
