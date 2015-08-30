#!/bin/bash  

##### Shell & environment information #####
#$ -S /bin/bash   

##### Job ID and notification #####
#$ -N py_higgs
# ( : a name to the jobs. The name will appear in the output of qstat )

#export CURRENT='/data/wscho/simulation/OptiMass-Hhh/Hhh_event/model-repo/models/Pythia8/2hdm/gg_H_hh'
export output_event_dir='/data/repo-om/Hhh_event/2hdm_event'
export output_log_dir='/data/repo-om/Hhh_event/2hdm_event/log'

##### Output setting #####
#$ -o /dev/null
# ( => Redirects the standard output to the named file. )
##$ -e file_name
# ( => Redirects the standard error to the named location)
#$ -cwd        
# ( => Execute jobs in same directory where qsub was run, otherwise output+error report files will apper in the home directory of each user )
#$ -j y   
# ( => Merges the normal output of the file and any error messages into one file, typically with the name <Job-Name>.o<Job-ID> )

##### System resource setting #####
##$ -pe pe_name slot_range
##$ -l resource_list (h_rt=hard time in hh:mm:ss format, h_vmem=mem;hard virtual memory limit, h_stack=mem;mem stack size limit, chip=, os=, lscratch, test=)

##### Arrayjob running #####
#$ -t 1-240
# => Arrayjob index : Tell the SGE that this is an arrayjob with "tasks" to be numbered 1 to 10. When a single command in the arrayjob is sent to a compute node, its task number is stored in the variable SGE_TASK_ID, so we can use the value of that variable to identify the job and get the results, especially when we want to submit multiple serial jobs of the same executables, but just with different input arguments:
#
# Job start
#echo Hello Users! from the job ID.arrayjob ID = $JOB_ID.$SGE_TASK_ID >> output_$JOB_ID.$SGE_TASK_ID
echo Start the job ID.arrayjob ID = $JOB_ID.$SGE_TASK_ID
# Job start date
date

# Main executable
./gg_H_hh_cluster $output_event_dir/2hdm_$SGE_TASK_ID.lhe 10000 $SGE_TASK_ID $SGE_TASK_LAST > $output_log_dir/2hdm_$SGE_TASK_ID.log

# Job end
echo End the jobID.arrayjob ID = $JOB_ID.$SGE_TASK_ID

# Job end date
date
#############################

