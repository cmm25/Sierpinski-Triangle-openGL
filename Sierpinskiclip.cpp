#include <GL/glut.h>
#include <iostream>
#include <vector>
#include<algorithm>
using namespace std;

// Define a Point structure
struct Point {
    GLfloat x, y;
    Point(GLfloat x_, GLfloat y_) : x(x_), y(y_) {}
};

// Function prototypes
Point calculate_intersection(Point p1, Point p2, Point p3, Point p4);
void classify_intersection(Point& intersection, const Point& edge_start, const Point& edge_end, const Point& clip_vertex);
vector<Point> clip_polygon(const vector<Point>& subject_polygon, const vector<Point>& clipping_polygon);

// Global variables
vector<Point> vertices = { {0.0, 0.0}, {25.0, 50.0}, {50.0, 0.0} }; // Sierpinski Triangle vertices
vector<Point> colors = { Point(1.0, 0.0), Point(0.0, 1.0), Point(0.0, 0.0) }; // RGB colors for each vertex
int n;

// function to draw the triangle with interpolated colors
void draw_triangle(const Point& a, const Point& b, const Point& c, const Point& colorA, const Point& colorB, const Point& colorC)
{
    glBegin(GL_TRIANGLES);
    glColor3f(colorA.x, colorA.y, 0.0f); 
    glColor3f(colorB.x, colorB.y, 0.0f);
    glVertex2f(b.x, b.y);
    glColor3f(colorC.x, colorC.y, 0.0f);
    glVertex2f(c.x, c.y);
    glEnd();
}

// Function to divide the triangle into smaller triangles
void divide_triangle(const Point& a, const Point& b, const Point& c, const Point& colorA, const Point& colorB, const Point& colorC, int m)
{
    Point v[3] = { {(a.x + b.x) / 2, (a.y + b.y) / 2}, {(a.x + c.x) / 2, (a.y + c.y) / 2}, {(b.x + c.x) / 2, (b.y + c.y) / 2} };

    // Calculate colors for the new vertices by interpolating between the original colors
    Point colorsV[3] = { {(colorA.x + colorB.x) / 2, (colorA.y + colorB.y) / 2}, {(colorA.x + colorC.x) / 2, (colorA.y + colorC.y) / 2}, {(colorB.x + colorC.x) / 2, (colorB.y + colorC.y) / 2} };

    if (m > 0)
    {
        divide_triangle(a, v[0], v[1], colorA, colorsV[0], colorsV[1], m - 1);
        divide_triangle(b, v[2], v[0], colorB, colorsV[2], colorsV[0], m - 1);
        divide_triangle(c, v[1], v[2], colorC, colorsV[1], colorsV[2], m - 1);
    }
    else
        draw_triangle(a, b, c, colorA, colorB, colorC);
}



// Function to clip the Sierpinski Triangle against another polygon
vector<Point> clip_sierpinski_triangle(const vector<Point>& subject_polygon, const vector<Point>& clipping_polygon) {
    vector<Point> result = subject_polygon;

    for (size_t i = 0; i < clipping_polygon.size(); i++) {
        size_t next = (i + 1) % clipping_polygon.size();
        Point clip_start = clipping_polygon[i];
        Point clip_end = clipping_polygon[next];

        vector<Point> output_polygon;

        for (size_t j = 0; j < result.size(); j++) {
            size_t next_subject = (j + 1) % result.size();
            Point subject_start = result[j];
            Point subject_end = result[next_subject];

            Point intersection_point = calculate_intersection(subject_start, subject_end, clip_start, clip_end);
            classify_intersection(intersection_point, subject_start, subject_end, clip_end);

            if (intersection_point.x == -1.0) {
                output_polygon.push_back(intersection_point);
                if (subject_end.x != -1.0)
                    output_polygon.push_back(subject_end);
            }
            else if (intersection_point.x == -2.0) {
                output_polygon.push_back(intersection_point);
            }
        }
        result = output_polygon;
    }

    return result;
}

// Function to calculate the intersection point of two line segments
Point calculate_intersection(Point p1, Point p2, Point p3, Point p4) {
    Point intersection_point(-1.0, -1.0);

    GLfloat x1 = p1.x, y1 = p1.y;
    GLfloat x2 = p2.x, y2 = p2.y;
    GLfloat x3 = p3.x, y3 = p3.y;
    GLfloat x4 = p4.x, y4 = p4.y;

    GLfloat denominator = ((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4));

    if (denominator == 0.0) {
        intersection_point.x = -1.0;
        intersection_point.y = -1.0;
        return intersection_point;
    }

    intersection_point.x = (((x1 * y2 - y1 * x2) * (x3 - x4)) - ((x1 - x2) * (x3 * y4 - y3 * x4))) / denominator;
    intersection_point.y = (((x1 * y2 - y1 * x2) * (y3 - y4)) - ((y1 - y2) * (x3 * y4 - y3 * x4))) / denominator;

    return intersection_point;
}

// Function to classify the intersection point (entering or exiting)
void classify_intersection(Point& intersection, const Point& edge_start, const Point& edge_end, const Point& clip_vertex) {
    GLfloat dx = edge_end.x - edge_start.x;
    GLfloat dy = edge_end.y - edge_start.y;
    GLfloat dxi = clip_vertex.x - edge_start.x;
    GLfloat dyi = clip_vertex.y - edge_start.y;
    GLfloat cross_product = dx * dyi - dy * dxi;

    if (dy == 0) {
        if (dx > 0 && clip_vertex.y < edge_start.y)
            intersection.x = -1.0; 
        else if (dx < 0 && clip_vertex.y > edge_start.y)
            intersection.x = -2.0; 
        else
            intersection.x = -3.0; 
    }
    else if (cross_product > 0)
        intersection.x = -1.0; 
    else
        intersection.x = -3.0; 
}

// Display function
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the original Sierpinski Triangle
    GLfloat colors[3][3] = { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} };

    Point colorA(colors[0][0], colors[0][1]);
    Point colorB(colors[1][0], colors[1][1]);
    Point colorC(colors[2][0], colors[2][1]);

    // Define the clipping polygon vertices (a larger triangle)
    vector<Point> clipping_polygon = { {0.0, 0.0}, {100.0, 100.0}, {200.0, 0.0} };

    // Clip the Sierpinski Triangle against the clipping polygon
    vector<Point> clipped_polygon = clip_sierpinski_triangle(vertices, clipping_polygon);

    // Draw the clipped polygon
    glColor3f(1.0, 0.0, 0.0); 
    glBegin(GL_POLYGON);
    for (const auto& vertex : clipped_polygon) {
        glVertex2f(vertex.x, vertex.y);
    }
    glEnd();

    glFlush();
}

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 200.0, 0.0, 200.0); // Set the viewing area
}

int main(int argc, char** argv)
{
    cout << "Enter the number of divisions: ";
    cin >> n;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Clipped Sierpinski Triangle");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
