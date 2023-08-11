universe = vanilla
initialdir = .

error = error/run_$(ClusterId)-$(Process).error
log = log/run_$(ClusterId)-$(Process).log
output = output/run_$(ClusterId)-$(Process).out
executable = submit_fullsim_noCopy_ttalp.sh
#           part         mass   events
arguments = $(Process)   1      10000
Notification=never
transfer_output_files = ""
+JobFlavour           = "tomorrow"

queue 100