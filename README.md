# Traffic-Density-Estimation
Traffic density estimation using OpenCV Functions.
### Camera angle correction and frame cropping

#### Instructions to run

1. Navigate to the project directory.
2. Type `make` or `make all` in the terminal.
3. Run the executable named *final*  by typing `./final <image_file_path>` where `<image_file_path>` is the path to the image file you want to give as input. If the command line arguments are not input properly, you will get an error message in the console.
4. A window titled "Original Frame" will be opened. Please select the 4 corners of the area that you want to project in the following order: **Top Left, Top Right, Bottom Right, Bottom Left**. After selecting the 4 points, press any key. 
5. A window titled "Projected Frame" will open and show the transformed frame (without cropping). On pressing any key again, a window titled "Cropped Frame" will be opened which will give you the final result of projection as well as cropping. 
6. Press any key to end the execution. The projected and cropped images will be saved in the same folder as the input image with names *image_name_projected.ext* and *image_name_cropped.ext* where *image_name* and  *.ext* are the name and extension of the input image respectively.

#### Common Error
If you see an error which says  `No package 'opencv' found`, it's likely because it is present under the name opencv4 instead of opencv(which is there in the standard installation) in your system. Please correspondingly change the `LIBS` variable in the Makefile and write opencv4 instead of opencv.

Done with [Rakshita Choudhary](https://github.com/rakshitachoudhary) as a project for course COP290 (Design Practices in Computer Science) at IIT Delhi. 
