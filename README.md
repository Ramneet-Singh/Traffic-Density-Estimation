# Traffic-Density-Estimation
Traffic density estimation using OpenCV Functions. Please check out the [report](Traffic_Density_Estimation.pdf] for details and analysis.
### Camera angle correction and frame cropping

#### Instructions to run

1. Navigate to the project directory.
2. Type `make` or `make all` in the terminal.
3. Run the executable named *final*  by typing `./final <video_file_path>` where `<video_file_path>` is the path to the video file you want to give as input. If the command line arguments are not input properly, you will get an error message in the console.
4. Comma separated values of the timestamp (in seconds), queue density, and dynamic density will be output to the terminal, with each row being separated by a newline (and the last row ending with a newline character).

#### Common Error
If you see an error which says  `No package 'opencv' found`, it's likely because it is present under the name opencv4 instead of opencv(which is there in the standard installation) in your system. Please correspondingly change the `LIBS` variable in the Makefile and write opencv4 instead of opencv.

Done with [Rakshita Choudhary](https://github.com/rakshitachoudhary) as a project for course COP290 (Design Practices in Computer Science) at IIT Delhi. 
