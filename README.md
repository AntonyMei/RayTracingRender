# Ray Tracing Render
___
## Usage on Windows
+ step 1: compile this project using cmake
+ step 2: run "Project.exe 8", here 8 means using 8 threads (after running we will get 8 .partial files)
+ step 3: run "packager.exe 8", here 8 means combining 8 partial files (after running we will get a .ppm image,
which can be opened using OpenSeeIt)
+ step 4: run python convert.py, which converts .ppm into .jpg (note that opencv for python is required to run this)

## Usage on Linux
run "bash linux_run.sh", which compiles the renderer, uses 80 processes to run it and process result into a .ppm and
a .jpg file. (Note that for default Sponza Crytek scene, about 150 GB memory is required. This requirement is
proportional to the number of processes.)

**NOTE: For detailed usage and code explanation, see /report/report.pdf.**
___
## Results
![Hollow Glass Ball]()
