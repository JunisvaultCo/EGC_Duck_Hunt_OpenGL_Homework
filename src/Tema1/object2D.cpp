#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace transform2D;

Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(0, 0, 1), color),
        VertexFormat(corner + glm::vec3(length, 0, 1), color),
        VertexFormat(corner + glm::vec3(length, length, 1), color),
        VertexFormat(corner + glm::vec3(0, length, 1), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateCircle(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float radius,
    std::vector<glm::vec3> colors,
    bool fill,
    int pointCount,
    int divideBy)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices;

    Mesh* circle = new Mesh(name);
    std::vector<unsigned int> indices;

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }
    
    int n = pointCount;
    double add = 0; 
    double x1 = corner.x;
    double y1 = corner.y;
    vertices.push_back(VertexFormat(glm::vec3(0, 0, 1), colors[0]));
    for (int i = 1; i <= n / divideBy + (divideBy != 1); i++) {
        x1 = cos(add) * radius;
        y1 = sin(add) * radius;
        add += (M_PI * 2) / n;
        vertices.push_back(VertexFormat(glm::vec3(x1, y1, 1), colors[i % colors.size()]));
    }
    for (int i = 0; i <= (n / divideBy + (divideBy != 1)) - 2; i++) {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }
    if (divideBy == 1) {
        indices.push_back(0);
        indices.push_back(n);
        indices.push_back(1);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* object2D::CreateCircle(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float radius,
    glm::vec3 color,
    bool fill,
    int pointCount,
    int divideBy)
{
    std::vector<glm::vec3> v {color};
    return object2D::CreateCircle(name, leftBottomCorner, radius, v, fill, pointCount, divideBy);
}


Mesh* object2D::CreatePolygon(
    const std::string &name,
    std::vector<glm::vec3> points,
    std::vector<glm::vec3> colors,
    bool fill)
{
    glm::vec3 corner = points[0];

    std::vector<VertexFormat> vertices;

    Mesh* mesh = new Mesh(name);
    std::vector<unsigned int> indices;

    for (unsigned int j = 0; j < points.size(); j++) {
        vertices.push_back(VertexFormat(points[j] + glm::vec3(0, 0, 1), colors[j]));
    }
    if (!fill) {
        mesh->SetDrawMode(GL_LINE_LOOP);
        for (unsigned int j = 0; j < points.size(); j++) {
            indices.push_back(j);
        }
    } else {
        for (unsigned int j = 0; j < points.size() - 2; j++) {
            indices.push_back(j);
            indices.push_back(j + 1);
            indices.push_back(j + 2);
        }
        indices.push_back(0);
        indices.push_back(points.size() - 1);
        indices.push_back(1);
    }

    mesh->InitFromData(vertices, indices);
    return mesh;
}


Mesh* object2D::CreateOctagram(
    const std::string &name,
    std::vector<glm::vec3> colors,
    double sideLength,
    bool fill)
{
    std::vector<VertexFormat> vertices;

    Mesh* octagram = new Mesh(name);
    std::vector<unsigned int> indices;

    if (!fill) {
        octagram->SetDrawMode(GL_LINE_LOOP);
    } else {
        octagram->SetDrawMode(GL_TRIANGLES);
    }
    double add = 0; 
    double x1 = 0;
    double y1 = 0;
    glm::vec2 sum = glm::vec2(0, 0);
    // create the triangle we will rotate 4 times
    double l = sideLength;
    double a = (l / sqrt(2))/ (3 + 2 * sqrt(2));
    double b = a * (1 + sqrt(2));
    double c = (a + b) / sqrt(2);
    glm::vec3 centerCoords = glm::vec3(l / 2, l / 2 - c, 0);
    
    std::vector<glm::vec3> trianglePos {
        glm::vec3(0, 0, 1),
        glm::vec3(l, 0, 1),
        glm::vec3(l / 2, l / 2, 1)
    };
    // center it
    for (int i = 0; i < 3; i++) {
        trianglePos[i] -= centerCoords;
    }
    // rotate the triangle 3 times
    for (int i = 0; i < 4; i++) {
        add += M_PI / 2;
        for (auto j: trianglePos) {
            vertices.push_back(VertexFormat(Rotate(add) * j, colors[i % colors.size()]));
            indices.push_back(((int)vertices.size()) - 1);
        }
    }

    octagram->InitFromData(vertices, indices);
    return octagram;
}