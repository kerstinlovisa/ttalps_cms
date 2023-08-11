#!/bin/bash

#source /osg/osg3.2/osg-wn-client/setup.sh
export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc700
export CMSSW_GIT_REFERENCE=/cvmfs/cms.cern.ch/cmssw.git

#INPUT: $1: part $2: mass $3: event/job

home_dir=/afs/cern.ch/user/l/lrygaard/TTALPs/MC_sample_production/LHEtoNanoAOD
mass=`echo "$2" | tr . p`
file_collection=tta_mAlp-${mass}GeV_nEvents-$3
file_name=${file_collection}_part-${1}

input_dir=/eos/user/l/lrygaard/TTALP/signal_LHE_files/$file_collection/
output_dir=/eos/user/l/lrygaard/TTALP/LHEtoNanoAOD_output
echo gunzip $input_dir$file_name.lhe.gz
gunzip $input_dir/$file_name.lhe.gz

save_allsteps=false

condordir=`echo $_CONDOR_SCRATCH_DIR`
if [ -z "$condordir" ]
then
  condordir=$home_dir/tmp
  mkdir $condordir
fi

echo $1 $mass $3 $condordir
echo $home_dir
echo Input files stored in: $input_dir
echo Output files stored in: output_dir/$file_collection

pwd
mkdir -p $output_dir/UL_MiniAOD/$file_collection
mkdir -p $output_dir/UL_NanoAOD/$file_collection
cd $condordir
mkdir -p output/UL_MiniAOD/$file_collection
mkdir -p output/UL_NanoAOD/$file_collection

echo cp $home_dir/Hadronizers/run_GEN_ttalp_noCopy.py $condordir
cp $home_dir/Hadronizers/run_GEN_ttalp_noCopy.py $condordir
pwd

cmsrel CMSSW_10_6_30_patch1
cd CMSSW_10_6_30_patch1/src
eval `scramv1 runtime -sh`
cd $condordir

export HOME=$home_dir
echo 1_run_GEN
ls $condordir
pwd
cmsRun run_GEN_ttalp_noCopy.py $1 $file_name $condordir $3 $input_dir
if [ "$save_allsteps" = true ] 
then
  cp ${file_name}_GENSIM_${1}.root $output_dir/$file_collection/.
fi

cmsrel CMSSW_10_6_19_patch3
cd CMSSW_10_6_19_patch3/src
eval `scramv1 runtime -sh`
export HOME=$home_dir

echo 2_run_SIM
cp $home_dir/Hadronizers/run_SIM_noCopy.py $condordir
cd $condordir
pwd
cmsRun run_SIM_noCopy.py $(($1+0)) $file_name $condordir
if [ "$save_allsteps" = true ] 
then
  cp ${file_name}_SIM_${1}.root $output_dir/$file_collection/.
fi

cmsrel CMSSW_10_6_17_patch1
cd CMSSW_10_6_17_patch1/src
eval `scramv1 runtime -sh`
export HOME=$home_dir


echo 3_run_DIGIPremix
cp $home_dir/Hadronizers/run_DIGIPremix_noCopy.py $condordir
cd $condordir
pwd
cmsRun run_DIGIPremix_noCopy.py $(($1+0)) $file_name $condordir
if [ "$save_allsteps" = true ] 
then
  cp ${file_name}_DIGIPremix_${1}.root $output_dir/$file_collection/.
fi

cmsrel CMSSW_10_2_16_UL
cd CMSSW_10_2_16_UL/src
eval `scramv1 runtime -sh`
export HOME=$home_dir


echo 4_run_HLT
cp $home_dir/Hadronizers/run_HLT_noCopy.py $condordir
cd $condordir
pwd
cmsRun run_HLT_noCopy.py $(($1+0)) $file_name $condordir
if [ "$save_allsteps" = true ] 
then
  cp ${file_name}_HLT_${1}.root $output_dir/$file_collection/.
fi

cd CMSSW_10_6_17_patch1/src
eval `scramv1 runtime -sh`
export HOME=$home_dir


echo 5_run_RECO
cp $home_dir/Hadronizers/run_RECO_noCopy.py $condordir
cd $condordir
pwd
cmsRun run_RECO_noCopy.py $(($1+0)) $file_name $condordir
if [ "$save_allsteps" = true ] 
then
  cp ${file_name}_RECO_${1}.root $output_dir/$file_collection/.
fi

cmsrel CMSSW_10_6_20
cd CMSSW_10_6_20/src
eval `scramv1 runtime -sh`
export HOME=$home_dir


echo 6_run_MiniAOD
cp $home_dir/Hadronizers/run_MiniAOD_noCopy.py $condordir
mkdir -p $condordir/output/UL_MiniAOD/$file_collection
cd $condordir
pwd
cmsRun run_MiniAOD_noCopy.py $(($1+0)) $file_collection $file_name $condordir
cp output/UL_MiniAOD/$file_collection/${file_name}_MiniAOD_$(($1+0)).root $output_dir/UL_MiniAOD/$file_collection/.

cmsrel CMSSW_10_6_29
cd CMSSW_10_6_29/src
eval `scramv1 runtime -sh`
export HOME=$home_dir


echo 7_run_NanoAOD
cp $home_dir/Hadronizers/run_NanoAOD.py $condordir
mkdir -p $condordir/output/UL_NanoAOD/$file_collection
cd $condordir
pwd
cmsRun run_NanoAOD.py $(($1+0)) $file_collection $file_name $condordir
cp output/UL_NanoAOD/$file_collection/${file_name}_NanoAOD_$(($1+0)).root $output_dir/UL_NanoAOD/$file_collection/.

if [ -z "$condordir" ]
then
  rm -r $condordir
fi