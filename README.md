# OpenGL Skybox Scene
This project creates a scene within a skybox using OpenGL

## General Info
When the user starts the project, they will be able to perform the below operations through the OpenGL window:
 * t - toggle the use of the bloom shader effect
 * p - toggle the display of panels showcasing the clipping of the boat in the water
 * b - toggle the display of panels showcaseing the bloom implementation
 * Left Mouse Button - move the camera

## Technologies
This project was run using the following:
* Visual Studio 2022
* FreeGlut 3.0.0 MSVC
* GLee

## Setup
After cloning or forking the repository, you can run the project in the below manner:
1. Download freeglut 3.0.0 for MSVC
2. Extract the contents of the zip file and place it in the C drive
   - This should result in `C:\freeglut` existing
3. Go to `C:\freeglut\bin'
4. Move `freeglut.dll` to `C:\Windows\System32` or `C:\Windows\SysWOW64` (depending on your operating system)
5. Download `GLee.h` and `GLee.lib` from the [Git repository on Sourceforge](https://sourceforge.net/p/glee/glee/ci/master/tree/)
6. Place `GLee.h` within `C:\freeglut\include\GL`
7. Place `GLee.lib` within `C:\freeglut\lib`
8. Double click file `Cube.sln` to open Visual Studio
9. Go to the Solution Explorer window and right-click **Cube** (not **Solution 'Cube'**)
10. Select **Properties** and left-click **C/C++**
11. At **Additional Include Directories**, add `C:\freeglut\include`
12. Left-click **Linker**
13. At **Additional Library Directories** add `C:\freeglut\lib`

## Execution
If seup is completed successfully, simply build the project by either clicking the green **Start** button
* You can also press **F5** or **Ctrl + F5**
