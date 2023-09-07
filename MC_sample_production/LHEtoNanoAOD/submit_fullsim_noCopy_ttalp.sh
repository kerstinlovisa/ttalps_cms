#!/bin/bash

#source /osg/osg3.2/osg-wn-client/setup.sh
export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc700
export CMSSW_GIT_REFERENCE=/cvmfs/cms.cern.ch/cmssw.git

#INPUT: $1: processId $2: part $3: mass $4: tot events $5: events/job for SIM

production_dir=$(pwd)
mass=`echo "$3" | tr . p`

# Two ways to run: 
# Either on 1 input lhe which gets divided into smaller files in the SIM step, 
# given number of events/job
# Or run over all events in several lhe-files, if part = -1 ($2)
file_collection=tta_mAlp-${mass}GeV_nEvents-$4
# input_dir=/eos/user/l/lrygaard/TTALP/signal_LHE_files/$file_collection/
# output_dir=/eos/user/l/lrygaard/TTALP/LHEtoNanoAOD_output
input_dir=/nfs/dust/cms/user/lrygaard/ttalps_cms/signal_LHE_files/$file_collection/
output_dir=/nfs/dust/cms/user/lrygaard/ttalps_cms/LHEtoNanoAOD_output
file_name=$file_collection
file_part=$1
eventsperjob=$5
if [ "$2" != -1 ] #part
then
  file_name=${file_name}_part-$2
else
  file_name=${file_name}_part-$1
  eventsperjob=0
fi

echo gunzip $input_dir$file_name.lhe.gz
gunzip $input_dir/$file_name.lhe.gz

save_allsteps=false

condordir=`echo $_CONDOR_SCRATCH_DIR`
if [ -z "$condordir" ]
then
  condordir=$production_dir/tmp
  mkdir $condordir
fi

echo processId: $1, mass: $mass, part: $2, events: $4, events/job: $5, condordir: $condordir
echo Directory for NanoAOD production: $production_dir
echo Input files stored in: $input_dir
echo Output files stored in: output_dir/$file_collection

mkdir -p $output_dir/UL_MiniAOD/$file_collection
mkdir -p $output_dir/UL_NanoAOD/$file_collection
cd $condordir
mkdir -p output/UL_MiniAOD/$file_collection
mkdir -p output/UL_NanoAOD/$file_collection

echo cp $production_dir/Hadronizers/run_GEN_ttalp_noCopy.py $condordir
cp $production_dir/Hadronizers/run_GEN_ttalp_noCopy.py $condordir

cmsrel CMSSW_10_6_30_patch1
cd CMSSW_10_6_30_patch1/src
eval `scramv1 runtime -sh`
cd $condordir

echo 1_run_GEN
# inputs: jobID,  file_name,  condordir,  events/job,  filedir
cmsRun run_GEN_ttalp_noCopy.py $1 $file_name $condordir $eventsperjob $input_dir
if [ "$save_allsteps" = true ] 
then
  cp ${file_name}_GENSIM_${1}.root $output_dir/$file_collection/.
fi

cmsrel CMSSW_10_6_19_patch3
cd CMSSW_10_6_19_patch3/src
eval `scramv1 runtime -sh`

echo 2_run_SIM
cp $production_dir/Hadronizers/run_SIM_noCopy.py $condordir
cd $condordir
cmsRun run_SIM_noCopy.py $(($1+0)) $file_name $condordir
if [ "$save_allsteps" = true ] 
then
  cp ${file_name}_SIM_${1}.root $output_dir/$file_collection/.
fi

cmsrel CMSSW_10_6_17_patch1
cd CMSSW_10_6_17_patch1/src
eval `scramv1 runtime -sh`

echo 3_run_DIGIPremix
cp $production_dir/Hadronizers/run_DIGIPremix_noCopy.py $condordir
cd $condordir
cmsRun run_DIGIPremix_noCopy.py $(($1+0)) $file_name $condordir
if [ "$save_allsteps" = true ] 
then
  cp ${file_name}_DIGIPremix_${1}.root $output_dir/$file_collection/.
fi

cmsrel CMSSW_10_2_16_UL
cd CMSSW_10_2_16_UL/src
eval `scramv1 runtime -sh`


echo 4_run_HLT
cp $production_dir/Hadronizers/run_HLT_noCopy.py $condordir
cd $condordir
cmsRun run_HLT_noCopy.py $(($1+0)) $file_name $condordir
if [ "$save_allsteps" = true ] 
then
  cp ${file_name}_HLT_${1}.root $output_dir/$file_collection/.
fi

cd CMSSW_10_6_17_patch1/src
eval `scramv1 runtime -sh`


echo 5_run_RECO
cp $production_dir/Hadronizers/run_RECO_noCopy.py $condordir
cd $condordir
cmsRun run_RECO_noCopy.py $(($1+0)) $file_name $condordir
if [ "$save_allsteps" = true ] 
then
  cp ${file_name}_RECO_${1}.root $output_dir/$file_collection/.
fi

cmsrel CMSSW_10_6_20
cd CMSSW_10_6_20/src
eval `scramv1 runtime -sh`

echo 6_run_MiniAOD
cp $production_dir/Hadronizers/run_MiniAOD_noCopy.py $condordir
mkdir -p $condordir/output/UL_MiniAOD/$file_collection
cd $condordir
cmsRun run_MiniAOD_noCopy.py $(($1+0)) $file_collection $file_name $condordir
cp output/UL_MiniAOD/$file_collection/${file_name}_MiniAOD_$(($1+0)).root $output_dir/UL_MiniAOD/$file_collection/.

# cmsrel CMSSW_10_6_29
cd $production_dir/CMSSW_10_6_29/src
eval `scramv1 runtime -sh`

echo 7_run_NanoAOD
cp $production_dir/Hadronizers/run_NanoAOD.py $production_dir/Hadronizers/ttalps_cff.py $condordir
mkdir -p $condordir/output/UL_NanoAOD/$file_collection
cd $condordir
cmsRun run_NanoAOD.py $(($1+0)) $file_collection $file_name $condordir
cp output/UL_NanoAOD/$file_collection/${file_name}_NanoAOD_$(($1+0)).root $output_dir/UL_NanoAOD/$file_collection/.


if [ "$condordir" == $production_dir/tmp ]
then
  rm -r $condordir
fi
