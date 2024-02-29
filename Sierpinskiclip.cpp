#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Point {
    GLfloat x, y;
};

struct Edge {
    Point start, end;
};
//functio to calculate the intersection between 2 lines
Point calculate_intersection(Point p1, Point p2, Point p3, Point p4) {
    Point intersection_point;
    GLfloat x1 = p1.x, y1 = p1.y;
    GLfloat x2 = p2.x, y2 = p2.y;
    GLfloat x3 = p3.x, y3 = p3.y;
    GLfloat x4 = p4.x, y4 = p4.y;

    // find denominator
    GLfloat denominator = ((x1 - x2) * (y2 - y3)) - ((y1 - y2) * (x3 - x4));

    // check if lines are parallel
    if (denominator == 0.0) {
        intersection_point.x = -1.0;
        intersection_point.y = -1.0;
        return intersection_point;
    }
    //find intersection point co-ordinates

    intersection_point.x = ((((x1 * y2) - (y1 * x2)) * (x3 - x4)) - ((x1 - x2) * ((x3 * y4) - (y3 * x4)))) / denominator;
    intersection_point.y = ((((x1 * y2) - (y1 * x2)) * (x3 - x4)) - ((y1 - y2) * ((x3 * y4) - (y3 * x4)))) / denominator;
    return intersection_point;
 }
// check if point is inside a convex polygon
bool is_inside(Point p1, Point p2, Point p3) {
    return(((p2.x - p1.x) * (p3.y - p1.y)) - ((p2.y - p1.y) * (p3.x - p1.x))) <= 0.0;
 }


vector<Point> clip_polygon(vector<Point> subject_polygon, vector<Point> clipping_polygon) {
    vector<Point> result;

    // Greiner-Hormann algorithm
    for (int i = 0; i < clipping_polygon.size(); i++) {
        int next = (i + 1) % clipping_polygon.size();
        Point clip_start = clipping_polygon[i];
        Point clip_end = clipping_polygon[next];

        // storage vector for clipped vertices
        vector<Point> output_polygon;
        // iteration through each point of the target triangle
        for (int j = 0; j < subject_polygon.size(); j++) {
            int next = (j + 1) % clipping_polygon.size();
            Point triangle_start = subject_polygon[j];
            Point triangle_end = subject_polygon[next];

        // intersection calculation in realation to the traingele to the clipping rectangle
            Point intersection_point = calculate_intersection(triangle_start, triangle_end, clip_start, clip_end);
            // Determine if the intersection_point if its inside clipping region
            if(is_inside(clip_start,clip_end,intersection_point)){
                output_polygon.push_back(intersection_point);
            }
            // Determine edge_points if it is inside clipping region
            if (is_inside(clip_start, clip_end, triangle_start)) {
                output_polygon.push_back(triangle_start);
            }
            if (is_inside(clip_start, clip_end, triangle_end)) {
                output_polygon.push_back(triangle_end);
            }
        }
        subject_polygon = output_polygon;
    }
    result = subject_polygon;
    return result;
}

void draw_clipped_triangle(vector<Point> clipped_polygon) {
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0); // Red color for the clipped triangle
    for (const auto& point : clipped_polygon) {
        glVertex2f(point.x, point.y);
    }
    glEnd();
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Original triangle
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 0.0, 1.0); // Blue color for the original triangle
    glVertex2f(0.0, 0.0);
    glVertex2f(25.0, 50.0);
    glVertex2f(50.0, 0.0);
    glEnd();

    //clipping area Rectangle
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(1.5);
    glVertex2f(10.0, 10.0);
    glVertex2f(40.0,10.0);
    glVertex2f(40.0, 40.0);
    glVertex2f(10.0, 40.0);
    glEnd();
    glLineWidth(0.5);
     


    // Clip the triangle against the clipping region
    vector<Point> subject_polygon = { {0.0, 0.0}, {25.0, 50.0}, {50.0, 0.0} };
    vector<Point> clipping_polygon = { {10.0, 10.0}, {40.0, 10.0}, {40.0, 40.0}, {10.0, 40.0} };
    vector<Point> clipped_polygon = clip_polygon(subject_polygon, clipping_polygon);

    // Draw the clipped triangle
    draw_clipped_triangle(clipped_polygon);

    glFlush();
}

void initialize() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 50.0, 0.0, 50.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Clipping Triangle");
    initialize();
    glutDisplayFunc(draw);
    glutMainLoop();
    return 0;
}
