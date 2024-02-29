
# Clipping the First Triangle of the Sierpinski Fractal using the Greiner-Hormann Algorithm (C++ with OpenGL)

## Introduction:

This repository contains C++ code that demonstrates the clipping of the first triangle from the Sierpinski fractal using the Greiner-Hormann algorithm in backward mode. It utilizes the OpenGL library ([GLUT](https://freeglut.sourceforge.net/)) for visual display.

## Code Breakdown:

### 1. Libraries and Data Structures:

- `#include <GL/glut.h>`: Includes the OpenGL Utility Toolkit library for graphics functions.
- `#include <iostream>`: Includes standard input/output facilities.
- `#include <vector>`: Includes the vector container class for dynamic arrays.
- `#include <algorithm>`: Includes algorithms for various operations on data structures.
- `struct Point { GLfloat x, y; }`: Defines a Point structure to represent points (vertices) in 2D space with floating-point coordinates (x and y).
- `struct Edge { Point start, end; }`: Defines an Edge structure to represent line segments with a starting and ending point (start and end).

### 2. Intersection Calculation (`calculate_intersection` function):

- This function calculates the intersection point of two line segments (p1-p2 and p3-p4).
- It uses the line-line intersection formula to compute the intersection coordinates (`intersection_point.x` and `intersection_point.y`).
- It checks for degenerate cases where the lines are parallel or coincident, returning a special value (-1.0) in such situations.

### 3. Intersection Classification (`classify_intersection` function):

- This function classifies the intersection point (`intersection_point`) between a line segment (edge_start-edge_end) and a point (clip_vertex) as entering (-1.0 or -2.0), exiting (-3.0), or on the edge (0.0).
- It uses the cross product of the line segment direction vector and the vector from the starting point to the clip vertex to determine the relative position.

### 4. Clipping Function (`clip_polygon` function):

- This function performs the backward clipping of a subject polygon (`subject_polygon`) against a clipping polygon (`clipping_polygon`) using the Greiner-Hormann algorithm.
- It iterates through each edge of the clipping polygon in forward order.
- For each edge of the subject polygon:
  - It calculates the intersection point with the current clipping edge (if it exists).
  - It classifies the intersection point.
  - Based on the classification:
    - Entering: Adds the intersection point and the next vertex of the subject polygon (if not already added) to the output polygon.
    - Exiting: Does nothing (since the remaining part of the subject polygon lies outside the clipping region).
- The function returns the final clipped polygon.

### 5. Drawing Functions:

- `draw_clipped_polygon`: Draws the clipped polygon using OpenGL functions, setting the color to red (`glColor3f(1.0, 0.0, 0.0)`).
- `draw_sierpinski_triangle`: Draws the first triangle of the Sierpinski fractal directly (without clipping) by:
  - Clearing the color buffer (`glClear`).
  - Defining the subject polygon as the first triangle.
  - Calling `draw_clipped_polygon` to draw it.
  - Flushing the display (`glFlush`).

### 6. Initialization and Main Function:

- `initialize`: Sets the background color, projection matrix, and viewing area using OpenGL functions.
- `main`:
  - Initializes GLUT.
  - Sets display mode (single buffer, RGB color).
  - Creates a window with the title "Clipped Sierpinski Triangle".
  - Calls `initialize` for OpenGL setup.
  - Sets the display callback function to `draw_sierpinski_triangle`.
  - Enters the main GLUT event loop (`glutMainLoop`).

## How to Run:

1. Download or clone the repository.
2. Install the necessary libraries (GLUT and a C++ compiler).
3. Compile the code using your preferred C++ compiler (e.g., `g++ clipper.cpp -lglut -o clipper`).
4. Run the executable (e.g., `./clipper`).
5. The program should display the first triangle of the Sierpinski fractal in a red color.

