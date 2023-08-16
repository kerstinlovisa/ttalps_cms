universe = vanilla
initialdir = .

error = error/run_$(ClusterId)-$(Process).error
log = log/run_$(ClusterId)-$(Process).log
output = output/run_$(ClusterId)-$(Process).out
executable = submit_fullsim_noCopy_ttalp.sh
#           process      part   mass   events    GENevents/job
arguments = $(Process)   0      1      100       10
Notification=never
transfer_output_files = ""
+JobFlavour           = "workday"

queue 100
