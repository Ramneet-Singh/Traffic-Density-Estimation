# Traffic-Density-Estimation
Traffic density estimation using OpenCV Functions.

#### Instructions to run
*Note: Project directory in all subsequent points refers to the code folder of the overall assignment package. Similarly, analysis directory refers to the analysis folder of the overall assignment package.*

1. Navigate to the project directory.
2. Type `make task3` in the terminal to compile the files for subtask 3 and generate an executable. 
3. Run the executable named *final3* by typing `./final3 <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>` where 
`<video_file_path>` is the path to the video file you want to give as input, `<method_number>` is an integer denoting the method you would like to run (method numbers here are the same as the ones mentioned in assignment specifications, additionally number 5 is used to execute sparse optical density function), `<method_parameters>` are the parameters you would want to pass to your method (a single integer in case of methods 1, 3 and 4, two space separated integers for method 2 and no parameters required for method 5), Additional provision to perform average time analysis has been provided, you can perform the same by passing an additional timing flag `-t`. If the command line arguments are not input properly, you will get an appropriately worded error message in the console.
5. Comma separated values of the timestamp (in seconds) and queue density will be written to an output file (in a folder corresponding to the method number), with each row being separated by a newline (and the last row ending with a newline character). In case you are running method 4 (temporal parallelisation), you will need to perform an additional step of running the `combineParallelQueueFiles.sh` shell script from the project directory. The script is present in `<analysis_directory>/postProcessingScripts`.
6. If you would like to plot the graphs, please navigate to the "analysis" directory and enter gnuplot terminal. Type `call <script_name> <method_number> <filenames_if_needed>` where `<script_name>` is the name of the gnuplot script enclosed in single quotes (decide the script according to the type of graph required), `<method_number>` is the method corresponding to the script, and if the script requires filenames (only the optical flow script requires these), `<filenames_if_needed>` are the paths of the sparse and dense optical flow output files enclosed in double quotes.

#### Common Error
If you see an error which says  `No package 'opencv' found`, it's likely because it is present under the name opencv4 instead of opencv(which is there in the standard installation) in your system. Please correspondingly change the `LIBS` variable in the Makefile and write opencv4 instead of opencv.

Done with [Rakshita Choudhary](https://github.com/rakshitachoudhary) as a project for course COP290 (Design Practices in Computer Science) at IIT Delhi.
