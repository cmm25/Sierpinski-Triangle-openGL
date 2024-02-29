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

Point calculate_intersection(Point p1, Point p2, Point p3, Point p4) {
    Point intersection_point;
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

void classify_intersection(Point& intersection, const Point& edge_start, const Point& edge_end, const Point& clip_vertex) {
    GLfloat dx = edge_end.x - edge_start.x;
    GLfloat dy = edge_end.y - edge_start.y;
    GLfloat dxi = clip_vertex.x - edge_start.x;
    GLfloat dyi = clip_vertex.y - edge_start.y;
    GLfloat cross_product = dx * dyi - dy * dxi;

    if (dy == 0) {
        if (dx > 0 && clip_vertex.y < edge_start.y)
            intersection.x = -1.0; // Entering
        else if (dx < 0 && clip_vertex.y > edge_start.y)
            intersection.x = -2.0; // Entering
        else
            intersection.x = -3.0; // Exiting
    }
    else if (cross_product > 0)
        intersection.x = -1.0; // Entering
    else
        intersection.x = -3.0; // Exiting
}

vector<Point> clip_polygon(const vector<Point>& subject_polygon, const vector<Point>& clipping_polygon) {
    vector<Point> result;
    vector<Point> subject = subject_polygon;

    for (size_t i = 0; i < clipping_polygon.size(); i++) {
        size_t next = (i + 1) % clipping_polygon.size();
        Point clip_start = clipping_polygon[i];
        Point clip_end = clipping_polygon[next];

        vector<Point> output_polygon;

        for (size_t j = 0; j < subject.size(); j++) {
            size_t next_subject = (j + 1) % subject.size();
            Point subject_start = subject[j];
            Point subject_end = subject[next_subject];

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
        subject = output_polygon;
    }

    for (const auto& point : subject) {
        if (point.x >= 0)
            result.push_back(point);
    }

    return result;
}

void draw_clipped_polygon(const vector<Point>& clipped_polygon) {
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0); // Red color for the clipped polygon
    for (const auto& point : clipped_polygon) {
        glVertex2f(point.x, point.y);
    }
    glEnd();
}

void draw_sierpinski_triangle(int n) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Original Sierpinski triangle (no need to clip here)
    vector<Point> subject_polygon = { {0.0, 0.0}, {25.0, 50.0}, {50.0, 0.0} };

    // Draw the first triangle directly (no clipping needed)
    draw_clipped_polygon(subject_polygon);

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
    glutCreateWindow("Clipped Sierpinski Triangle");
    initialize();
    glutDisplayFunc([]() { draw_sierpinski_triangle(3); });
    glutMainLoop();
    return 0;
}
