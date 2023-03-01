#pragma once

#include <string>
#include <vector>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
#include "Tema1/transform2D.h"

namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    // It will loop over the color palette if given less than the pointCount + 1
    // Note that the center will be 0.
    // divideBy parameter is for drawing halves or thirds of circles
    Mesh* CreateCircle(const std::string &name, glm::vec3 leftBottomCorner, float radius, std::vector<glm::vec3> colors, bool fill = false, int pointCount = 15, int divideBy = 1);
    Mesh* CreateCircle(const std::string &name, glm::vec3 leftBottomCorner, float radius, glm::vec3 color, bool fill = false, int pointCount = 15, int divideBy = 1);
    // this isn't centered.
    Mesh* CreatePolygon(const std::string &name, std::vector<glm::vec3> points, std::vector<glm::vec3> colors, bool fill = false);
    // yes, specialised function.
    Mesh* CreateOctagram(const std::string &name, std::vector<glm::vec3> colors, double sideLength, bool fill = false);
}
