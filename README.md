# RayTracer

![](https://user-images.githubusercontent.com/18747212/207518181-fd38229f-e0aa-4832-8193-7fe3792d0057.png)

This was the final assigment for Fall 2022 CSC 305: Introduction to Computer Graphics. This is a simple raytracer program that renders a PPM image given an input txt file describing the scene.

## Supported Features

Only spheres and point lights objects are supported.

Lighting features:
- Ambient
- Diffuse
- Specular
- Reflections

## Compilation

This project is provided as a VS2019 project setup to build a x64 executable. A makefile to compile on Linux is also provided. Both methods place the executable in [`./x64/Release/`](https://github.com/OwenBC/RayTracer/tree/master/x64/Release).

## Usage
 
Windows:
`RayTracer.exe testCase1.txt`

Linux:
`./RayTracer testCase1.txt`

### Input File Syntax

A .txt with the following syntax should be input.

*The eye position is fixed at (0, 0) looking down the negative z axis.

```
// Near plane. Float value representing distance down the view plane in world coordinates.
NEAR <n>

// Left, right, top, and bottom view planes. Float values in world coordinates.
LEFT <l>

RIGHT <r>

BOTTOM <b>

TOP <t>

// Image resolution num. of columns x num. of rows (positive integer values)
RES <x> <y>

// Any number of spheres, with name, position, scaling, color, ambient/diffuse/specular/reflective intensities, and specular shininess. 
// - Position is float values in world coordinates.
// - Scaling is positive float values representing the scaling factor on a uniform sphere of radius 1.
// - Color in rgb as float values in the range [0-1].
// - Intensity constants are float values in the range [0-1].
// - Specular shininess is a positive integer value.
SPHERE <name> <pos x> <pos y> <pos z> <scl x> <scl y> <scl z> <r> <g> <b> <Ka> <Kd> <Ks> <Kr> <n>

// Any number of point lights, with name, position and color. 
// - Position is float values in world coordinates.
// - Color in rgb as float values in the range [0-1].
LIGHT <name> <pos x> <pos y> <pos z> <Ir> <Ig> <Ib>

// Background color in rgb as float values in the range [0-1].
BACK <r> <g > <b>

// Ambient lighting intensity in rgb as float values in the range [0-1].
AMBIENT <Ir> <Ig> <Ib>

// Output filename. Must end in '.ppm'.
OUTPUT <name>
```

The input files provided with this assignment can be found in [`./x64/Release/`](https://github.com/OwenBC/RayTracer/tree/master/x64/Release).

### Other Notes
- Increasing light count and to a much lesser degree, sphere count, will slow render time.
- The depth of reflection is set to 3 iterations (including the intial ray from the eye). This can be changed by the value of `DEPTH` on line 12 (currently) of [`RayTracer.cpp`](https://github.com/OwenBC/RayTracer/blob/5f3459310ad8534447525624ee1a6d68056abd7c/RayTracer/RayTracer.cpp#L12). 
