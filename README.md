# Traffic-Density-Estimation
Traffic density estimation using OpenCV Functions.
### Camera angle correction and frame cropping

#### Instructions to run

1. Navigate to the project directory.
2. Type `make task3` in the terminal to compile the files for task3. 
3. Run the executable named *final3* by typing `./final3 <video_file_path> <method_number> <method_parameters> <optional_timing_avg_analysis_flag>` where 
`<video_file_path>` is the path to the video file you want to give as input, `<method_number>` is an integer denoting the method you would like to run (method numbers here are the same as the ones mentioned in assignment specifications, additionally number 5 is used to execute sparse optical density function), `<method_parameters>` are the parameters you would want to pass to your method (a single integer in case of methods 1, 3 and 4, two space separated integers for method 2 and no parameters required for method 5), Additional provision to perform average time analysis has been provided, you can perform the same by passing an additional argument `-t`. If the command line arguments are not input properly, you will get an error message in the console.
5. Comma separated values of the timestamp (in seconds) and queue density will be written to an output file (in a folder corresponding to the method number), with each row being separated by a newline (and the last row ending with a newline character).
6. If you would like to plot the graphs, please navigate to the "Analysis" directory and enter gnuplot terminal. Type `call <script_name> <method_number>` where `<script_name>` is the name of the gnuplot script (decide the script according to the type of graph required), here `<method_number>` can only be 1,2,3,4.

#### Common Error
If you see an error which says  `No package 'opencv' found`, it's likely because it is present under the name opencv4 instead of opencv(which is there in the standard installation) in your system. Please correspondingly change the `LIBS` variable in the Makefile and write opencv4 instead of opencv.

Done with [Rakshita Choudhary](https://github.com/rakshitachoudhary) as a project for course COP290 (Design Practices in Computer Science) at IIT Delhi.
