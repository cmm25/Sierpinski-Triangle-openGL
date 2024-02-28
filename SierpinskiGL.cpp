#include <GL/glut.h>
#include <iostream>
#include <math.h>

GLfloat vertices[3][2] = { {0.0, 0.0}, {25.0, 50.0}, {50.0, 0.0} };
GLfloat colors[3][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0} }; // RGB colors for each vertex
int n;

// function to draw the triangle with interpolated colors
void draw_triangle(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* colorA, GLfloat* colorB, GLfloat* colorC)
{
    glBegin(GL_TRIANGLES);
    glColor3fv(colorA);
    glVertex2fv(a);
    glColor3fv(colorB);
    glVertex2fv(b);
    glColor3fv(colorC);
    glVertex2fv(c);
    glEnd();
}

// function to divide the triangle into smaller triangles
void divide_triangle(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* colorA, GLfloat* colorB, GLfloat* colorC, int m)
{
    GLfloat v[3][2];
    GLfloat colorsV[3][3];
    int j;
    if (m > 0)
    {
        for (j = 0; j < 2; j++)
        {
            v[0][j] = (a[j] + b[j]) / 2;
            v[1][j] = (a[j] + c[j]) / 2;
            v[2][j] = (b[j] + c[j]) / 2;
        }

        // Calculate colors for the new vertices by interpolating between the original colors
        for (j = 0; j < 3; j++)
        {
            colorsV[0][j] = (colorA[j] + colorB[j]) / 2;
            colorsV[1][j] = (colorA[j] + colorC[j]) / 2;
            colorsV[2][j] = (colorB[j] + colorC[j]) / 2;
        }

        divide_triangle(a, v[0], v[1], colorA, colorsV[0], colorsV[1], m - 1);
        divide_triangle(c, v[1], v[2], colorC, colorsV[1], colorsV[2], m - 1);
        divide_triangle(b, v[2], v[0], colorB, colorsV[2], colorsV[0], m - 1);
    }
    else
        draw_triangle(a, b, c, colorA, colorB, colorC);
}

// Display function
void draw_sierpinski_triangle()
{
    glClear(GL_COLOR_BUFFER_BIT);
    divide_triangle(vertices[0], vertices[1], vertices[2], colors[0], colors[1], colors[2], n);
    glFlush();
}

// initialization function
void initialize()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 50.0, 0.0, 50.0);
}

// main function
int main(int argc, char** argv)
{
    std::cout << "Enter the number of divisions: ";
    std::cin >> n;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Sierpinski Gasket");
    initialize();
    glutDisplayFunc(draw_sierpinski_triangle);
    glutMainLoop();
    return 0;
}
