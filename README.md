# CUDA Encoder

Well, technically this is more NVENC than CUDA, but that's semantics.  
This program uses CUDA/NVENC for de/encoding input files to H.264 output. Currently only tested input is H.264 file (plain H.264, mp4 and mkv containers seem to work).
This program is released under MIT license. Note that there are NVIDIA source code files used for compiling, which uses different license.  

The program is built using QT 5.4 and Visual Studio 2013. It is saved in Win32\Release folder. To run this program, you need to have NVIDIA GPU with Kepler/Maxwell chip and Visual C++ Redistributable 2013 32-bit.  

For compilation, you need additional programs/utilities:
* nVidia drivers no older than R337 - The program is tested on 350.12
* Visual Studio 2013 - vcxproj file is present
* QT 5.4.X
* QT Visual Studio Add-in
* NVENC SDK - the 5.0.1 release files are included in source files, shouldn't be needed
* CUDA Toolkit
