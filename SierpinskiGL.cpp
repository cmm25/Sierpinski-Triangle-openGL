#include<GL/glut.h>
#include<iostream>
#include<math.h>

GLfloat vertices[3][2] = {{0.0, 0.0}, {25.0, 50.0}, {50.0, 0.0}};\
int n;

// function to draw the triangle

void draw_triangle(GLfloat* a, GLfloat* b, GLfloat* c)
{
	glBegin(GL_TRIANGLES);
	glVertex2fv(a);
	glVertex2fv(b);
	glVertex2fv(c);
	glEnd();
}

// function to divide the triangle into smaller triangles

