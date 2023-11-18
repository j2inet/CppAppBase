# CppAppBase
A sample of a foundation class that I'm using in some other native/C++ Windows projects

This code should largely be viewed as a scratchpad project. There are some contributions from real projects within here. But most of this code is for demonstration purposes only. 


# Projects

There are three projects within this project. Each of which has its own solution file. 

*	**CppAppBase** - This is the foundation class that I'm using in other projects.
*	**D2D1AppWindow** - This is a sample project that uses the CppAppBase class to create a simple window that uses Direct2D for rendering.
*	**D3DAppWindow** - This is a sample project that uses the CppAppBase class to create a simple window that uses Direct3D for rendering.

There are some files that are shared across the projects. Some of these files are loaded in the `common` folder. Over time, I'll probably move 
other shared files into this folder. The original repository for this code is https://github.com/j2inet/CppAppBase. Note that there are a few
branches in that repository that mark checkpoints for the D3D11 project. 


* [00-draw-triangle](https://github.com/j2inet/CppAppBase/tree/checkpoint/00-draw-triangle) - Draws a triangle using Direct3D 11
* [01-move-triangle](https://github.com/j2inet/CppAppBase/tree/checkpoint/01-move-triangle) - Moves the triangle using a timer and constant buffers
 