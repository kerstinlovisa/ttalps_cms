#!/bin/bash

# Script to create multiple submit .jcl files with different inputs on mass and part
# And submit these jobs to condor
# Using submit_fullsim_noCopy_ttalp_base.jcl as a reference submit file

masses=(0.35)

flavour='"testmatch"'

nevents=10000
eventsperjob=1000
queue=$((${nevents} / ${eventsperjob}))

submitfile_base=submit_fullsim_noCopy_ttalp_base.jcl

mkdir tmp_submits

for mass in $masses
do
  masstring=`echo "$mass" | tr . p`
  for part in {0..100}
  do
    tmp_submit=tmp_submits/submit_fullsim_noCopy_ttalp_mAlp-${masstring}GeV_part-${part}.jcl
    sed "s/MASS/$mass/g" $submitfile_base > $tmp_submit
    sed -i "s/PART/$part/g" $tmp_submit
    sed -i "s/NEVENTS/$nevents/g" $tmp_submit
    sed -i "s/EVENTSPERJOB/$eventsperjob/g" $tmp_submit
    sed -i "s/FLAVOUR/$flavour/g" $tmp_submit
    sed -i "s/QUEUE/$queue/g" $tmp_submit

    condor_submit $tmp_submit
  done
done