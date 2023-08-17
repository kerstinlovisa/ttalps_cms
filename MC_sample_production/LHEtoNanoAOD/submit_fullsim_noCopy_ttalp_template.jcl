universe = vanilla
initialdir = .

error = error/run_$(ClusterId)-$(Process).error
log = log/run_$(ClusterId)-$(Process).log
output = output/run_$(ClusterId)-$(Process).out
executable = submit_fullsim_noCopy_ttalp.sh
arguments = $(Process)   PART   MASS   NEVENTS   EVENTSPERJOB
Notification=never
transfer_output_files = ""
+JobFlavour           = FLAVOUR

queue QUEUE
