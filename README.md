# ObjectGL
Object GL is a object-oriented wraper around OpenGL objects, like vertex buffers, index buffers, shader storage buffers, and so on. It is designed to work with new OpenGL-features, so a context version of 3.0 or greater is recomended, but not strictly necessery. 
# Downloading
To download the library, move into a directory of your choise and run `git clone https://github.com/DM8AT/ObjectGL.git` to get the newest stable version of the library. After that, see the (the chapter about compilation)[#Compiling]. 
# Compiling
## Dependencys
To compile the library, a few other librarys need to be installed. They are:
 - OpenGL: Of course you need OpenGL for any library that works with OpenGL. 
    - Installation on Linux: On Linux, run `sudo apt install libgl-dev` to install it
 - GLEW: GLEW is a library to map OpenGL commands so the call names are independend of OpenGL version, driver version and operating system. 
    - Installation on Linux: On Linux, run `sudo apt install libglew-dev`
 - SDL2: SDL2 stands for "Simple Direct media Layer" and the library is the second major version. In this library, SDL2 is nececery to abstract window and instance creation from the operating system and window manager. 
    - Installation on Linux: On Linux, run `sudo apt install libsdl2-dev`
 - SDL2 image: SDL2 image is a sub-library of SDL2. It is used to load images and format them for OpenGL. 
    - Installation on Linux: On Linux, run `sudo apt install libsdl2-image-dev`
## Simple installation
### Linux
On Linux, to install all the librarys just run 
`sudo apt install libgl-dev libglew-dev libsdl2-dev libsdl2-image-dev` in the terminal. 
## Compiling under Linux
### Compiling the library
To compile the library, go into the main directory the library was downloaded to (the directory should contain this README and an makefile). Then, run `make libObjectGL -j` and the file "libObjectGL" should appear in the current directory. This is the static compiled library. Just link it to any project you want, and it should work. If not, write an issue detailing what did not work. 
### Compiling the example
To compile the example, run `make -j` in this directory. After that, the "bin" folder should fill with object files, the static compiled library and an executable named "main". This is the example program. To run it, keep staying in this directory and type `./bin/main` into a console. You should see a window named "Hello World!" open. 

# Ongoing developement
## What is to come
Here is a list of all features i am planing to add to this library:
- a class to handle framebuffers
- functions to add geometry, tesselation and tesselation controll shaders to the shader class

## Help the developement
If you are missing a feature you'd love to have a wrapper for, create an issue using the "Feature Request" template and describe the feature according to the questions in the template. 

## Changelog
Here is a list of all code related chages that happend after the last major release: